
```cpp
// Compare two just_t objects
template< class T, class U >
constexpr bool operator==( const just_t<T>& lhs, const just_t<U>& rhs ); // (43)
template< class T, class U >
constexpr bool operator!=( const just_t<T>& lhs, const just_t<U>& rhs ); // (44)
template< class T, class U >
constexpr bool operator<( const just_t<T>& lhs, const just_t<U>& rhs );  // (45)
template< class T, class U >
constexpr bool operator<=( const just_t<T>& lhs, const just_t<U>& rhs ); // (46)
template< class T, class U >
constexpr bool operator>( const just_t<T>& lhs, const just_t<U>& rhs );  // (47)
template< class T, class U >
constexpr bool operator>=( const just_t<T>& lhs, const just_t<U>& rhs ); // (48)

// Compare a just_t object with a nothing
template< class T >
constexpr bool operator==( const just_t<T>& opt, nothing_t ) noexcept; // (49)
template< class T >
constexpr bool operator==( nothing_t, const just_t<T>& opt ) noexcept; // (50)
template< class T >
constexpr bool operator!=( const just_t<T>& opt, nothing_t ) noexcept; // (51)
template< class T >
constexpr bool operator!=( nothing_t, const just_t<T>& opt ) noexcept; // (52)
template< class T >
constexpr bool operator<( const just_t<T>& opt, nothing_t ) noexcept;  // (53)
template< class T >
constexpr bool operator<( nothing_t, const just_t<T>& opt ) noexcept;  // (54)
template< class T >
constexpr bool operator<=( const just_t<T>& opt, nothing_t ) noexcept; // (55)
template< class T >
constexpr bool operator<=( nothing_t, const just_t<T>& opt) noexcept;  // (56)
template< class T >
constexpr bool operator>( const just_t<T>& opt, nothing_t ) noexcept;  // (57)
template< class T >
constexpr bool operator>( nothing_t, const just_t<T>& opt ) noexcept;  // (58)
template< class T >
constexpr bool operator>=( const just_t<T>& opt, nothing_t ) noexcept; // (59)
template< class T >
constexpr bool operator>=( nothing_t, const just_t<T>& opt ) noexcept; // (60)

// Compare two nothing objects
constexpr bool operator==( nothing_t, nothing_t ) noexcept; // (61)
constexpr bool operator!=( nothing_t, nothing_t ) noexcept; // (62)
constexpr bool operator<( nothing_t, nothing_t ) noexcept;  // (63)
constexpr bool operator<=( nothing_t, nothing_t ) noexcept; // (64)
constexpr bool operator>( nothing_t, nothing_t ) noexcept;  // (65)
constexpr bool operator>=( nothing_t, nothing_t ) noexcept; // (66)
```
