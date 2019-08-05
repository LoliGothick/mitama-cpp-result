**maybe&lt;T&gt;::is_just() -> bool**

```cpp
template <class T>
class maybe {
  constexpr bool maybe<T>::is_just() const noexcept ;
};
```

Returns `true` if the maybe has some value.

**Example**

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
using namespace mitama;

int main() {
  maybe<int> x = just(2);
  assert( x.is_just() );

  maybe<int> y = nothing;
  assert( ! y.is_just() );
}
// end example
```
