**maybe&lt;T&gt;::or_else(F f) -> maybe&lt;T&gt;**
**where F: () -> maybe&lt;T&gt;**

```cpp
template <class T>
class maybe {
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
};
```

Returns the maybe if it contains a value, otherwise calls `f` and returns the result.

**Example**

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  auto nobody = []() -> maybe<std::string> { return nothing; };
  auto vikings = []() -> maybe<std::string> { return just("vikings"s); };

  assert(maybe{just("barbarians"s)}.or_else(vikings) == just("barbarians"s));
  assert(maybe<std::string>{}.or_else(vikings) == just("vikings"s));
  assert(maybe<std::string>{}.or_else(nobody) == nothing);
}
// end example
```
