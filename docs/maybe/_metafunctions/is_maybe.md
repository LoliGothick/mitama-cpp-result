## is_maybe

```cpp
template <class>
struct is_maybe : std::false_type
{
};
template <class T>
struct is_maybe<maybe<T>> : std::true_type
{
};
template <class T>
inline constexpr bool is_maybe_v = is_maybe<T>::value;
```

