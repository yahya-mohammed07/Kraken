#ifndef NUMERIC_METHODS_HPP
#define NUMERIC_METHODS_HPP

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

#include "common/Apology/apology.hpp"
#include "common/newton.hpp"
#include "common/comp_decimal_point_nums.hpp"      // comparing numbers with decimal point
#include "matrix.hpp"
#include "numeric.hpp"

namespace kraken::num_methods {

  /// @brief Perform's gauss-elimination
  /// @return a gauss-eliminated matrix if it was normal
  template<class Ty, std::size_t ROW, std::size_t COL>
    requires ( std::is_floating_point_v<Ty> )
  [[nodiscard]] constexpr
  auto gauss_elimination(const matrix_<Ty, ROW, COL> &matrix)
  {
    bool gauss_sliminated {false};
    for (std::size_t i {}; i < ROW; ++i ) {
      for ( std::size_t j {}; j < COL; ++j) {
        if ( i > j ) {
          if ( cal::equal(matrix.at( i,j ), static_cast<Ty>(0.)) )
            {gauss_sliminated = true;}
        }
      }
    }
    matrix_<Ty, ROW, COL> arr = std::move(matrix);
    if ( !gauss_sliminated ) {
      Ty P{};
      for (std::size_t k {}; k < ROW; ++k) {
        if ( arr.at(k, k) == 0 ) { continue; }
        for (std::size_t i {k+1}; i < ROW; ++i) {
          P = arr.at(i, k) / arr.at(k, k); // p = ( row-below / row-above it)
          for (std::size_t j {}; j < COL; ++j) {
            // multiply P with every element in row above...
            // the row below - that
            arr.at(i, j) -= (P * arr.at(k, j));
          }
        }
      }
    }
    return arr;
  }

  /// @brief Gives the determined matrix, matrix must be squared
  /// @return Ty
  template<class Ty, std::size_t ROW, std::size_t COL>
  [[nodiscard]] constexpr
  auto determined(const matrix_<Ty, ROW, COL> &matrix)
      -> Ty
  {
    static_assert(ROW == COL, "- Matrix must be squared");
    const auto temp{ std::move(gauss_elimination(matrix)) };

    Ty deter{1};
    for (std::size_t i {0}; i < COL; ++i) {
      deter *= temp.at( i,i );
    }

    return deter;
  }

  /// @brief Performs least squares on xi, yi given by user
  /// @param xi xi container
  /// @param yi yi container
  /// @param x the x value in (y = a+(b*X))
  /// @return Ty
  template <class Ty, const std::size_t COL>
  [[nodiscard]]
  constexpr
  auto least_squares(matrix_<Ty, 1UL, COL> xi,
                      matrix_<Ty, 1UL, COL> yi,
                        Ty x = 1)
      -> Ty
  {
    const auto sum_xi { cal::acc(static_cast<Ty>(0), xi) };
    const auto sum_yi { cal::acc(static_cast<Ty>(0), yi)};
    //
    matrix_<Ty, 1UL, COL> xi_temp {xi};
    const Ty sum_xi_pow2 { cal::acc(static_cast<Ty>(0), std::move(xi*xi)) };
    op::restore(xi, xi_temp);
    const Ty sum_product_xi_yi {cal::acc(static_cast<Ty>(0),
                                                    std::move(xi*yi)) };
    //
    const Ty m {static_cast<Ty>(xi.col())};
    const Ty b {(m * sum_product_xi_yi - sum_yi * sum_xi)
                        /
                    (m * sum_xi_pow2 - sum_xi * sum_xi)};
    //
    const Ty a {(sum_yi - b * sum_xi) / m};
    return a + (b * x); // returns y
  }

  /**
   * @brief change with right_side of the matrix
   * , each call gets incremented by 1
   * @param col the matrix col size
   * @param R linear collection of right_side
   * @return nothing
   */
  template<class Ty, std::size_t ROW, std::size_t COL>
  auto change_with_R(matrix_<Ty, ROW, COL> &from,
                                    matrix_<Ty, 1, COL> R)
      -> void
  {
    static std::size_t changed{};
    for (std::size_t i {}; i < COL; ++i)
      {from.at(i, changed) = R.at(0, i);}

    ++changed;
  }

  /// @brief performs cramer's rule on given matrix
  /// @param arr the gavin matrix
  /// @param right_side the values after =
  /// @return matrix_<Ty, 1, COL>
  template <class Ty, const size_t ROW, const size_t COL>
  auto cramer(matrix_<Ty, ROW, COL> arr, const matrix_<Ty, 1, COL> &right_side)
      -> auto
  {
    static_assert(ROW == COL, "- matrix must be squared");
    const matrix_<Ty, ROW, COL> copy {arr};
    const Ty deter_a {determined(arr)};
    //
    matrix_<Ty, 1, COL> D;
    matrix_<Ty, 1, COL> X;
    //
    for (size_t i = 0; i < COL; ++i) {
      // reset arr after being minupilated in line 85
      op::restore(arr, copy);
      change_with_R(arr, right_side);
      D.at(0, i) = (determined(arr));
      X.at(0, i) = (D.at(0, i) / deter_a);
    }
    return X;
  }

  /// @brief Simpson's rule
  /// @param n give n (must be even)
  /// @param A a
  /// @param B b
  /// @param Op function obj
  /// @return A
  template<class A, class B, class Op>
  requires (std::is_floating_point_v<A> && std::is_floating_point_v<B>)
  [[nodiscard]]
  constexpr
  auto simpson(const std::size_t n, const A a, const B b, Op op,
                 [[maybe_unused]] src_loc src = src_loc::current())
    -> A
  {
    #ifdef APOLOGY
    if ( n & 1 ) {
        Apology( [&src] { return error{ src.file_name(), src.function_name(),
                                      err_codes::odd, src.line() }; } );
    }
    #endif
    const A h { ((b-a) / static_cast<A>(n)) };
    //
    const A f_start {op(a)};
    const A f_end   {op(b)};
    //
    A m{}, sum{};
    for ( auto i {1UL}; i < n; ++i ) {
      m = static_cast<A>(a+(h*static_cast<A>(i)));
      //
      if ( i & 1UL ) { sum += op(m) * 4; }
      else { sum += op(m) * 2; }
    }
    return ( h / static_cast<A>(3.) )
                    *
              ( f_start + sum + f_end );
  }

  /// @brief gives the back-substitution of gauss eliminated matrix
  /// , must call gauss_elimination
  /// @param col number of columns in matrix
  /// @return matrix<Ty, 1, COL>
  template<class Ty, std::size_t ROW, std::size_t COL>
  [[nodiscard]]
  constexpr
  auto back_substitution(const matrix_<Ty, ROW, COL> &arr)
      -> auto
  {
    static_assert(ROW < COL, "--`col` must always be greater than `row`...");
    matrix_<Ty, 1, COL - 1> x;
    //
    for (auto i {static_cast<int64_t>(ROW - 1)}; i >= 0; --i) {
      x.at(0, i) = {arr.at(i, COL - 1)};
      for (auto j {static_cast<std::size_t>(i)}; j < COL - 1; ++j)
        {x.at(0, i) -= arr.at(i, j) * x.at(0, j);}

      x.at(0, i) /= arr.at(i, i);
    }
    return x;
  }

  /// @brief performes Newton's Forward Difference Formula on two dynamic containers
  /// @param xi container
  /// @param yi container
  /// @param x [by default it's `1`]
  /// @return Ty
  template < class Ty, class Cont>
  [[nodiscard]]
  is_float auto newtown_forward(const Cont &xi,
                     Cont &yi, const Ty x = 1)
  {
    Cont forward { yi[0] }; // holds forward difference of `yi` init = yi[0]
    std::size_t size {yi.size()};
    //
    for ( std::size_t i {}; i < size; ++i ) { // calculates forward difference of `yi`
      Cont temp{};
      for ( std::size_t j {1}; j < size; ++j ) {
        temp.push_back( yi[j-1] = ( yi[j] - yi[j-1] ) );
        if ( j == yi.size()-1 ) { yi.erase( yi.begin() +
                                    static_cast<int64_t>(size-1) ); }
      }
      size = temp.size();
      forward.push_back( temp[0] );
    }
    forward.push_back( yi[0] = yi[1]-yi[0] );
    const auto cal_u = []( const Ty u, const std::size_t times )
      -> Ty
    {
      Ty result {u};
      for ( std::size_t i {1}; i < times; ++i ) {
        result *= u - static_cast<Ty>( i );
      }
      return result;
    };
    const Ty u { ( x - xi[0] ) / ( xi[1] - xi[0] ) };
    Ty y { (forward[0]) };
    //
    for ( std::size_t i {1}; i < xi.size(); ++i ) {
      y = y + ( cal_u(u, i) * ( forward[i] ) )
          /
      static_cast<Ty>( cal::factorial( i ) );
    }
    return y;
  }

  /// @brief performs lagrange interpolation on `xi, yi` with `x` provided
  /// @param xi first container
  /// @param yi second container
  /// @param x
  /// @return Ty
  template < class Cont, class Ty >
  [[nodiscard]]
  constexpr
  is_float auto lagrange( const Cont &xi, const Cont &yi , const Ty x )
  {
    Ty temp{};
    Ty lagrange{};
    for (size_t i{}; i < xi.size(); ++i) {
      temp = yi[i];
      for (size_t j{}; j < xi.size(); ++j) {
        if (i != j) {
          temp *= (x - xi[j]) / (xi[i] - xi[j]);
        }
      }
      lagrange += temp;
    }
    return lagrange;
  }
} // namespace kraken::num_methods

#endif // NUMERIC_METHODS_HPP
