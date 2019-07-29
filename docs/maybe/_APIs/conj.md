**maybe&lt;T&gt;::conj(maybe&lt;U&gt;) -> maybe&gt;U&gt;**

```cpp
template <class T>
class maybe {
  template <class U>
  auto maybe<T>::conj(maybe<U> const& rhs) -> maybe<U> ;
};
```

Returns `nothing` if the `lhs` is `nothing`, otherwise returns `rhs`.

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
    maybe<std::string> y = nothing;
    assert(x.conj(y) == nothing);
  }
  {
    maybe<int> x = nothing;
    maybe y = just("foo"s);
    assert(x.conj(y) == nothing);
  }
  {
    maybe x = just(2);
    maybe y = just("foo"s);
    assert(x.conj(y) == just("foo"s));
  }
  {
    maybe<int> x = nothing;
    maybe<std::string> y = nothing;
    assert(x.conj(y) == nothing);
  }
}
// end example
```
