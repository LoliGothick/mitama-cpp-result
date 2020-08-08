#ifndef MITAMA_RESULT_CONCEPTS_FORMATTABLE_HPP
#define MITAMA_RESULT_CONCEPTS_FORMATTABLE_HPP

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
  struct formattable_element_impl: std::false_type {};
/// Higher Kind Constraint
  template <class, class = void>
  struct formattable_range_impl : std::false_type {};
/// Higher Kind Constraint
  template <class, class = void>
  struct formattable_dictionary_impl: std::false_type {};
/// Higher Kind Constraint
  template <class, class = void>
  struct formattable_tuple_impl: std::false_type {};
/// Constraits Facade
  template <class T, class = void>
  struct formattable: std::false_type {};

  template <class T>
  struct formattable<T, std::enable_if_t<
          std::disjunction_v<
                  formattable_element_impl<T>,
                  formattable_range_impl<T>,
                  formattable_dictionary_impl<T>,
                  formattable_tuple_impl<T>
          >>>: std::true_type {};


  template < >
  struct formattable_element_impl<std::monostate>
          : std::true_type {};

  template < >
  struct formattable_element_impl<const std::monostate>
          : std::true_type {};

  template <class T>
  struct formattable_element_impl<T, std::void_t<decltype(std::declval<std::ostream &>() << std::declval<std::decay_t<T>>())>>
          : std::true_type {};

  template <class Range>
  struct formattable_range_impl<Range, std::enable_if_t<meta::is_range<Range>::value>>
          : formattable<typename std::remove_cvref_t<Range>::value_type> {};
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
  struct formattable_tuple_impl<Tuple, std::enable_if_t<meta::is_tuple_like<Tuple>::value>>
          : meta::detail::is_formattable_tuple_detail<Tuple, std::make_index_sequence<std::tuple_size_v<Tuple>>>
  {};

  template <class Dict>
  struct formattable_dictionary_impl<Dict, std::void_t<typename std::remove_cvref_t<Dict>::key_type, typename std::remove_cvref_t<Dict>::mapped_type>>
          : std::conjunction<formattable<typename std::remove_cvref_t<Dict>::key_type>, formattable<typename std::remove_cvref_t<Dict>::mapped_type>> {};

  template <class T> concept formattable_element = formattable_element_impl<T>::value;
  template <class T> concept formattable_range = formattable_range_impl<T>::value;
  template <class T> concept formattable_tuple = formattable_tuple_impl<T>::value;
  template <class T> concept formattable_dictionary = formattable_dictionary_impl<T>::value;

}

#endif
