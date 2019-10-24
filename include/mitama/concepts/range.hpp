#ifndef MITAMA_RESULT_CONCEPTS_DICTIONARY_HPP
#define MITAMA_RESULT_CONCEPTS_DICTIONARY_HPP
#include <iterator>

namespace mitama {

namespace impl {
using std::begin, std::end;

template< class T >
concept range_impl = requires(T&& t) {
  begin(std::forward<T>(t));
  end  (std::forward<T>(t));
};
}

template< class T >
concept range = impl::range_impl<T&>;

}

#endif
