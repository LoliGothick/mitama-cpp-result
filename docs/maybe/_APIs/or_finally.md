**or_finally(F f) -> void**


```cpp
template <class F>
constexpr
std::enable_if_t<
    std::is_invocable_v<F>>
or_finally(F&& f) & ;

template <class F>
constexpr
std::enable_if_t<
    std::is_invocable_v<F>>
or_finally(F&& f) const & ;

template <class F>
constexpr
std::enable_if_t<
    std::is_invocable_v<F>>
or_finally(F&& f) && ;
```

Invokes the provided function (if nothing), or doing nothing (if any).

**Example**

```cpp
maybe x = nothing;
int hook = 0;
assert(x.or_peek([&hook]{ hook = 42; }) == nothing);
assert(hook == 42);
```
