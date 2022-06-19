#ifndef MAX_MIN_HPP
#define MAX_MIN_HPP

/*

MIT License

Copyright (c) 2021 yahya mohammed

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include <cstdint>
#include <functional>
#include <initializer_list>
#include <type_traits>

template <typename It> class My_Iota {
private:
  It m_begin{};
  It m_end{};

public:
  constexpr auto begin() const -> It { return m_begin; }
  constexpr auto end() const -> It { return m_end; }
  constexpr My_Iota(const It a, const It b) : m_begin(a), m_end(b) {}
};

template <class Ty> struct Min_Max {
  const Ty Min{};
  const Ty Max{};
};

namespace kraken::cal {

template <class Forward_iter, class Comp>
[[nodiscard]] constexpr auto
range_template(Forward_iter first, const Forward_iter last, Comp comp) noexcept
    -> Forward_iter {
  if (first == last) {
    return last;
  }
  Forward_iter res{first};
  for (; ++first != last;) {
    if (comp(*first, *res)) {
      res = first;
    }
  }
  return res;
}

/// @brief Finds the greatest element in the range [first, last).
/// @param first
/// @param last
/// @param comp comparison function object
/// @return Iterator to the greatest element in the range [first, last).
/// If several elements in the range are equivalent to the greatest element,
/// returns the iterator to the first such element.
/// Returns last if the range is empty.
template <class Forward_iter, class Comp>
[[nodiscard]] constexpr auto max_range(Forward_iter first,
                                       const Forward_iter last, Comp comp)
    -> Forward_iter {
  return range_template(first, last, comp);
}
/// @brief Finds the greatest element in the range [first, last).
/// @param first
/// @param last
/// @return Iterator to the greatest element in the range [first, last).
/// If several elements in the range are equivalent to the greatest element,
/// returns the iterator to the first such element.
/// Returns last if the range is empty.
template <class Forward_iter>
[[nodiscard]] constexpr auto max_range(Forward_iter first,
                                       const Forward_iter last)
    -> Forward_iter {
  return max_range(first, last, std::greater<>{});
}

/// @brief Finds the minimum element in the range [first, last).
/// @param first
/// @param last
/// @param comp comparison function object
/// @return Iterator to the greatest element in the range [first, last).
/// If several elements in the range are equivalent to the greatest element,
/// returns the iterator to the first such element.
/// Returns last if the range is empty.
template <class Forward_iter, class Comp>
[[nodiscard]] constexpr auto min_range(Forward_iter first,
                                       const Forward_iter last, Comp comp)
    -> Forward_iter {
  return range_template(first, last, comp);
}
/// @brief Finds the minimum element in the range [first, last).
/// @param first
/// @param last
/// @return Iterator to the greatest element in the range [first, last).
/// If several elements in the range are equivalent to the greatest element,
/// returns the iterator to the first such element.
/// Returns last if the range is empty.
template <class Forward_iter>
[[nodiscard]] constexpr auto min_range(Forward_iter first,
                                       const Forward_iter last)
    -> Forward_iter {
  return min_range(first, last, std::less<>{});
}

/// @brief calculate `max` with two numbers
/// @param  a  A thing of arbitrary type.
/// @param  b  Another thing of arbitrary type.
/// @return Ty
template <class T> [[nodiscard]] constexpr auto max(const T a, const T b) -> T {
  return a > b ? a : b;
}

/// @brief calculate `max` with two numbers
/// @param  a  A thing of arbitrary type.
/// @param  b  Another thing of arbitrary type.
/// @return Ty
/// @note special case
template <>
[[nodiscard]] constexpr inline auto max<std::int32_t>(std::int32_t a,
                                                      std::int32_t b)
    -> std::int32_t {
  return a ^ ((a ^ b) & -(a < b));
}

/// @brief calculate `min` with two numbers
/// @param  a  A thing of arbitrary type.
/// @param  b  Another thing of arbitrary type.
/// @return Ty
/// @note special case
template <class T> [[nodiscard]] constexpr auto min(const T a, const T b) -> T {
  return a > b ? b : a;
}

/// @brief calculate `min` with two numbers
/// @param  a  A thing of arbitrary type.
/// @param  b  Another thing of arbitrary type.
/// @return Ty
template <>
[[nodiscard]] constexpr inline auto min<std::int32_t>(std::int32_t a,
                                                      std::int32_t b)
    -> std::int32_t {
  return b ^ ((a ^ b) & -(a < b));
}

/// @brief calculates max in a init-list
/// @return Ty
template <class Ty>
[[nodiscard]] constexpr auto max(const std::initializer_list<Ty> &args) -> Ty {
  return *max_range(args.begin(), args.end());
}

/// @brief calculate `max` with arbitrary numbers
/// @param  a...  arbitrary number of arguments
/// @return First
template <class First, class Second, class... Args>
[[nodiscard]] constexpr auto max(First a, Second b, Args... args) -> First {
  if constexpr (sizeof...(args) == 0) {
    return max(a, b);
  } else {
    return max(max(a, b), args...);
  }
}

/// @brief: finds max in a container
template <class Cont>
requires std::is_class_v<Cont>
[[nodiscard]] constexpr auto max(const Cont &container) {
  return *max_range(container.begin(), container.end());
}

/// @brief calculates min in a init-list
/// @return Ty
template <class Ty>
[[nodiscard]] constexpr auto min(const std::initializer_list<Ty> &args) -> Ty {
  return *min_range(args.begin(), args.end());
}

/// @brief calculate `min` with arbitrary numbers
/// @param  a...  arbitrary number of arguments
/// @return First
template <class First, class Second, class... Args>
[[nodiscard]] constexpr auto min(First a, Second b, Args... args) -> First {
  if constexpr (sizeof...(args) == 0) {
    return min(a, b);
  } else {
    return min(min(a, b), args...);
  }
}

/// @brief: finds min in a container
template <class Cont>
requires std::is_class_v<Cont>
[[nodiscard]] constexpr auto min(const Cont &container) {
  return *min_range(container.begin(), container.end());
}

/// @brief calculate `min_max` with two numbers
/// @param  a  A thing of arbitrary type.
/// @param  b  Another thing of arbitrary type.
/// @return Min_Max<Ty>
template <class Ty>
requires std::is_floating_point_v<Ty> || std::is_integral_v<Ty>
[[nodiscard]] constexpr auto min_max(const Ty a, const Ty b) -> Min_Max<Ty> {
  return {min(a, b), max(a, b)};
}

/// @brief calculates `min_max` in a init-list
/// @return Min_Max<Ty>
template <class Ty>
[[nodiscard]] constexpr auto min_max(const std::initializer_list<Ty> &args)
    -> Min_Max<Ty> {
  return {*min_range(args.begin(), args.end()),
          *max_range(args.begin(), args.end())};
}

/// @brief calculate `min_max` with arbitrary numbers
/// @param  a...  A thing of arbitrary number of arguments
/// @return Min_Max<Ty>
template <class First, class Second, class... Args>
[[nodiscard]] constexpr auto min_max(First a, Second b, Args... args)
    -> Min_Max<First> {
  return {min({a, b, args...}), max({a, b, args...})};
}

/// @brief: `finds min_max` in a container
/// @return Min_Max<decltype(container[0])>
template <class Cont>
requires std::is_class_v<Cont>
[[nodiscard]] constexpr auto min_max(const Cont &container)
    -> Min_Max<decltype(container[0])> {
  return {*min_range(container.begin(), container.end()),
          *max_range(container.begin(), container.end())};
}

/// @brief: `finds min_max` in a range
/// @return Min_Max<decltype(first)>
template <class Forward_iter>
[[nodiscard]] constexpr auto min_max_range(Forward_iter first,
                                           Forward_iter last)
    -> Min_Max<decltype(first)> {
  return {*min_range(first, last), *max_range(first, last)};
}
} // namespace kraken::cal

#endif // MAX_MIN_HPP
