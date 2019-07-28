**and_peek(F f) -> maybe&lt;T&gt;**


```cpp
template <class F>
constexpr
std::enable_if_t<
    std::disjunction_v<
        std::is_invocable<F, T&>,
        std::is_invocable<F>>,
basic_result&>
and_peek(F&& f) & ;

template <class F>
std::enable_if_t<
    std::disjunction_v<
        std::is_invocable<F, T const&>,
        std::is_invocable<F>>,
basic_result const&>
and_peek(F&& f) const& ;

template <class F>
std::enable_if_t<
    std::disjunction_v<
        std::is_invocable<F, T&&>,
        std::is_invocable<F>>,
basic_result&&>
and_peek(F&& f) && ;
```

Peeks the contained success value and then returns self.

Invokes the provided function with the contained value and then return self (if success), or return self without doing anything (if failure).

**Example**

```cpp
maybe x = just(42);
int hook = 0;
assert(x.and_peek([&hook](int const& v){ hook = v; }) == just(42));
assert(hook == 42);
```
