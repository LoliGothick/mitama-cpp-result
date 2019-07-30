## is_result

```cpp
template <class>
struct is_result : std::false_type
{
};
template <class T, class E>
struct is_result<result<T, E>> : std::true_type
{
};
template <class T>
inline constexpr bool is_result_v = is_result<T>::value;
```

