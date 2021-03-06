#ifndef NEWTON_HPP
#define NEWTON_HPP

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

#include "comp_decimal_point_nums.hpp" // comparing numbers with decimal point
#include <utility>

namespace kraken::num_methods {

/// @brief computes the newton method given `init` and two function ops
/// @param init inital value
/// @param max_it max iterations
/// @param fx f(x)
/// @param fx_ f`(x)
template <class A, class Op1, class Op2>
[[nodiscard]] constexpr is_float auto
newton(const A init, const std::size_t max_it, Op1 &&fx, Op2 &&fx_) {
  A x0{static_cast<A>(init)};                  // initial guess
  A x1{};                                      // initial guess
  constexpr A tolerance{static_cast<A>(1e-7)}; // 7 digit accuracy is desired
  constexpr A eps = std::numeric_limits<A>::epsilon();
  for (std::size_t i{1}; i < max_it; ++i) {
    A y{static_cast<A>(fx(x0))};
    A y_{static_cast<A>(fx_(x0))};
    if (cal::less_than(y_, eps, tolerance)) {
      break;
    }
    x1 = x0 - (y / y_); // Do Newton's computation
    if (cal::less_or_equal(cal::abs(x1 - x0), tolerance, eps)) {
      // Stop when the result is within the desired tolerance
      break;
    }
    x0 = std::move(x1);
  }
  return x1;
}
} // namespace kraken::num_methods

#endif // NEWTON_HPP