**unwrap_or_else()**

```cpp
template <class F>
std::enable_if_t<
    std::conjunction_v<
        std::is_invocable<F&&>,
        meta::has_type<std::common_type<T&, std::invoke_result_t<F&&>>>>,
std::common_type_t<T&, std::invoke_result_t<F&&>>>
maybe<T>::unwrap_or_else(F&& f) & ;

template <class F>
std::enable_if_t<
    std::conjunction_v<
        std::is_invocable<F&&>,
        meta::has_type<std::common_type<T const&, std::invoke_result_t<F&&>>>>,
std::common_type_t<T const&, std::invoke_result_t<F&&>>>
maybe<T>::unwrap_or_else(F&& f) const& ;

template <class F>
std::enable_if_t<
    std::conjunction_v<
        std::is_invocable<F&&>,
        meta::has_type<std::common_type<T&&, std::invoke_result_t<F&&>>>>,
std::common_type_t<T&&, std::invoke_result_t<F&&>>>
maybe<T>::unwrap_or_else(F&& f) && ;
```

Returns the contained value or computes it from a invocable object `op`.

**Example**

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
using namespace mitama;

int main() {
  int k = 10;
  assert(maybe{just(4)}.unwrap_or_else([k]{ return 2 * k; }) == 4);
  assert(maybe<int>{}.unwrap_or_else([k]{ return 2 * k; }) == 20);
}
// end example
```
