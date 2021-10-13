#ifndef FLOAT_COMP_HPP
#define FLOAT_COMP_HPP

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

#include "abs.hpp"
#include "max_min.hpp"
#include <limits>

namespace kraken::cal {

  /// @brief checks if two floating-point numbers are equal
  template <class Ty>
  requires std::is_floating_point_v<Ty>
  [[nodiscard]] inline constexpr
  auto equal(Ty a, Ty b,
        const Ty eps = std::numeric_limits<Ty>::epsilon())
    -> bool
  {
    return abs(a - b) <= ( min( abs(a),abs(b) ) * eps );
  }

  /// @brief checks if two floating-point numbers are approximately equal
  template <class Ty>
  requires std::is_floating_point_v<Ty>
  [[nodiscard]] inline constexpr
  auto approx_equal(Ty a, Ty b,
              const Ty eps = std::numeric_limits<Ty>::epsilon())
    -> bool
  {
    return abs(a - b) <= ( max( abs(a),abs(b) ) * eps );
  }

  /// @brief checks if `A` is greater than `B`
  template <class Ty>
  requires std::is_floating_point_v<Ty>
  [[nodiscard]] inline constexpr
  auto greater_than(Ty a, Ty b,
      const Ty eps = std::numeric_limits<Ty>::epsilon())
    -> bool
  {
    return (a - b) > ( max( abs(a),abs(b) ) * eps );
  }

  /// @brief checks if `A` is greater than `B` or equal
  template <class Ty>
  requires std::is_floating_point_v<Ty>
  [[nodiscard]] inline constexpr
  auto greater_or_equal(Ty a, Ty b,
      const Ty eps = std::numeric_limits<Ty>::epsilon())
    -> bool
  {
    return ( (a - b) > ( max( abs(a),abs(b) ) * eps ) || equal(a, b, eps));
  }

  /// @brief checks if `A` is less than `B`
  template <class Ty>
  requires std::is_floating_point_v<Ty>
  [[nodiscard]] inline constexpr
  auto less_than(Ty a, Ty b,
      const Ty eps = std::numeric_limits<Ty>::epsilon())
    -> bool
  {
    return (b - a) > ( max( abs(a),abs(b) ) * eps );
  }

  /// @brief checks if `A` is less than `B` or equal
  template <class Ty>
  requires std::is_floating_point_v<Ty>
  [[nodiscard]] inline constexpr
  auto less_or_equal(Ty a, Ty b,
      const Ty eps = std::numeric_limits<Ty>::epsilon())
    -> bool
  {
    return ( (b - a) > ( max( abs(a),abs(b) ) * eps ) || equal(a, b, eps) );
  }
} // namespace kraken::cal

#endif // FLOAT_COMP_HPP
