**maybe&lt;T&gt;::unwrap_or_default() -> T**

```cpp
template <class T>
class maybe {
  auto maybe<T>::unwrap_or_default() const & -> std::remove_reference_t<T> ;
};
```

Returns the contained value or a default.

If `just`, returns the contained value, otherwise if `nothing`, returns the default value for that type.

**Remarks**

This operator shall be defined as deleted unless `is_default_constructible_v<T>` is true.

**Example**

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  maybe<std::string> x = nothing;
  assert(x.unwrap_or_default() == ""s);
}
// end example
```

