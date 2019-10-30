#ifndef MITAMA_RESULT_DETAIL_META_HPP
#define MITAMA_RESULT_DETAIL_META_HPP
#include <type_traits>
#include <optional>
#include <utility>
#include <tuple>
#include <boost/optional/optional_fwd.hpp>

namespace mitama {
inline namespace meta {

template < class T, class = void >
struct has_type: std::false_type {};

template < class T >
struct has_type<T, std::void_t<typename std::decay_t<T>::type>>: std::true_type {};

template < class T, class = void >
struct has_value: std::false_type {};

template < class T >
struct has_value<T, std::void_t<decltype(std::decay_t<T>::value)>>: std::true_type {};

template < class T > struct force_add_const: std::add_const<T> {};
template < class T > struct force_add_const<T&> { using type = std::add_const_t<T>&; };
template < class T > struct force_add_const<T&&> { using type = std::add_const_t<T>&&; };
template < class T > using force_add_const_t = typename force_add_const<T>::type;

}}

namespace mitama {
inline namespace meta {
namespace detail {
  template <class AlwaysVoid, class... Types> struct has_common_type: std::false_type {};
  template <class... Types>
  struct has_common_type<std::enable_if_t<::mitama::meta::has_type<std::common_type<Types...>>::value>, Types...>: std::true_type {};
}}}

namespace mitama {
inline namespace meta {
  template <class... Types>
  struct has_common_type: detail::has_common_type<void, Types...> {};
}
}

namespace mitama {
inline namespace meta {
namespace detail {
template < class, class, class = void > struct is_tuple_like_detail: std::false_type {};

template < class T, std::size_t... I >
struct is_tuple_like_detail<T, std::index_sequence<I...>, std::void_t<decltype(std::get<I>(std::declval<T>()))...>>
  : std::conjunction<has_type<std::tuple_element<I, T>>...>
{};

template < class TupleLike >
struct is_tuple_like_impl: is_tuple_like_detail<TupleLike, std::make_index_sequence<std::tuple_size_v<TupleLike>>> {};
}}}

namespace mitama {
inline namespace meta {
/// is_tuple_like
/// requires
///   std::tuple_element<0, T>
///   std::tuple_size_v<T>
///   (T t) { std::get<0>(t) }
template < class T >
struct is_tuple_like
  : std::conjunction<
      has_value<std::tuple_size<T>>,
      detail::is_tuple_like_impl<T>
    >
{};
}}

namespace mitama {
inline namespace meta {

/// is_optional family
template < class >
struct is_std_optional: std::false_type {};
template < class >
struct is_boost_optional: std::false_type {};

template < class T >
struct is_std_optional<std::optional<T>>: std::true_type {};
template < class T >
struct is_boost_optional<boost::optional<T>>: std::true_type {};

template < class T >
struct is_optional
  : std::disjunction<
      meta::is_std_optional<std::remove_cvref_t<T>>,
      meta::is_boost_optional<std::remove_cvref_t<T>>
    >
{};

/// is_range
template <class, class = void> struct is_range: std::false_type {};
template <class Range>
struct is_range<Range, std::void_t<decltype(*std::begin(std::declval<std::decay_t<Range>>()), std::begin(std::declval<std::decay_t<Range>>()) != std::end(std::declval<std::decay_t<Range>>()))>>
  : std::true_type {};

/// is_dictionary
template <class, class = void> struct is_dictionary: std::false_type {};
template <class Dict>
struct is_dictionary<Dict, std::void_t<typename std::remove_cvref_t<Dict>::key_type, typename std::remove_cvref_t<Dict>::mapped_type>>
  : is_range<Dict> {};

template < class > struct repack;

template < template < class > class Opt, class T >
struct repack<Opt<T>> {
  template < class U >
  using type = Opt<U>;
};
}}

#endif
