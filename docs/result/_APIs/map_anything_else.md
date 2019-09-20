**basic_result&lt;_, T, E&gt;::map_anything_else(Map _map) -> U**  
**where**  
**Map: T -> U,**  

__[since v7.8.0]__

```cpp
template <mutability _mu, class T, class E>
class basic_result {
  template <class Map>
  constexpr auto map_or_else(Map&& _map) const&
    -> std::common_type_t<std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>> ;
};
```

Maps a `result<T, E>` to `U` by applying a function `_map` to a contained either `success` or `failure` value.

This function is syntax sugar for `res.map_or_else(_map, _map)`.

**Examples**

Basic usage:

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  {
    result<std::string, std::string> x = success("foo"s);
    assert(x.map_anything_else([](auto v) { return v.length(); }) == 3);
  }
  {
    result<std::string, std::string> x = failure("bar"s);
    assert(x.map_anything_else([](auto v) { return v.length(); }) == 3);
  }
}
// end example
```
