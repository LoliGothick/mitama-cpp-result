## operator==, !=, &lt;, &lt;=, &gt;, &gt;=(basic_result)

```cpp
namespace mitama {
// Compare two basic_result objects
template< class T, class E, class U, class F >
constexpr bool operator==( const basic_result<T, E>& lhs,
                           const basic_result<U, F>& rhs ); // (1)
template< class T, class E, class U, class F >
constexpr bool operator!=( const basic_result<T, E>& lhs,
                           const basic_result<U, F>& rhs ); // (2)
template< class T, class E, class U, class F >
constexpr bool operator<( const basic_result<T, E>& lhs,
                          const basic_result<U, F>& rhs );  // (3)
template< class T, class E, class U, class F >
constexpr bool operator<=( const basic_result<T, E>& lhs,
                           const basic_result<U, F>& rhs ); // (4)
template< class T, class E, class U, class F >
constexpr bool operator>( const basic_result<T, E>& lhs,
                          const basic_result<U, F>& rhs );  // (5)
template< class T, class E, class U, class F >
constexpr bool operator>=( const basic_result<T, E>& lhs,
                           const basic_result<U, F>& rhs ); // (6)

// Compare a basic_result object with a success(value)
template< class T, class E, class U >
constexpr bool operator==( const basic_result<T, E>& res,
                           success<U> const& ok );          // (7)
template< class T, class E, class U >
constexpr bool operator==( success<T> const& ok,
                           const basic_result<U, E>& res ); // (8)
template< class T, class E, class U >
constexpr bool operator!=( const basic_result<T, E>& res,
                           success<U> const& ok );          // (9)
template< class T, class E, class U >
constexpr bool operator!=( success<T> const& ok,
                           const basic_result<U, E>& res ); // (10)
template< class T, class E, class U >
constexpr bool operator<( const basic_result<T, E>& res,
                          success<U> const& ok );           // (11)
template< class T, class E, class U >
constexpr bool operator<( success<T> const& ok,
                          const basic_result<U, E>& res );  // (12)
template< class T, class E, class U >
constexpr bool operator<=( const basic_result<T, E>& res,
                           success<U> const& ok );          // (13)
template< class T, class E, class U >
constexpr bool operator<=( success<T> const& ok,
                           const basic_result<U, E>& res ); // (14)
template< class T, class E, class U >
constexpr bool operator>( const basic_result<T, E>& res,
                          success<U> const& ok );           // (15)
template< class T, class E, class U >
constexpr bool operator>( success<T> const& ok,
                          const basic_result<U, E>& res );  // (16)
template< class T, class E, class U >
constexpr bool operator>=( const basic_result<T, E>& res,
                           success<U> const& ok );          // (17)
template< class T, class E, class U >
constexpr bool operator>=( success<T> const& ok, 
                           const basic_result<U, E>& res ); // (18)

// Compare a basic_result object with a failure(value)
template< class T, class E, class F >
constexpr bool operator==( const basic_result<T, E>& res,
                           failure<F> const& err );         // (19)
template< class T, class E, class U >
constexpr bool operator==( failure<E> const& err,
                           const basic_result<T, F>& res ); // (20)
template< class T, class E, class U >
constexpr bool operator!=( const basic_result<T, E>& res,
                           failure<F> const& err );         // (21)
template< class T, class E, class U >
constexpr bool operator!=( failure<E> const& err,
                           const basic_result<T, F>& res ); // (22)
template< class T, class E, class U >
constexpr bool operator<( const basic_result<T, E>& res,
                          failure<F> const& err );          // (23)
template< class T, class E, class U >
constexpr bool operator<( failure<E> const& err,
                          const basic_result<T, F>& res );  // (24)
template< class T, class E, class U >
constexpr bool operator<=( const basic_result<T, E>& res,
                          failure<F> const& err );          // (25)
template< class T, class E, class U >
constexpr bool operator<=( failure<E> const& err,
                          const basic_result<T, F>& res );  // (26)
template< class T, class E, class U >
constexpr bool operator>( const basic_result<T, E>& res,
                          failure<F> const& err );          // (27)
template< class T, class E, class U >
constexpr bool operator>( failure<E> const& err,
                          const basic_result<T, F>& res );  // (28)
template< class T, class E, class U >
constexpr bool operator>=( const basic_result<T, E>& res,
                          failure<F> const& err );          // (29)
template< class T, class E, class U >
constexpr bool operator>=( failure<E> const& err,
                          const basic_result<T, F>& res );  // (30)

// Compare a basic_result object with a T
template< class T, class E, class U >
constexpr bool operator==( const basic_result<T, E>& res,
                           const U& value );                // (31)
template< class T, class E, class U >
constexpr bool operator==( const T& value,
                           const basic_result<U, E>& res ); // (32)
template< class T, class E, class U >
constexpr bool operator!=( const basic_result<T, E>& res,
                           const U& value );                // (33)
template< class T, class E, class U >
constexpr bool operator!=( const T& value,
                           const basic_result<U, E>& res ); // (34)
template< class T, class E, class U >
constexpr bool operator<( const basic_result<T, E>& res,
                          const U& value );                 // (35)
template< class T, class E, class U >
constexpr bool operator<( const T& value,
                          const basic_result<U, E>& res );  // (36)
template< class T, class E, class U >
constexpr bool operator<=( const basic_result<T, E>& res,
                           const U& value );                // (37)
template< class T, class E, class U >
constexpr bool operator<=( const T& value,
                           const basic_result<U, E>& res);  // (38)
template< class T, class E, class U >
constexpr bool operator>( const basic_result<T, E>& res,
                          const U& value );                 // (39)
template< class T, class E, class U >
constexpr bool operator>( const T& value,
                          const basic_result<U, E>& res );  // (40)
template< class T, class E, class U >
constexpr bool operator>=( const basic_result<T, E>& res,
                           const U& value );                // (41)
template< class T, class E, class U >
constexpr bool operator>=( const T& value,
                           const basic_result<U, E>& res ); // (42)
}
```

Performs comparison operations on basic_result objects.

1-6) Compares two basic_result objects, `lhs` and `rhs`. The contained values are compared (using the corresponding operator of `T` and `U` or `E` and `F`) only if the contents of `lhs` and `rhs` are either both success values or are both failure values.

Otherwise,

- `lhs` is considered less than `rhs` if, and only if, `rhs` contains a success value and `lhs` contains failure value.
- `lhs` is considered less than `rhs` if, and only if, `rhs` contains a failure value and `lhs` contains success value.

7-18) Compares `res` with a `ok`. The values are compared (using the corresponding operator of `E` and `F`) only if `res` contains a success value.
Otherwise, `res` is considered less than `ok`.

If the corresponding comparison expression between `res.unwrap()` and success value is not well-formed, or if its result is not convertible to bool, the behavior is undefined.

19-30) Compares `res` with a `err`. The values are compared (using the corresponding operator of `T` and `U`) only if `res` contains a success value.
Otherwise, `res` is considered greater than `err`.

If the corresponding comparison expression between `res.unwrap()` and failure value is not well-formed, or if its result is not convertible to bool, the behavior is undefined.

31-42) Compares `res` with a `value`. Equivalent to (7-18) expression `res ~ success(value)` or `success(value) ~ res`.

**Parameters**

`lhs`, `rhs`, `res`	- basic_result object to compare

`ok` - a success object to compare

`err` - a failure object to compare

`value`	- value to compare to the contained value


**Return value**

1. If `lhs.is_ok() && rhs.is_ok()`, returns `lhs.unwrap() == rhs.unwrap()`.
  
    Otherwise, if `lhs.is_err() && rhs.is_err()`, returns `lhs.unwrap_err() == rhs.unwrap_err()`

    Otherwise, if returns `false`.

2. If `lhs.is_ok() && rhs.is_ok()`, returns `lhs.unwrap() != rhs.unwrap()`.
  
    Otherwise, if `lhs.is_err() && rhs.is_err()`, returns `lhs.unwrap_err() != rhs.unwrap_err()`

    Otherwise, if returns `true`.

3. If `lhs.is_ok() && rhs.is_ok()`, returns `lhs.unwrap() < rhs.unwrap()`.
  
    Otherwise, if `lhs.is_err() && rhs.is_err()`, returns `lhs.unwrap_err() < rhs.unwrap_err()`

    Otherwise, if `lhs.is_err() && rhs.is_ok()`, returns `true`

    Otherwise, returns `false`.

4. If `lhs.is_ok() && rhs.is_ok()`, returns `lhs.unwrap() <= rhs.unwrap()`.
  
    Otherwise, if `lhs.is_err() && rhs.is_err()`, returns `lhs.unwrap_err() <= rhs.unwrap_err()`

    Otherwise, if `lhs.is_err() && rhs.is_ok()`, returns `true`

    Otherwise, returns `false`.

5. If `lhs.is_ok() && rhs.is_ok()`, returns `lhs.unwrap() > rhs.unwrap()`.
  
    Otherwise, if `lhs.is_err() && rhs.is_err()`, returns `lhs.unwrap_err() > rhs.unwrap_err()`

    Otherwise, if `lhs.is_ok() && rhs.is_err()`, returns `true`

    Otherwise, returns `false`.

6. If `lhs.is_ok() && rhs.is_ok()`, returns `lhs.unwrap() >= rhs.unwrap()`.
  
    Otherwise, if `lhs.is_err() && rhs.is_err()`, returns `lhs.unwrap_err() >= rhs.unwrap_err()`

    Otherwise, if `lhs.is_ok() && rhs.is_err()`, returns `true`

    Otherwise, returns `false`.

7. Returns `res.is_ok() ? res.unwrap() == ok.value : false`.

8. Returns `res.is_ok() ? ok.value == res.unwrap() : false`.

9. Returns `res.is_ok() ? res.unwrap() != ok.value : true`.

10. Returns `res.is_ok() ? ok.value != res.unwrap() : true`.

11. Returns `res.is_ok() ? res.unwrap() < ok.value : true`.

12. Returns `res.is_ok() ? ok.value < res.unwrap() : false`.

13. Returns `res.is_ok() ? res.unwrap() <= ok.value : true`.

14. Returns `res.is_ok() ? ok.value <= res.unwrap() : false`.

15. Returns `res.is_ok() ? res.unwrap() > ok.value : false`.

16. Returns `res.is_ok() ? ok.value > res.unwrap() : true`.

17. Returns `res.is_ok() ? res.unwrap() >= ok.value : false`.

18. Returns `res.is_ok() ? ok.value >= res.unwrap() : true`.

19. Returns `res.is_err() ? res.unwrap_err() == err.value : false`.

20. Returns `res.is_err() ? err.value == res.unwrap_err() : false`.

21. Returns `res.is_err() ? res.unwrap_err() != err.value : true`.

22. Returns `res.is_err() ? err.value != res.unwrap_err() : true`.

23. Returns `res.is_err() ? res.unwrap_err() < err.value  : false`.

24. Returns `res.is_err() ? err.value < res.unwrap_err()  : true`.

25. Returns `res.is_err() ? res.unwrap_err() <= err.value : false`.

26. Returns `res.is_err() ? err.value <= res.unwrap_err() : true`.

27. Returns `res.is_err() ? res.unwrap_err() > err.value  : true`.

28. Returns `res.is_err() ? err.value > res.unwrap_err()  : false`.

29. Returns `res.is_err() ? res.unwrap_err() >= err.value : false`.

30. Returns `res.is_err() ? err.value >= res.unwrap_err() : true`.

31. Returns `res == success(value)`.

32. Returns `success(value) == res`.

33. Returns `res != success(value)`.

34. Returns `success(value) != res`.

35. Returns `res < success(value)`.

36. Returns `success(value) < res`.

37. Returns `res <= success(value)`.

38. Returns `success(value) <= res`.

39. Returns `res > success(value)`.

40. Returns `success(value) > res`.

41. Returns `res >= success(value)`.

42. Returns `success(value) >= res`.

**Note**

- `err.value`, `ok.value`: `value` is a private member of `success` or `failure` (actually, cannot access to it).
