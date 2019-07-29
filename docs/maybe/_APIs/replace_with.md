**maybe&lt;T&gt;::replace_with(F f, Args... args) -> maybe&lt;T&gt;**
**where T is constructible from F(Args...)**

```cpp
template <class T>
class maybe {
  template <class F, class... Args>
  maybe<T> replace_with(F&& f, Args&&... args) & ;
};
```

Replaces the actual value in the maybe by expression `std::invoke(std::forward<F>(f), std::forward<Args>(args)...)`, returning the old value if present, leaving a `just` value in its place without deinitializing either one.

**Example**

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  {
    maybe x = just(2);
    auto old = x.replace_with([]{ return 5; });
    assert(x == just(5));
    assert(old == just(2));
  }
  {
    maybe<int> x = nothing;
    auto old = x.replace_with(&std::string::size, "foo"s);
    assert(x == just(3));
    assert(old == nothing);
  }
}
// end example
```
