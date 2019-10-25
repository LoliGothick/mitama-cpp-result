#ifndef MITAMA_RESULT_CONCEPTS_META_FUNCTION_HPP
#define MITAMA_RESULT_CONCEPTS_META_FUNCTION_HPP

namespace mitama {

template <class T>
concept meta_function = requires {
    { T::value } -> std::convertible_to<bool>;
};

}

#endif
