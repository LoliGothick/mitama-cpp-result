**and_finally(F f) -> void**


```cpp
template <class F>
constexpr
std::enable_if_t<
    std::disjunction_v<
        std::is_invocable<F, T&>,
        std::is_invocable<F>>>
and_finally(F&& f) & ;

template <class F>
std::enable_if_t<
    std::disjunction_v<
        std::is_invocable<F, T const&>,
        std::is_invocable<F>>>
and_finally(F&& f) const& ;

template <class F>
std::enable_if_t<
    std::disjunction_v<
        std::is_invocable<F, T&&>,
        std::is_invocable<F>>>
and_finally(F&& f) && ;
```

Invokes the provided function with the contained value (if any), or doing nothing (if not).

**Example**

```cpp
maybe x = just(42);
int hook = 0;
assert(x.and_peek([&hook](int const& v){ hook = v; }) == just(42));
assert(hook == 42);
```
