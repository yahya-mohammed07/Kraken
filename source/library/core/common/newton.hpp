#ifndef NEWTON_HPP
#define NEWTON_HPP

#include "abs.hpp" // abs
#include <limits>

namespace kraken::num_methods {
    template<class A, class Op1, class Op2>
  requires (std::is_floating_point_v<A>)
  [[nodiscard]]
  constexpr
  auto newton(const A init, const std::size_t max_it, Op1 &&fx, Op2 &&fx_ )
    -> A
  {
    A x0 {init}; // initial guess
    A x1 {}; // initial guess
    const double tolerance {1e-7}; // 7 digit accuracy is desired
    for ( std::size_t i {1}; i < max_it; ++i ) {
      A y  {fx(x0)};
      A y_ {static_cast<A>(fx_(x0))};
      if ( kraken::cal::abs(y_) < std::numeric_limits<double>::epsilon() ) {
        break;
      }
      x1 = x0-(y/y_); // Do Newton's computation
      if ( kraken::cal::abs(x1-x0) <= tolerance ) {
        // Stop when the result is within the desired tolerance
        break;
      }
      x0=x1;
    }
    return x1;
  }
}

#endif // NEWTON_HPP