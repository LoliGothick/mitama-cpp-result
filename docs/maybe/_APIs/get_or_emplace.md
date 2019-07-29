**maybe&lt;T&gt;::get_or_emplace(Args... args) -> T&**

```cpp
template <class T>
class maybe {
  template <class... Args>
  std::enable_if_t<
    std::is_constructible_v<T, Args&&...>,
  T&>
  get_or_emplace(Args&&... args) & ;
};
```

Emplace constructs `T` into the maybe with expression `std::forward<Args>(args)...` if it is `nothing`, then returns a mutable reference to the contained value.

**Example**

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

struct noncopyable {
  noncopyable() = default;
  noncopyable(noncopyable const&) = delete;
  noncopyable& operator=(noncopyable const&) = delete;
  noncopyable(noncopyable&&) = default;
  noncopyable& operator=(noncopyable&&) = default;

  bool operator==(noncopyable&&) & { return true; }
  bool operator==(noncopyable&&) const& { return true; }
  bool operator==(noncopyable const&) & { return true; }
  bool operator==(noncopyable const&) const& { return true; }
};

int main() {
  maybe<noncopyable> x = nothing;
  auto& y = x.get_or_emplace(noncopyable{});
  assert(y == noncopyable{});
}
// end example
```
