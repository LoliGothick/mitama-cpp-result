**maybe&lt;T&gt;::and_then(F f) -> maybe&lt;U&gt;**
**where F: T -> maybe&lt;U&gt;**

```cpp
template <class T>
class maybe {
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
};
```

Returns `nothing` if the option is `nothing`, otherwise calls `f` with the wrapped value and returns the result.

Some languages call this operation flatmap.

**Example**

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
using namespace mitama;

int main() {
    auto sq = [](int x) -> maybe<int> { return just(x * x); };
    auto nope = [](...) -> maybe<int> { return nothing; };

    assert(maybe{just(2)}.and_then(sq).and_then(sq) == just(16));
    assert(maybe{just(2)}.and_then(sq).and_then(nope) == nothing);
    assert(maybe{just(2)}.and_then(nope).and_then(sq) == nothing);
    assert(nope().and_then(sq).and_then(sq) == nothing);
}
// end example
```
