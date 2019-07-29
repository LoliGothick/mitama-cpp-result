**maybe&lt;T&gt;::map_or_else(D def, F f) -> U**
**where**
**D: () -> U,**
**F: T -> U,**

```cpp
template <class T>
class maybe {
  template <class D, class F>
  std::enable_if_t<
    std::conjunction_v<
      std::is_invocable<D&&>,
      std::is_invocable<F&&, T&>,
      meta::has_type<std::common_type<std::invoke_result_t<D&&>, std::invoke_result_t<F&&, T&>>>>,
  std::common_type_t<std::invoke_result_t<D&&>, std::invoke_result_t<F&&, T&>>>
  map_or_else(D&& def, F&& f) & ;

  template <class D, class F>
  std::enable_if_t<
    std::conjunction_v<
      std::is_invocable<D&&>,
      std::is_invocable<F&&, T const&>,
      meta::has_type<std::common_type<std::invoke_result_t<D&&>, std::invoke_result_t<F&&, T const&>>>>,
  std::common_type_t<std::invoke_result_t<D&&>, std::invoke_result_t<F&&, T const&>>>
  map_or_else(D&& def, F&& f) const& ;

  template <class D, class F>
  std::enable_if_t<
    std::conjunction_v<
      std::is_invocable<D&&>,
      std::is_invocable<F&&, T&&>,
      meta::has_type<std::common_type<std::invoke_result_t<D&&>, std::invoke_result_t<F&&, T&&>>>>,
  std::common_type_t<std::invoke_result_t<D&&>, std::invoke_result_t<F&&, T&&>>>
  map_or_else(D&& def, F&& f) && ;
};
```

Applies a function to the contained value (if any), or computes a default (if not).

**Examples**

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  int k = 21;
  maybe x = just("foo"s);
  assert(x.map_or_else([k]{ return 2 * k; }, &std::string::size) == 3);
  maybe<std::string> y = nothing;
  assert(y.map_or_else([k]{ return 2 * k; }, &std::string::size) == 42);
}
// end example
```
