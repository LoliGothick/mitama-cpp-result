**as_ref()**

```cpp
auto maybe<T>::as_ref() & -> maybe<T&> ;

auto maybe<T>::as_ref() const& -> maybe<const T&> ;
```

Converts from `maybe<T>&` to `maybe<T&>`.

**Example**

```cpp
maybe text = just("Hello, world!"s);
// First, cast `maybe<T>` to `maybe<T&>` with `as_ref`,
auto text_length = text.as_ref().map(&std::string::size);
std::cout << "still can print text: " << text << "\n";
```

