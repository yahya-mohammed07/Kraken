#ifndef NUMERIC_METHODS_HPP
#define NUMERIC_METHODS_HPP

#include "matrix.hpp"
#include "numeric.hpp"

namespace kraken::num_methods {

  /**
   * @brief Perform's gauss-elimination
   * @return a gauss-eliminated matrix if it was normal
   */
  template<class T, std::size_t ROW, std::size_t COL>
    requires ( std::is_floating_point_v<T> )
  [[nodiscard]] constexpr
  auto gauss_elimination(const matrix_<T, ROW, COL> &matrix)
      -> const auto
  {
    bool gauss_sliminated = false;
    for (std::size_t i = 0; i < ROW; ++i ) {
      for ( std::size_t j = 0; j < COL; ++j) {
        if ( i > j ) {
          if ( matrix.at(i,j) == 0.0 ) gauss_sliminated = true;
        }
      }
    }
    matrix_<T, ROW, COL> arr = std::move(matrix);
    if ( !gauss_sliminated ) {
      double P{};
      for (std::size_t k = 0; k < ROW; ++k) {
        if ( arr.at(k, k) == 0 ) { continue; }
        for (std::size_t i = k + 1; i < ROW; ++i) {
          P = arr.at(i, k) / arr.at(k, k); // p = ( row-below / row-above it)
          for (std::size_t j = 0; j < COL; ++j) {
            // multiply P with every element in row above...the row below - that
            arr.at(i, j) -= (P * arr.at(k, j));
          }
        }
      }
    }
    return arr;
  }

  /**
   * @brief Gives the determined matrix, matrix must be squared
   * @return T
   */
  template<class T, std::size_t ROW, std::size_t COL>
  [[nodiscard]] constexpr
  auto determined(const matrix_<T, ROW, COL> &matrix)
      -> T
  {
    static_assert(ROW == COL, "- Matrix must be squared");
    const auto temp = std::move(gauss_elimination(matrix));

    T deter{1};
    for (std::size_t i = 0; i < COL; ++i) {
      deter *= temp.at( i,i );
    }

    return deter;
  }
  /**
  * @brief Performs least squares on xi, yi given by user
  * @param xi xi container
  * @param yi yi container
  * @param x the x value in (y = a+(b*X))
  * @return Ty
  */
  template <class Ty, const std::size_t COL>
  [[nodiscard]]
  inline constexpr
  auto least_squares(matrix_<Ty, 1ul, COL> xi,
                      matrix_<Ty, 1ul, COL> yi,
                        Ty &&x = 1)
      -> Ty
  {
    const auto sum_xi = cal::acc(static_cast<Ty>(0), xi);
    const auto sum_yi = cal::acc(static_cast<Ty>(0), yi);
    //
    matrix_<Ty, 1ul, COL> xi_temp = xi;
    const Ty sum_xi_pow2  = cal::acc(static_cast<Ty>(0), std::move(xi*xi));
    op::restore(xi, xi_temp);
    const Ty sum_product_xi_yi  = cal::acc(static_cast<Ty>(0),
                                                    std::move(xi*yi));
    //
    const Ty m = {static_cast<Ty>(xi.col())};
    const Ty b = {(m * sum_product_xi_yi - sum_yi * sum_xi)
                        /
                    (m * sum_xi_pow2 - sum_xi * sum_xi)};
    //
    const Ty a = {(sum_yi - b * sum_xi) / m};
    return a + (b * x); // returns y
  }

  /**
   * @brief change with right_side of the matrix
   * , each call gets incremented by 1
   * @param col the matrix col size
   * @param R linear collection of right_side
   * @return nothing
   */
  template<class T, std::size_t ROW, std::size_t COL>
  auto change_with_R(matrix_<T, ROW, COL> &from,
                                    matrix_<T, 1, COL> R)
      -> void
  {
    static std::size_t changed{};
    for (std::size_t i = 0; i < COL; ++i)
      from.at(i, changed) = R.at(0, i);

    ++changed;
  }
/**
 * @brief performs cramer's rule on given matrix
 *
 * @param arr the gavin matrix
 * @param right_side the values after =
 * @return matrix_<T, 1, COL>
 */
  template <class T, const size_t ROW, const size_t COL>
  auto cramer(matrix_<T, ROW, COL> arr, const matrix_<T, 1, COL> &right_side)
      -> auto
  {
    static_assert(ROW == COL, "- matrix must be squared");
    const matrix_<T, ROW, COL> copy = arr;
    const T deter_a = determined(arr);
    //
    matrix_<T, 1, COL> D;
    matrix_<T, 1, COL> X;
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

  /**
   * @brief Simpson's rule
   * @param n give n (must be even)
   * @param A a
   * @param B b
   * @param Op function obj
   */
  template<class A, class B, class Op>
  requires (std::is_floating_point_v<A> && std::is_floating_point_v<B>)
  [[nodiscard]]
  constexpr
  auto simpson(const std::size_t n, const A a, const B b, Op op)
    -> A
  {
    if ( n & 1 ) { std::cerr << "- `n` must be even!\n"; assert(false); }
    const std::size_t N {n};
    const A h = { ((b-a) / N) };
    //
    const A f_start = op(a);
    const A f_end   = op(b);
    //
    A m{}, sum{}, result{};
    for ( std::size_t i = 1; i < N; ++i ) {
      m = static_cast<decltype(A())>(a+(h*i));
      //
      if ( i & 1 ) { sum += op(m) * 4; }
      else { sum += op(m) * 2; }
    }
    result = { ( h / static_cast<decltype(A())>(3.) )
                    *
              ( f_start + sum + f_end ) };
    return result;
  }

  template<class A, class Op1, class Op2>
  requires (std::is_floating_point_v<A>)
  [[nodiscard]]
  constexpr
  extern auto newton(const A init, const std::size_t max_it, Op1 &&fx, Op2 &&fx_ )
    -> A;

  /**
   * @brief gives the back-substitution of gauss eliminated matrix
   * , must call gauss_elimination
   * @param col number of columns in matrix
   * @return matrix<float, 1>
   */
  template<class T, std::size_t ROW, std::size_t COL>
  [[nodiscard]] constexpr
  auto back_substitution(const matrix_<T, ROW, COL> &arr)
      -> auto
  {
    static_assert(ROW < COL, "--`col` must always be greater than `row`...");
    matrix_<T, 1, COL - 1> x;
    //
    for (int64_t i = static_cast<int64_t>(ROW - 1); i >= 0; --i) {
      x.at(0, i) = {arr.at(i, COL - 1)};
      for (std::size_t j = static_cast<std::size_t>(i); j < COL - 1; ++j)
        x.at(0, i) -= arr.at(i, j) * x.at(0, j);

      x.at(0, i) /= arr.at(i, i);
    }
    return x;
  }

} // namespace num_methods

#endif // NUMERIC_METHODS_HPP
