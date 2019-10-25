#ifndef MITAMA_RESULT_CONCEPTS_POINTER_LIKE_HPP
#define MITAMA_RESULT_CONCEPTS_POINTER_LIKE_HPP

namespace mitama {

template <class T>
concept pointer_like = requires (T&& p) {
    p.operator->();
    *p;
    bool(p);
};

}

#endif
