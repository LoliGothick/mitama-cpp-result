**maybe&lt;T&gt;::unwrap() -> T&**

```cpp
template <class T>
class maybe {
  auto maybe<T>::unwrap() & -> T& ;

  auto maybe<T>::unwrap() const& -> T const& ;

  auto maybe<T>::unwrap() && -> T&& ;
};
```

Unwraps a maybe, yielding the content of an `just`.

**Exception**

Raise `mitama::runtime_panic` if a maybe has not `just` value.

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
    maybe x = just("air"s);
    assert(x.unwrap() == "air"s);
  }
  try {
    maybe<int> x = nothing;
    x.unwrap(); // raise an exception
  }
  catch ( mitama::runtime_panic const& panic ) {
    std::cerr << panic.what() << std::endl; // runtime panicked at 'called `maybe::unwrap()` on a `nothing` value'
  }
}
// end example
```
