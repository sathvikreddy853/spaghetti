#include <exception.hpp>

namespace spaghetti {

future_error::future_error (const char* what_arg)
: what_arg_ ("future_error: " + std::string (what_arg)) {}

future_error::future_error (const std::string& what_arg)
: what_arg_ ("future_error: " + what_arg) {}

const char* future_error::what () const noexcept { return what_arg_.c_str (); }

} // namespace spaghetti