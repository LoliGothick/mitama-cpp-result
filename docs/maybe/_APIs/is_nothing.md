**maybe&lt;T&gt;::is_nothing() -> bool**

```cpp
template <class T>
class maybe {
  constexpr bool maybe<T>::is_nothing() const noexcept ;
};
```

Returns `true` if the `maybe` is a `nothing` value.

**Example**

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
using namespace mitama;

int main() {
  maybe<int> x = just(2);
  assert( ! x.is_nothing() );

  maybe<int> y = nothing;
  assert( y.is_nothing() );
}
// end example
```
