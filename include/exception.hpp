#ifndef SPAGHETTI_EXCEPTION
#define SPAGHETTI_EXCEPTION

#include <macros.hpp>

namespace spaghetti {

class future_error : public std::exception {
    public:
    explicit future_error (const char* what_arg);
    explicit future_error (const std::string& what_arg);

    future_error (const future_error&) noexcept            = default;
    future_error& operator= (const future_error&) noexcept = default;
    ~future_error () noexcept override                     = default;

    const char* what () const noexcept override;

    private:
    std::string what_arg_;
};

} // namespace spaghetti

#endif // SPAGHETTI_EXCEPTION