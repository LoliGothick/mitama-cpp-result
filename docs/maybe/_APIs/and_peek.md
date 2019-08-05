**maybe&lt;T&gt;::and_peek(F f) -> maybe&lt;T&gt;**
**where F: T -> void**

```cpp
template <class T>
class maybe {
  template <class F>
  constexpr
  std::enable_if_t<
    std::disjunction_v<
      std::is_invocable<F, T&>,
      std::is_invocable<F>>,
  maybe&>
  and_peek(F&& f) & ;

  template <class F>
  std::enable_if_t<
    std::disjunction_v<
      std::is_invocable<F, T const&>,
      std::is_invocable<F>>,
  maybe const&>
  and_peek(F&& f) const& ;

  template <class F>
  std::enable_if_t<
    std::disjunction_v<
      std::is_invocable<F, T&&>,
      std::is_invocable<F>>,
  maybe&&>
  and_peek(F&& f) && ;
};
```

Peeks the contained value if self is `just`, then returns self.

Invokes the provided function with the contained value and then returns self (if any), or returns self without doing anything (if not).

**Example**

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
using namespace mitama;

int main() {
    maybe x = just(42);
    int hook = 0;
    assert(x.and_peek([&](int const& v){ hook = v; }) == just(42));
    assert(hook == 42);
}
// end example
```
