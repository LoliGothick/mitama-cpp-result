#ifndef MITAMA_RESULT_TRAIT_HPP
#define MITAMA_RESULT_TRAIT_HPP

#include <mitama/result/detail/meta.hpp>
#include <type_traits>
#include <utility>
#include <iostream>
#include <string>
#include <string_view>

namespace mitama::trait {

// Trait Helpers

template < class ... Requires >
using where = std::enable_if_t<std::conjunction_v<Requires...>, std::nullptr_t>;

inline constexpr std::nullptr_t required = nullptr;


/// Atomic Constraint
template <class T, class = void>
struct formattable_element: std::false_type {};
/// Higher Kind Constraint
template <class, class = void>
struct formattable_range : std::false_type {};
/// Higher Kind Constraint
template <class, class = void>
struct formattable_dictionary: std::false_type {};
/// Higher Kind Constraint
template <class, class = void>
struct formattable_tuple: std::false_type {};
/// Constraits Facade
template <class T, class = void>
struct formattable: std::false_type {};

template <class T>
struct formattable<T, std::enable_if_t<
    std::disjunction_v<
        formattable_element<T>,
        formattable_range<T>,
        formattable_dictionary<T>,
        formattable_tuple<T>
>>>: std::true_type {}; 


template < >
struct formattable_element<std::monostate>
    : std::true_type {};

template < >
struct formattable_element<const std::monostate>
    : std::true_type {};

template <class T>
struct formattable_element<T, std::void_t<decltype(std::declval<std::ostream &>() << std::declval<std::decay_t<T>>())>>
    : std::true_type {};

template <class Range>
struct formattable_range<Range, std::enable_if_t<meta::is_range<Range>::value>>
    : formattable<typename meta::remove_cvr_t<Range>::value_type> {};
}

namespace mitama {
inline namespace meta {
namespace detail {
    template <class, class>
    struct is_formattable_tuple_detail: std::false_type {};

    template <class Tuple, std::size_t... I>
    struct is_formattable_tuple_detail<Tuple, std::index_sequence<I...>>
        : std::conjunction<trait::formattable<std::tuple_element_t<I, Tuple>>...>
    {};
}}}

namespace mitama::trait {

template <class Tuple>
struct formattable_tuple<Tuple, std::enable_if_t<meta::is_tuple_like<Tuple>::value>>
    : meta::detail::is_formattable_tuple_detail<Tuple, std::make_index_sequence<std::tuple_size_v<Tuple>>>
{};

template <class Dict>
struct formattable_dictionary<Dict, std::void_t<typename meta::remove_cvr_t<Dict>::key_type, typename meta::remove_cvr_t<Dict>::mapped_type>>
    : std::conjunction<formattable<typename meta::remove_cvr_t<Dict>::key_type>, formattable<typename meta::remove_cvr_t<Dict>::mapped_type>> {};

}

#endif