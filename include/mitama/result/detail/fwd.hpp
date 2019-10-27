#ifndef MITAMA_RESULT_FWD
#define MITAMA_RESULT_FWD
#include <variant>
#include <type_traits>
#include <concepts>

namespace mitama {

/// for mutability control
enum class mutability: bool {
    mut = false,
    immut = true,
};

constexpr mutability operator&&(mutability _1, mutability _2) {
    return mutability{ !(!static_cast<bool>(_1) && !static_cast<bool>(_2)) };
}

template < mutability Mut >
inline constexpr bool is_mut_v = !static_cast<bool>(Mut);

template <mutability,
          class T = std::monostate,  // success type
          class E = std::monostate   // failure type
>
  requires (std::is_object_v<std::remove_cvref_t<T>>)
        && (std::is_object_v<std::remove_cvref_t<E>>)
        && (std::negation_v<std::is_array<std::remove_cvref_t<T>>>)
        && (std::negation_v<std::is_array<std::remove_cvref_t<E>>>)
class basic_result;

/// alias template for immutable result
template <class T = std::monostate, class E = std::monostate>
using result = basic_result<mutability::immut, T, E>;

/// alias template for mutable result
template <class T = std::monostate, class E = std::monostate>
using mut_result = basic_result<mutability::mut, T, E>;

template <class T = std::monostate>
class success;

/// Deduction guide for `success`
template <class T>
success(T&&)->success<T>;

template <class = std::monostate>
class failure;

/// Deduction guide for `failure`
template <class E>
failure(E&&)->failure<E>;

class in_place_ok_t {};
inline constexpr in_place_ok_t in_place_ok = {};

class in_place_err_t {};
inline constexpr in_place_err_t in_place_err = {};

}

namespace mitama {

template <class>
struct is_result : std::false_type {};
template <mutability _mut, class T, class E>
struct is_result<basic_result<_mut, T, E>> : std::true_type {};
template <class T>
inline constexpr bool is_result_v = is_result<T>::value;

template <class, class...>
struct is_convertible_result_with : std::false_type {};
template <mutability _mut, class T, class E, class U>
struct is_convertible_result_with<basic_result<_mut, T, E>, success<U>>: std::is_constructible<U, T> {};
template <mutability _mut, class T, class E, class F>
struct is_convertible_result_with<basic_result<_mut, T, E>, failure<F>>: std::is_constructible<F, E> {};
template <mutability _mut, class T, class E, class U, class F>
struct is_convertible_result_with<basic_result<_mut, T, E>, success<U>, failure<F>>
  : std::conjunction<
      std::is_constructible<U, T>,
      std::is_constructible<F, E>>
{};

template <class T, class... Requires>
inline constexpr bool is_convertible_result_with_v = is_convertible_result_with<std::remove_cvref_t<T>, Requires...>::value;

template <class>
struct is_err_type : std::false_type {};
template <class T>
struct is_err_type<failure<T>> : std::true_type {};
template <class>
struct is_ok_type : std::false_type {};
template <class T>
struct is_ok_type<success<T>> : std::true_type {};

struct [[nodiscard]] nothing_t {};

inline constexpr nothing_t nothing{};

} // !namespace mitama


#endif
