**basic_result::and_finally(F f) -> void**

```cpp
template <mutability _mu, class T, class E>
class basic_result {
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
};
```

Invokes the provided function with the contained success value (if success), or doing nothing (if failure).

**Example**

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
using namespace mitama;

int main() {
  result<int, std::string> x = success(42);
  int hook = 0;
  x.and_finally([&](int const& v){ hook = v; });
  assert(hook == 42);
}
// end example
```
