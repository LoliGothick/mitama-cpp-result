## is_result_with

```cpp
template <class, class...>
struct is_result_with : std::false_type
{
};
template <class T, class E>
struct is_result_with<result<T, E>, success<T>> : std::true_type
{
};
template <class T, class E>
struct is_result_with<result<T, E>, failure<E>> : std::true_type
{
};
template <class T, class E>
struct is_result_with<result<T, E>, success<T>, failure<E>> : std::true_type
{
};
template <class T, class... Requires>
inline constexpr bool is_result_with_v = is_result_with<T, Requires...>::value;
```

