#ifndef MITAMA_MAYBE_FWD_HPP
#define MITAMA_MAYBE_FWD_HPP
#include <variant>
namespace mitama {

template <class...> class just_t;
template <class T> just_t(T&&) -> just_t<T>;

template <class> class maybe;

template <class T> maybe(just_t<T>&) -> maybe<T>;
template <class T> maybe(just_t<T>const&) -> maybe<const T>;
template <class T> maybe(just_t<T>&&) -> maybe<std::remove_reference_t<T>>;

template <class>
struct is_maybe: std::false_type {};

template <class T>
struct is_maybe<maybe<T>>: std::true_type {};

template <class, class>
struct is_maybe_with: std::false_type {};

template <class T>
struct is_maybe_with<maybe<T>, T>: std::true_type {};

}
#endif
