#ifndef SPAGHETTI_FUTURE
#define SPAGHETTI_FUTURE

#include <macros.hpp>

namespace spaghetti {

enum class future_status {
    READY,
    DEFERRED,
    TIMEOUT,
};

template <typename T> class State {
    public:
    
    State (bool valid = true, bool readable = false, value = std::nullopt)
    : valid (valid), readable (readable), value (value) {}
    
    private: 
    std::condition_variable cv;
    bool valid; 
    bool readable;
    std::optional<T> value;
    void wait() const;
};

template <typename T> class Future {
    Future () = default;
    T get ();
    bool valid ();
    void wait () const;

    private:
    State state;
    friend State;
};

} // namespace spaghetti

#endif // SPAGHETTI_FUTURE