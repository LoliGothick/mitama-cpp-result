## operator==, !=, &lt;, &lt;=, &gt;, &gt;=(maybe)

```cpp
namespace mitama {
// Compare two maybe objects
template< class T, class U >
constexpr bool operator==( const maybe<T>& lhs, const maybe<U>& rhs ); // (1)
template< class T, class U >
constexpr bool operator!=( const maybe<T>& lhs, const maybe<U>& rhs ); // (2)
template< class T, class U >
constexpr bool operator<( const maybe<T>& lhs, const maybe<U>& rhs );  // (3)
template< class T, class U >
constexpr bool operator<=( const maybe<T>& lhs, const maybe<U>& rhs ); // (4)
template< class T, class U >
constexpr bool operator>( const maybe<T>& lhs, const maybe<U>& rhs );  // (5)
template< class T, class U >
constexpr bool operator>=( const maybe<T>& lhs, const maybe<U>& rhs ); // (6)

// Compare a maybe object with a nothing
template< class T >
constexpr bool operator==( const maybe<T>& opt, nothing_t ) noexcept; // (7)
template< class T >
constexpr bool operator==( nothing_t, const maybe<T>& opt ) noexcept; // (8)
template< class T >
constexpr bool operator!=( const maybe<T>& opt, nothing_t ) noexcept; // (9)
template< class T >
constexpr bool operator!=( nothing_t, const maybe<T>& opt ) noexcept; // (10)
template< class T >
constexpr bool operator<( const maybe<T>& opt, nothing_t ) noexcept;  // (11)
template< class T >
constexpr bool operator<( nothing_t, const maybe<T>& opt ) noexcept;  // (12)
template< class T >
constexpr bool operator<=( const maybe<T>& opt, nothing_t ) noexcept; // (13)
template< class T >
constexpr bool operator<=( nothing_t, const maybe<T>& opt) noexcept;  // (14)
template< class T >
constexpr bool operator>( const maybe<T>& opt, nothing_t ) noexcept;  // (15)
template< class T >
constexpr bool operator>( nothing_t, const maybe<T>& opt ) noexcept;  // (16)
template< class T >
constexpr bool operator>=( const maybe<T>& opt, nothing_t ) noexcept; // (17)
template< class T >
constexpr bool operator>=( nothing_t, const maybe<T>& opt ) noexcept; // (18)

// Compare a maybe object with a just(value)
template< class T, class U >
constexpr bool operator==( const maybe<T>& opt, just_t<U> const& some ); // (19)
template< class T, class U >
constexpr bool operator==( just_t<T> const& some, const maybe<U>& opt ); // (20)
template< class T, class U >
constexpr bool operator!=( const maybe<T>& opt, just_t<U> const& some ); // (21)
template< class T, class U >
constexpr bool operator!=( just_t<T> const& some, const maybe<U>& opt ); // (22)
template< class T, class U >
constexpr bool operator<( const maybe<T>& opt, just_t<U> const& some );  // (23)
template< class T, class U >
constexpr bool operator<( just_t<T> const& some, const maybe<U>& opt );  // (24)
template< class T, class U >
constexpr bool operator<=( const maybe<T>& opt, just_t<U> const& some ); // (25)
template< class T, class U >
constexpr bool operator<=( just_t<T> const& some, const maybe<U>& opt ); // (26)
template< class T, class U >
constexpr bool operator>( const maybe<T>& opt, just_t<U> const& some );  // (27)
template< class T, class U >
constexpr bool operator>( just_t<T> const& some, const maybe<U>& opt );  // (28)
template< class T, class U >
constexpr bool operator>=( const maybe<T>& opt, just_t<U> const& some ); // (29)
template< class T, class U >
constexpr bool operator>=( just_t<T> const& some, const maybe<U>& opt ); // (30)

// Compare a maybe object with a T
template< class T, class U >
constexpr bool operator==( const maybe<T>& opt, const U& value ); // (31)
template< class T, class U >
constexpr bool operator==( const T& value, const maybe<U>& opt ); // (32)
template< class T, class U >
constexpr bool operator!=( const maybe<T>& opt, const U& value ); // (33)
template< class T, class U >
constexpr bool operator!=( const T& value, const maybe<U>& opt ); // (34)
template< class T, class U >
constexpr bool operator<( const maybe<T>& opt, const U& value );  // (35)
template< class T, class U >
constexpr bool operator<( const T& value, const maybe<U>& opt );  // (36)
template< class T, class U >
constexpr bool operator<=( const maybe<T>& opt, const U& value ); // (37)
template< class T, class U >
constexpr bool operator<=( const T& value, const maybe<U>& opt);  // (38)
template< class T, class U >
constexpr bool operator>( const maybe<T>& opt, const U& value );  // (39)
template< class T, class U >
constexpr bool operator>( const T& value, const maybe<U>& opt );  // (40)
template< class T, class U >
constexpr bool operator>=( const maybe<T>& opt, const U& value ); // (41)
template< class T, class U >
constexpr bool operator>=( const T& value, const maybe<U>& opt ); // (42)
}
```

Performs comparison operations on maybe objects.

1-6) Compares two maybe objects, `lhs` and `rhs`. The contained values are compared (using the corresponding operator of `T` and `U`) only if both `lhs` and `rhs` contain values.

Otherwise,

- `lhs` is considered equal to `rhs` if, and only if, both `lhs` and `rhs` do not contain a value.
- `lhs` is considered less than `rhs` if, and only if, `rhs` contains a value and `lhs` does not.

7-18) Compares `opt` with a `nothing`. Equivalent to (1-6) when comparing to a maybe that does not contain a value.

19-30) Compares `opt` with a `some := just_t(value)`. The values are compared (using the corresponding operator of `T`) only if `opt` contains a value.
Otherwise, `opt` is considered less than `some`.

If the corresponding comparison expression between `opt.unwrap()` and value is not well-formed, or if its result is not convertible to bool, the behavior is undefined.

31-42) Compares `opt` with a `value`. Equivalent to (19-30) expression `opt ~ just(value)`.

**Parameters**

`lhs`, `rhs`, `opt`	-	a maybe object to compare

`some` - a just_t object to compare

`value`	-	value to compare to the contained value


**Return value**

1. If `bool(lhs) != bool(rhs)`, returns false
  
    Otherwise, if `bool(lhs) == false` (and so `bool(rhs) == false` as well), returns true

    Otherwise, returns `lhs.unwrap() == rhs.unwrap()`.

2. If `bool(lhs) != bool(rhs)`, returns true

    Otherwise, if `bool(lhs) == false` (and so `bool(rhs) == false` as well), returns false

    Otherwise, returns `lhs.unwrap() != rhs.unwrap()`.

3. If `bool(rhs) == false` returns false

    Otherwise, if `bool(lhs) == false`, returns true

    Otherwise returns `lhs.unwrap() < rhs.unwrap()`.

4. If `bool(lhs) == false` returns true

    Otherwise, if `bool(rhs) == false`, returns false

    Otherwise returns `lhs.unwrap() <= rhs.unwrap()`.

5. If `bool(lhs) == false` returns false

    Otherwise, if `bool(rhs) == false`, returns true

    Otherwise returns `lhs.unwrap() > rhs.unwrap()`.

6. If `bool(rhs) == false` returns true

    Otherwise, if `bool(lhs) == false`, returns false

    Otherwise returns `lhs.unwrap() >= rhs.unwrap()`.

7. Returns `opt.is_nothing()`.

8. Returns `opt.is_nothing()`.

9. Returns `opt.is_just()`.

10. Returns `opt.is_just()`.

11. Returns `false`.

12. Returns `opt.is_just()`.

13. Returns `opt.is_nothing()`.

14. Returns `true`.

15. Returns `opt.is_just()`.

16. Returns `false`.

17. Returns `true`.

18. Returns `opt.is_nothing()`.

19. Returns `opt.is_just() ? opt.unwrap() == some.value : false`.

20. Returns `opt.is_just() ? some.value == opt.unwrap() : false`.

21. Returns `opt.is_just() ? opt.unwrap() != some.value : true`.

22. Returns `opt.is_just() ? some.value != opt.unwrap() : true`.

23. Returns `opt.is_just() ? opt.unwrap() < some.value  : true`.

24. Returns `opt.is_just() ? some.value < opt.unwrap()  : false`.

25. Returns `opt.is_just() ? opt.unwrap() <= some.value : true`.

26. Returns `opt.is_just() ? some.value <= opt.unwrap() : false`.

27. Returns `opt.is_just() ? opt.unwrap() > some.value  : false`.

28. Returns `opt.is_just() ? some.value > opt.unwrap()  : true`.

29. Returns `opt.is_just() ? opt.unwrap() >= some.value : false`.

30. Returns `opt.is_just() ? some.value >= opt.unwrap() : true`.

31. Returns `opt == just(value)`.

32. Returns `just(value) == opt`.

33. Returns `opt != just(value)`.

34. Returns `just(value) != opt`.

35. Returns `opt < just(value)`.

36. Returns `just(value) < opt`.

37. Returns `opt <= just(value)`.

38. Returns `just(value) <= opt`.

39. Returns `opt > just(value)`.

40. Returns `just(value) > opt`.

41. Returns `opt >= just(value)`.

42. Returns `just(value) >= opt`.

**Note**

- `some.value`: `value` is a private member of `just_t` (actually, cannot access to it).
