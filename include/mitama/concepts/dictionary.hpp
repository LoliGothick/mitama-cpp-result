#ifndef MITAMA_RESULT_CONCEPTS_DICTIONARY_HPP
#define MITAMA_RESULT_CONCEPTS_DICTIONARY_HPP
#include <mitama/concepts/range.hpp>

namespace mitama {

template <class T>
concept dictionary_impl = requires {
    typename T::value_type;
    typename T::key_type;
    typename T::mapped_type;
    requires std::same_as<typename T::value_type, std::pair<const typename T::key_type, typename T::mapped_type>>;
};

template <class T>
concept dictionary = forward_range<T> && dictionary_impl<T>;

}

#endif
