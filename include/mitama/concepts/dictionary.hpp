#ifndef MITAMA_RESULT_CONCEPTS_DICTIONARY_HPP
#define MITAMA_RESULT_CONCEPTS_DICTIONARY_HPP
#include <mitama/concepts/range.hpp>

namespace mitama {

template <class T>
concept dictionary_impl = requires {
    // type requirements
    typename T::value_type;
    typename T::key_type;
    typename T::mapped_type;
    requires std::same_as<typename T::value_type, std::pair<const typename T::key_type, typename T::mapped_type>>;
    // behavior requirements
    requires (T& dict, typename T::key_type const& key) {
        { dict[key] } -> std::same_as<typename T::mapped_type&>;
    }
};

template <class T>
concept dictionary = forward_range<T> && dictionary_impl<T>;

}

#endif
