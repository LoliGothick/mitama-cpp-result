**operator==**

```cpp
namespace mitama {
  template <class T, class U>
  constexpr bool operator==(const maybe<T>& lhs, const maybe<U>& rhs); // (1)

  template <class T, class U>
  constexpr bool operator==(const maybe<T>& lhs, const just_t<U>& rhs); // (2)
  template <class T, class U>
  constexpr bool operator==(const just_t<T>& lhs, const maybe<U>& rhs); // (3)

  template <class T>
  constexpr bool operator==(const maybe<T>& lhs, nothing_t) noexcept;   // (4)
  template <class T>
  constexpr bool operator==(nothing_t, const maybe<T>& rhs) noexcept;   // (5)

  template <class T, class U>
  constexpr bool operator==(const maybe<T>& lhs, const U& rhs);           // (6)
  template <class T, class U>
  constexpr bool operator==(const U& lhs, const maybe<T>& rhs);           // (7)
}
```

- 1-7) Compares two maybe objects, lhs and rhs. The contained values are compared (using the corresponding operator of T and U) only if both lhs and rhs contain values. Otherwise,
  - lhs is considered equal to rhs if, and only if, both lhs and rhs do not contain a value.
  - lhs is considered less than rhs if, and only if, rhs contains a value and lhs does not.
