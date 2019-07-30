## is_just

```cpp
template <class>
struct is_just : std::false_type
{
};
template <class T>
struct is_just<just_t<T>> : std::true_type
{
};
template <class T>
inline constexpr bool is_just_v = is_just<T>::value;
```

