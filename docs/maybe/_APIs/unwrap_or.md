**unwrap_or(U u) -> common_type&lt;T, U&gt;**


```cpp
template <class U>
std::enable_if_t<
    meta::has_type<std::common_type<T&, U&&>>::value,
std::common_type_t<T&, U&&>>
maybe<T>::unwrap_or(U&& def) & ;

template <class U>
std::enable_if_t<
    meta::has_type<std::common_type<T const&, U&&>>::value,
std::common_type_t<T const&, U&&>>
maybe<T>::unwrap_or(U&& def) const& ;

template <class U>
std::enable_if_t<
    meta::has_type<std::common_type<T&&, U&&>>::value,
std::common_type_t<T&&, U&&>>
maybe<T>::unwrap_or(U&& def) && ;
```

Returns the contained value or a default.

Arguments passed to `unwrap_or` are eagerly evaluated; if you are passing the result of a function call, it is recommended to use `unwrap_or_else`, which is lazily evaluated.

**Example**

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  assert(maybe{just("car"s)}.unwrap_or("bike"s) == "car"s);
  assert(maybe<std::string>{nothing}.unwrap_or("bike"s) == "bike"s);
}
// end example
```

