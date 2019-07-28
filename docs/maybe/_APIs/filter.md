**filter(Pred predicate) -> maybe&lt;T&gt;**

```cpp
template <class Pred>
std::enable_if_t<
    std::is_invocable_r_v<bool, Pred&&, T&>,
maybe<T>>
maybe<T>::filter(Pred&& predicate) & ;

template <class Pred>
std::enable_if_t<
    std::is_invocable_r_v<bool, Pred&&, T const&>,
maybe<T>>
maybe<T>::filter(Pred&& predicate) const& ;

template <class Pred>
std::enable_if_t<
    std::is_invocable_r_v<bool, Pred&&, T&&>,
maybe<T>>
maybe<T>::filter(Pred&& predicate) && ;
```

Returns `nothing` if the option is `nothing`, otherwise calls predicate with the wrapped value and returns:

- `just(t)` if predicate returns true (where t is the wrapped value), and
- `nothing` if predicate returns false.

**Example**

```cpp
auto is_even = [](int n) -> bool { return n % 2 == 0; };

assert(maybe<int>{}.filter(is_even) == nothing);
assert(maybe{just(3)}.filter(is_even) == nothing);
assert(maybe{just(4)}.filter(is_even) == just(4));
```

