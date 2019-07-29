**maybe&lt;T&gt;::or_finally(F f) -> void**
**where F: T -> void**

```cpp
template <class T>
class maybe {
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
};
```

Invokes the provided function (if nothing), or doing nothing (if any).

**Example**

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
using namespace mitama;

int main() {
  maybe x = nothing;
  int hook = 0;
  assert(x.or_peek([&hook]{ hook = 42; }) == nothing);
  assert(hook == 42);
}
// end example
```
