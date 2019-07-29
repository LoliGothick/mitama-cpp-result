**maybe&lt;T&gt;::as_ref() -> maybe&lt;T&&gt;**

```cpp
template <class T>
class maybe {
  auto maybe<T>::as_ref() & -> maybe<T&> ;

  auto maybe<T>::as_ref() const& -> maybe<const T&> ;
};
```

Converts from `maybe<T>&` to `maybe<T&>`.

**Example**

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
#include <string>
#include <iostream>
using namespace mitama;
using namespace std::string_literals;

int main() {
    maybe text = just("Hello, world!"s);
    // First, cast `maybe<T>` to `maybe<T&>` with `as_ref`,
    auto text_length = text.as_ref().map(&std::string::size);
    std::cout << "still can print text: " << text << "\n";
}
// end example
```
