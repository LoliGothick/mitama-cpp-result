**maybe&lt;T&gt;::disj(maybe&lt;T&gt;) -> maybe&gt;T&gt;**

```cpp
template <class T>
class maybe {
  auto maybe<T>::disj(maybe<T> const& rhs) -> maybe<T> ;
};
```

Returns the maybe if it contains a value, otherwise returns rhs.

Arguments passed to or are eagerly evaluated; if you are passing the result of a function call, it is recommended to use or_else, which is lazily evaluated.

**Example**

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
using namespace mitama;

int main() {
  {
    maybe x = just(2);
    maybe<int> y = nothing;
    assert(x.disj(y) == just(2));
  }
  {
    maybe<int> x = nothing;
    maybe y = just(100);
    assert(x.disj(y) == just(100));
  }
  {
    maybe x = just(2);
    maybe y = just(100);
    assert(x.disj(y) == just(2));
  }
  {
    maybe<int> x = nothing;
    maybe<int> y = nothing;
    assert(x.disj(y) == nothing);
  }
}
// end example
```
