#ifndef MITAMA_RESULT_CONCEPTS_DEREFERENCABLE_HPP
#define MITAMA_RESULT_CONCEPTS_DEREFERENCABLE_HPP
#include <mitama/concepts/satisfy.hpp>

namespace mitama {

template <class T>
concept dereferencable = requires (T&& t) {
    { *t } -> satisfy<std::is_lvalue_reference>;
};

template <dereferencable For>
struct deref_type {
    using type = decltype(*std::declval<For>());
};

template <dereferencable For>
using deref_type_t = deref_type<For>::type;

}

#endif
