**or_peek(F f) -> maybe&lt;T&gt;**


```cpp
template <class F>
constexpr
std::enable_if_t<
    std::disjunction_v<
        std::is_invocable<F, T&>,
        std::is_invocable<F>>,
basic_result&>
or_peek(F&& f) & ;

template <class F>
std::enable_if_t<
    std::disjunction_v<
        std::is_invocable<F, T const&>,
        std::is_invocable<F>>,
basic_result const&>
or_peek(F&& f) const& ;

template <class F>
std::enable_if_t<
    std::disjunction_v<
        std::is_invocable<F, T&&>,
        std::is_invocable<F>>,
basic_result&&>
or_peek(F&& f) && ;
```

Peeks the contained failure value and then returns self.

Invokes the provided function and then return self (if failure), or return self without doing anything (if success).

**Example**

```cpp
maybe x = nothing;
int hook = 0;
assert(x.or_peek([&hook]{ hook = 42; }) == nothing);
assert(hook == 42);
```
