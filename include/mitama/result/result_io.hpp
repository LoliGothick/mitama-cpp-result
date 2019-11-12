#ifndef MITAMA_RESULT_RESULT_IO_HPP
#define MITAMA_RESULT_RESULT_IO_HPP
#include <mitama/result/result.hpp>
#include <boost/hana/functional/fix.hpp>
#include <boost/hana/functional/overload.hpp>
#include <boost/hana/functional/overload_linearly.hpp>
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <mitama/concepts/display.hpp>
namespace mitama {
/// @brief
///   ostream output operator
///
/// @constrains
///   Format<T>;
///   Format<E>
///
/// @note
///   Output its contained value with pretty format, and is used by `operator<<` found by ADL.
template <mutability _, display T, display E>
std::ostream&
operator<<(std::ostream& os, basic_result<_, T, E> const& res) {
  return res.is_ok() ? os << fmt::format("success({})", as_display(res.unwrap()).as_str()) 
                     : os << fmt::format("failure({})", as_display(res.unwrap_err()).as_str());
}

}

#endif
