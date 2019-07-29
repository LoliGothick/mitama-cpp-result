**or_finally(F f) -> void**


```cpp
template <mutability _mu, class T, class E>
class basic_result {
  template <class F>
  constexpr
  std::enable_if_t<std::is_invocable_v<F&&, E&>>
  and_finally(F&& f) & ;

  template <class F>
  constexpr
  std::enable_if_t<std::is_invocable_v<F&&, E const&>>
  and_finally(F&& f) const& ;

  template <class F>
  constexpr
  std::enable_if_t<std::is_invocable_v<F&&, E&&>>
  and_finally(F&& f) const& ;
};
```

Invokes the provided function with contained failure value (if failure), or doing nothing (if success).

**Example**

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
using namespace mitama;

int main() {
  maybe x = nothing;
  int hook = 0;
  assert(x.or_peek([&]{ hook = 42; }) == nothing);
  assert(hook == 42);
}
// end example
```
