**maybe&lt;T&gt;::replace(Args... args) -> maybe&lt;T&gt;**
**where T is constructible from (Args...)**

```cpp
template <class T>
class maybe {
  maybe<T> replace(Args&&... args) & ;
};
```

Replaces the actual value in the maybe by expression `std::forward<Args>(args)...`, returning the old value if present, leaving a `just` value in its place without deinitializing either one.

**Example**

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
using namespace mitama;

int main() {
  {
    maybe x = just(2);
    auto old = x.replace(5);
    assert(x == just(5));
    assert(old == just(2));
  }
  {
    maybe<int> x = nothing;
    auto old = x.replace(3);
    assert(x == just(3));
    assert(old == nothing);
  }
}
// end example
```
