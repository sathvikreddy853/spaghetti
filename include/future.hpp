#ifndef SPAGHETTI_FUTURE
#define SPAGHETTI_FUTURE

#include <exception.hpp>
#include <macros.hpp>

namespace spaghetti {

template <typename T> struct shared_state {
    std::variant<std::monostate, T, std::exception_ptr> result;
    bool ready = false;
    std::condition_variable cv;
    std::mutex mutex;
};

template <typename T> class future {
    public:
    future () = default;

    /* Futures are not copyable */
    future (const future&)           = delete;
    future operator= (const future&) = delete;

    /* Futures are moveable */
    future (future&&)            = default;
    future& operator= (future&&) = default;

    explicit future (std::shared_ptr<shared_state<T>> state) : state_ (std::move (state)) {}

    T get () {
        if (not this->valid ()) throw future_error ("attempted to get value from invalid future");
        std::unique_lock lock(state_->mutex);
        state_->cv.wait(lock, [this]{ return state_->ready; });

        if (std::holds_alternative<std::exception_ptr>(state_->result))
            std::rethrow_exception(std::get<std::exception_ptr>(state_->result));
        
        return std::get<T>(state_->result);
    }

    bool valid () const noexcept { return state_ != nullptr; }

    private:
    std::shared_ptr<shared_state<T>> state_;
};

template <typename T> class promise {
    public:
    promise () { state_ = std::make_shared<shared_state<T>> (); }

    future<T> get_future () { return future<T> (state_); }

    void set_value (T value) {
        std::lock_guard lock (state_->mutex);
        if (state_->ready) throw future_error ("promise already contains a value or exception");
        state_->result = value;
        state_->ready  = true;
        state_->cv.notify_all ();
    }

    void set_exception (std::exception_ptr e) {
        std::lock_guard lock (state_->mutex);
        if (state_->ready) throw future_error ("promise already contains a value or exception");
        state_->result = e;
        state_->ready  = true;
        state_->cv.notify_all ();
    }

    private:
    std::shared_ptr<shared_state<T>> state_;
};

} // namespace spaghetti

#endif // SPAGHETTI_FUTURE