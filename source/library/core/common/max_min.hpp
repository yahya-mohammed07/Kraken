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

#include <type_traits>
#include <initializer_list>

template <class Ty>
struct Min_Max {
  const Ty Min{};
  const Ty Max{};
};

namespace kraken::cal {

  /// @brief calculate `max` with two numbers
  /// @param  a  A thing of arbitrary type.
  /// @param  b  Another thing of arbitrary type.
  /// @return Ty
  template<class T>
  [[nodiscard]]
  inline constexpr
  auto max(const T a, const T b)
      -> T
  {
    if constexpr ( std::is_integral_v<T>) {
      return a ^ ((a ^ b) & -(a < b));
    } else
    { return a > b ? a : b; }
  }

  /// @brief calculate `min` with two numbers
  /// @param  a  A thing of arbitrary type.
  /// @param  b  Another thing of arbitrary type.
  /// @return Ty
  template<class T>
  [[nodiscard]]
  inline constexpr
  auto min(const T a, const T b)
      -> T
  {
    if constexpr ( std::is_integral_v<T> ) {
      return b ^ ((a ^ b) & -(a < b));
    } else
    { return a > b ? b : a; }
  }


  /// @brief calculates max in a init-list
  /// @return Ty
  template<class Ty>
  [[nodiscard]]
  inline constexpr
  auto max( std::initializer_list<Ty>&& args)
      -> Ty
  {
    Ty high {*args.begin()};
    for ( auto && i : args) {
      if ( i > high ) { high = i; }
    }
    return high;
  }

  /// @brief calculates max in a init-list
  /// @return Ty
  template<class Ty>
  [[nodiscard]]
  inline constexpr
  auto max(const std::initializer_list<Ty>& args)
      -> Ty
  {
    Ty high {*args.begin()};
    for (const auto & i : args) {
      if ( i > high ) { high = i; }
    }
    return high;
  }

  /// @brief calculate `max` with arbitrary numbers
  /// @param  a...  arbitrary number of arguments
  /// @return First
  template<class First, class Second, class ...Args>
  [[nodiscard]]
  inline constexpr
  auto max(First a, Second b,
            Args... args)
     -> First
  {
    if constexpr ( sizeof...(args) == 0 ) {
      return max(a, b);
    } else {
      return max(max(a,b), args...);
    }
  }

  /// @brief: finds max in a container
  template<class Cont>
  requires std::is_class_v<Cont>
  [[nodiscard]]
  inline constexpr
  auto max(const Cont &container)
  {
    auto high {container[0]};
    for (auto &i : container) {
      if ( i > high ) { high = i; }
    }
    return high;
  }

  /// @brief calculates min in a init-list
  /// @return Ty
  template<class Ty>
  [[nodiscard]]
  inline constexpr
  auto min( std::initializer_list<Ty>&& args)
      -> Ty
  {
    Ty low {*args.begin()};
    for ( auto && i : args) {
      if ( i < low ) { low = i; }
    }
    return low;
  }

  /// @brief calculates min in a init-list
  /// @return Ty
  template<class Ty>
  [[nodiscard]]
  inline constexpr
  auto min(const std::initializer_list<Ty>& args)
      -> Ty
  {
    Ty low {*args.begin()};
    for (const auto & i : args) {
      if ( i < low ) { low = i; }
    }
    return low;
  }

  /// @brief calculate `min` with arbitrary numbers
  /// @param  a...  arbitrary number of arguments
  /// @return First
  template<class First, class Second, class ...Args>
  [[nodiscard]]
  inline constexpr
  auto min(First a, Second b,
                Args... args)
     -> First
  {
    if constexpr ( sizeof...(args) == 0 ) {
      return min(a, b);
    } else {
      return min(min(a,b), args...);
    }
  }

  /// @brief: finds min in a container
  template<class Cont>
  requires std::is_class_v<Cont>
  [[nodiscard]]
  inline constexpr
  auto min(const Cont &container)
  {
    auto low {container[0]};
    for (auto &i : container) {
      if ( i < low ) { low = i; }
    }
    return low;
  }

  /// @brief calculate `min_max` with two numbers
  /// @param  a  A thing of arbitrary type.
  /// @param  b  Another thing of arbitrary type.
  /// @return Min_Max<Ty>
  template<class Ty>
  requires std::is_floating_point_v<Ty> || std::is_integral_v<Ty>
  [[nodiscard]]
  inline constexpr
  auto min_max(const Ty a, const Ty b)
      -> Min_Max<Ty>
  {
    return { min(a, b), max( a, b) };
  }


  /// @brief calculates `min_max` in a init-list
  /// @return Min_Max<Ty>
  template<class Ty>
  [[nodiscard]]
  inline constexpr
  auto min_max(const std::initializer_list<Ty>& args)
      -> Min_Max<Ty>
  {
    Ty low {*args.begin()};
    Ty high {*args.begin()};
    for (const auto & i : args) {
      if ( i < low ) { low = i; }
      if ( i > high ) { high = i; }
    }
    return {low, high};
  }

  /// @brief calculates `min_max` in a init-list
  /// @return Min_Max<Ty>
  template<class Ty>
  [[nodiscard]]
  inline constexpr
  auto min_max(std::initializer_list<Ty> &&args)
      -> Min_Max<Ty>
  {
    Ty low {*args.begin()};
    Ty high {*args.begin()};
    for ( auto &&i : args) {
      if ( i < low ) { low = i; }
      if ( i > high ) { high = i; }
    }
    return {low, high};
  }

  /// @brief calculate `min_max` with arbitrary numbers
  /// @param  a...  A thing of arbitrary number of arguments
  /// @return Min_Max<Ty>
  template<class First, class Second, class ...Args>
  [[nodiscard]]
  inline constexpr
  auto min_max(First a, Second b,
                Args ...args)
     -> Min_Max<First>
  {
    return { min({a,b,args...}) , max({a,b,args...}) };
  }

  /// @brief: `finds min_max` in a container
  /// @return Min_Max<decltype(container[0])>
  template<class Cont>
  requires std::is_class_v<Cont>
  [[nodiscard]]
  inline constexpr
  auto min_max(const Cont &container)
    -> Min_Max<decltype(container[0])>
  {
    auto low {container[0]};
    auto high {container[0]};
    for (auto &&i : container) {
      if ( i < low ) { low = i; }
      if ( i > high ) { high = i; }
    }
    return {low, high};
  }

} // namespace kraken::cal

#endif // MAX_MIN_HPP
