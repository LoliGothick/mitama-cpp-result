#ifndef MITAMA_CMP_ORD_HPP
#define MITAMA_CMP_ORD_HPP
#include <concepts>
#include <compare>
#include <mitama/result/detail/fwd.hpp>

namespace mitama::cmp {
template <class Lhs, template<class...> class Rhs>
struct ord {
    template <class... Ts>
    friend bool operator==(const Lhs& lhs, const Rhs<Ts...>& rhs)
    { return lhs.operator<=>(rhs) == std::strong_ordering::equivalent; }
    template <class... Ts>
    friend bool operator!=(const Lhs& lhs, const Rhs<Ts...>& rhs)
    { return lhs.operator<=>(rhs) != std::strong_ordering::equivalent; }
    template <class... Ts>
    friend bool operator<(const Lhs& lhs, const Rhs<Ts...>& rhs)
    { return lhs.operator<=>(rhs) == std::strong_ordering::less; }
    template <class... Ts>
    friend bool operator<=(const Lhs& lhs, const Rhs<Ts...>& rhs)
    { return lhs.operator<=>(rhs) != std::strong_ordering::greater; }
    template <class... Ts>
    friend bool operator>(const Lhs& lhs, const Rhs<Ts...>& rhs)
    { return lhs.operator<=>(rhs) == std::strong_ordering::greater; }
    template <class... Ts>
    friend bool operator>=(const Lhs& lhs, const Rhs<Ts...>& rhs)
    { return lhs.operator<=>(rhs) != std::strong_ordering::less; }
};

template <class Lhs>
struct result_ord {
    template <mutability _, class T, class E>
    friend bool operator==(const Lhs& lhs, const basic_result<_,T,E>& rhs)
    { return lhs.operator<=>(rhs) == std::strong_ordering::equivalent; }
    template <mutability _, class T, class E>
    friend bool operator!=(const Lhs& lhs, const basic_result<_,T,E>& rhs)
    { return lhs.operator<=>(rhs) != std::strong_ordering::equivalent; }
    template <mutability _, class T, class E>
    friend bool operator<(const Lhs& lhs, const basic_result<_,T,E>& rhs)
    { return lhs.operator<=>(rhs) == std::strong_ordering::less; }
    template <mutability _, class T, class E>
    friend bool operator<=(const Lhs& lhs, const basic_result<_,T,E>& rhs)
    { return lhs.operator<=>(rhs) != std::strong_ordering::greater; }
    template <mutability _, class T, class E>
    friend bool operator>(const Lhs& lhs, const basic_result<_,T,E>& rhs)
    { return lhs.operator<=>(rhs) == std::strong_ordering::greater; }
    template <mutability _, class T, class E>
    friend bool operator>=(const Lhs& lhs, const basic_result<_,T,E>& rhs)
    { return lhs.operator<=>(rhs) != std::strong_ordering::less; }
};
}

#endif
