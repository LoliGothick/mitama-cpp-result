**range_to_maybe(Range range) -> maybe&lt;Range::Item&gt;**

In header `<mitama/maybe/range_to_maybe.hpp>`.

```cpp
template <class Range>
auto range_to_maybe(Range&& range) -> maybe<decltype(*begin(range))> ;
```

The `range_to_maybe` function returns `nothing` on an empty range or `just(v)` where `v` is the first element of the `range`.

**Example**

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <mitama/maybe/range_to_maybe.hpp>
#include <cassert>
using namespace mitama;

int main() {
  std::vector v{1,2,3};
  maybe x = range_to_maybe(v);
  assert(x == just(1));

  maybe y = range_to_maybe(std::vector<int>{});
  assert(y == nothing);

  int a[] = {1};
  maybe z = range_to_maybe(a);
  assert(z == 1);
}
// end example
```
