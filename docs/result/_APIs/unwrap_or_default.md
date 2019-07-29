**basic_result&lt;_, T, E&gt;::unwrap_or_default() -> T**
**where T is default constructible**

```cpp
template <mutability _mu, class T, class E>
class basic_result {
  auto basic_result<_, T, E>::unwrap_or_default() const &
    -> T ;
};
```

Returns the contained value or a default.

If `success`, returns the contained value, otherwise if `failure`, returns the default value for that type.

**Remarks**

This operator shall be defined as deleted unless `is_default_constructible_v<T>` is true.

**Example**

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  result<int> good = success(1909);
  result<int> bad = failure<>();
  auto good_year = good.unwrap_or_default();
  auto bad_year = bad.unwrap_or_default();
  assert(1909 == good_year);
  assert(0 == bad_year);
}
// end example
```
