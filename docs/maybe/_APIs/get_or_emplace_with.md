**get_or_emplace_with(F f,Args... args) -> T&**

```cpp
template <class F, class... Args>
std::enable_if_t<
    std::conjunction_v<
        std::is_invocable<F&&, Args&&...>,
        std::is_constructible<T, std::invoke_result_t<F&&, Args&&...>>>,
T&>
get_or_emplace_with(F&& f, Args&&... args) & ;
```

Emplace constructs `T` into the maybe with expression `std::invoke(std::forward<F>(f), std::forward<Args>(args)...)` if it is `nothing`, then returns a mutable reference to the contained value.

**Example**

```cpp
maybe<int> x = nothing;
std::ignore = x.get_or_emplace_with([]{ return 5; });
std::ignore = x.get_or_emplace_with([](auto x){ return x; }, 5);
auto& y = x.get_or_emplace_with(&std::string::size , "12345"s);
assert(y == 5);
y = 7;
assert(x == just(7));
```