**or_finally(F f) -> void**


```cpp
template <class F>
constexpr
std::enable_if_t<std::is_invocable_v<F&&, E&>>
and_finally(F&& f) & ;

template <class F>
constexpr
std::enable_if_t<std::is_invocable_v<F&&, E const&>>
and_finally(F&& f) const& ;

template <class F>
constexpr
std::enable_if_t<std::is_invocable_v<F&&, E&&>>
and_finally(F&& f) const& ;
```

Invokes the provided function with contained failure value (if failure), or doing nothing (if success).

**Example**

```cpp
maybe x = nothing;
int hook = 0;
assert(x.or_peek([&hook]{ hook = 42; }) == nothing);
assert(hook == 42);
```
