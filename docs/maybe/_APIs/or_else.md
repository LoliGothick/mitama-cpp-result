**or_else(F f) -> maybe&lt;T&gt;**

```cpp
template <class F>
std::enable_if_t<
    std::conjunction_v<
        std::is_invocable<F&&>,
        is_maybe_with<std::decay_t<std::invoke_result_t<F&&>>, T>>,
maybe<T>>
maybe<T>::or_else(F&& f) & ;

template <class F>
std::enable_if_t<
    std::conjunction_v<
        std::is_invocable<F&&>,
        is_maybe_with<std::decay_t<std::invoke_result_t<F&&>>, T>>,
maybe<T>>
maybe<T>::or_else(F&& f) const& ;

template <class F>
std::enable_if_t<
    std::conjunction_v<
        std::is_invocable<F&&>,
        is_maybe_with<std::decay_t<std::invoke_result_t<F&&>>, T>>,
maybe<T>>
maybe<T>::or_else(F&& f) && ;
```

Returns the maybe if it contains a value, otherwise calls `f` and returns the result.

**Example**

```cpp
auto nobody = []() -> maybe<std::string> { return nothing; };
auto vikings = []() -> maybe<std::string> { return just("vikings"s); };

assert(maybe{just("barbarians"s)}.or_else(vikings) == just("barbarians"s));
assert(maybe<std::string>{}.or_else(vikings) == just("vikings"s));
assert(maybe<std::string>{}.or_else(nobody) == nothing);
```