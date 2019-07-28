**and_finally(F f) -> void**


```cpp
template <class F>
constexpr
std::enable_if_t<std::is_invocable_v<F&&, T&>>
and_finally(F&& f) & ;

template <class F>
constexpr
std::enable_if_t<std::is_invocable_v<F&&, T const&>>
and_finally(F&& f) const& ;

template <class F>
constexpr
std::enable_if_t<std::is_invocable_v<F&&, T&&>>
and_finally(F&& f) const& ;
```

Invokes the provided function with the contained success value (if success), or doing nothing (if failure).

**Example**

```cpp
maybe x = just(42);
int hook = 0;
assert(x.and_peek([&hook](int const& v){ hook = v; }) == just(42));
assert(hook == 42);
```
