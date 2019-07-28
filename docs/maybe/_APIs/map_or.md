**map_or(U def, F f) -> common_type&lt;U, std::invoke_result_t&lt;F, T&gt;&gt;**

```cpp
template <class U, class F>
std::enable_if_t<
    std::conjunction_v<
        std::is_invocable<F&&, T&>,
        meta::has_type<std::common_type<U&&, std::invoke_result_t<F&&, T&>>>>,
std::common_type_t<U&&, std::invoke_result_t<F&&, T&>>>
map_or(U&& def, F&& f) & ;

template <class U, class F>
std::enable_if_t<
    std::conjunction_v<
        std::is_invocable<F&&, T const&>,
        meta::has_type<std::common_type<U&&, std::invoke_result_t<F&&, T const&>>>>,
std::common_type_t<U&&, std::invoke_result_t<F&&, T const&>>>
map_or(U&& def, F&& f) const& ;

template <class U, class F>
std::enable_if_t<
    std::conjunction_v<
        std::is_invocable<F&&, T&&>,
        meta::has_type<std::common_type<U&&, std::invoke_result_t<F&&, T&&>>>>,
std::common_type_t<U&&, std::invoke_result_t<F&&, T&&>>>
map_or(U&& def, F&& f) && ;
```

Applies a function to the contained value (if any), or returns the provided default (if not).

**Example**

```cpp
maybe x = just("foo"s);
assert(x.map_or(42, &std::string::size) == 3);

maybe<std::string> y = nothing;
assert(y.map_or(42, &std::string::size) == 42);
```
