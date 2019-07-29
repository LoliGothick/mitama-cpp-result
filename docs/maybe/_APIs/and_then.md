**and_then(F f) -> std::invoke_result_t&lt;F, T&gt;**


```cpp
template <class F>
std::enable_if_t<
    std::conjunction_v<
        std::is_invocable<F&&, T&>,
        is_maybe<std::decay_t<std::invoke_result_t<F&&, T&>>>>,
std::invoke_result_t<F&&, T&> >
and_then(F&& f) & ;

template <class F>
std::enable_if_t<
    std::conjunction_v<
        std::is_invocable<F&&, T const&>,
        is_maybe<std::decay_t<std::invoke_result_t<F&&, T const&>>>>,
std::invoke_result_t<F&&, T const&> >
and_then(F&& f) const& ;

template <class F>
std::enable_if_t<
    std::conjunction_v<
        std::is_invocable<F&&, T&>,
        is_maybe<std::decay_t<std::invoke_result_t<F&&, T&&>>>>,
std::invoke_result_t<F&&, T&> >
and_then(F&& f) && ;
```

Returns `nothing` if the option is `nothing`, otherwise calls `f` with the wrapped value and returns the result.

Some languages call this operation flatmap.

**Example**

```cpp
auto sq = [](int x) -> maybe<int> { return just(x * x); };
auto nope = [](...) -> maybe<int> { return nothing; };

assert(maybe{just(2)}.and_then(sq).and_then(sq) == just(16));
assert(maybe{just(2)}.and_then(sq).and_then(nope) == nothing);
assert(maybe{just(2)}.and_then(nope).and_then(sq) == nothing);
assert(nope().and_then(sq).and_then(sq) == nothing);
```