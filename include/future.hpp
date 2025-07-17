#ifndef SPAGHETTI_FUTURE
#define SPAGHETTI_FUTURE

#include <macros.hpp>

namespace spaghetti {

struct SharedState {
    int value;
    bool valid    = false;
    bool readable = false;
    std::condition_variable cv;
    std::mutex mutex;
};

struct Future {
    Future () {}

    Future (std::shared_ptr<SharedState> state) : state (state) {
        state->mutex.lock ();
        state->valid = true;
        state->mutex.unlock ();
    }

    int get () {
        std::unique_lock lock (state->mutex);
        state->cv.wait (lock, [this] { return state->readable; });
        return state->value;
    }

    std::shared_ptr<SharedState> state;
};

struct Promise {
    Promise () { state = std::make_shared<SharedState> (); }

    Future get_future () { return Future (state); }

    void set_value (int value) {
        std::unique_lock lock (state->mutex);
        state->value    = value;
        state->readable = true;
        lock.unlock ();
        state->cv.notify_one ();
    }

    std::shared_ptr<SharedState> state;
};

} // namespace spaghetti

#endif