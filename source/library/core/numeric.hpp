#ifndef NUMERIC_HPP
#define NUMERIC_HPP

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

#include "../../Apology/apology.hpp"  // Apology, kraken::err_codes
#include "common/abs.hpp"             // abs
#include "common/newton.hpp"          // newton
#include <algorithm>                  // std::move, std::forward
#include <initializer_list>
#include <limits.h>                   // LLONG_MAX, LLONG_MIN
#include "matrix.hpp"                 // matrix_<>
#include "constants.hpp"
#include <bit>                        // std::countl_zero

template <class Ty>
struct Min_Max {
  const Ty Min{};
  const Ty Max{};
};

template <class Ty>
struct Division {
  Ty quot{}; // quotient
  Ty rem{}; // reminder
};

template <typename It> class My_Iota {
private:
  It m_begin{};
  It m_end{};

public:
  constexpr auto begin() const -> It { return m_begin; }
  constexpr auto end() const -> It { return m_end; }
  constexpr My_Iota(const It a, const It b)   : m_begin(a), m_end(b) {}
};

namespace kraken {
    namespace op {
    struct plus {
      constexpr auto operator()(auto &&...args) noexcept  { return (args + ...); }
    };
    struct multiplies {
      constexpr auto operator()(auto &&...args) noexcept  { return (args * ...); }
    };
    struct minus {
      constexpr auto operator()(auto &&...args) noexcept  { return (args - ...); }
    };
    struct devide {
      template <class... T>
      requires(
          std::is_floating_point_v<std::decay_t<T>> &&...) constexpr auto
      operator()(T &&...args) noexcept  {
        return (args / ...);
      }
    };

    /**
    * @brief helper function gives back the original values of container after being minupilated
    * @param target the container that has been minupilated
    * @param copy_org copied version of that container
    */
    template <class T>
    inline constexpr
    auto restore( T &target, const T &copy_org )
        -> void
    {
      target = std::move(copy_org);
    }
  } // namespace op
}

/// @brief pure calculations for mostly any container or a collection
namespace kraken::cal {

  /// @brief  short name for => accumulate
  template <class b, class Binary_op, class T>
  [[nodiscard]]
  inline constexpr
  auto acc(b init, Binary_op &&Op,
                  const T &container) noexcept
      -> b
   {
    for (auto &&item : container) {
      init = Op(item, std::move(init));
    }
    return init;
  }

  //
  template <class b, class Binary_op, class It_begin, class It_end>
  [[nodiscard]]
  inline constexpr
  auto acc(b &&init, Binary_op &&Op,
                  const It_begin it_begin,
                    const It_end it_end)
      -> b
  {
    for (auto &&item : My_Iota(it_begin, it_end)) {
      init = Op(item, std::move(init));
    }
    return init;
  }

  //
  template <class b, class It_begin, class It_end>
  [[nodiscard]]
  inline constexpr
  auto acc(b &&init, It_begin &&it_begin,
                  It_end &&it_end)
      -> b
  {
    for ( auto && i : My_Iota(it_begin, it_end)) {
      init += i;
    }
    return init;
  }

  ///
  template <class b, class T>
  [[nodiscard]]
  inline constexpr
  auto acc(b &&init, const T &container)
      -> b
  {
    for ( auto && i : container) {
      init += i;
    }
    return init;
  }

  /// @brief  short name for => calculate
  template <class Init, class Binray_op, class... Args>
  requires(std::is_same_v<Init, Args> &&...)
  [[nodiscard]]
  inline constexpr
  auto calcu(Init &&init, Binray_op op,
    Args &&...args)
      -> Init
  {
    return (op(args..., std::move(init)));
  }

  //
  template <class b, class T, class Binary_op>
  [[nodiscard]]
  inline constexpr
  auto calcu(b &&init, Binary_op op,
                    std::initializer_list<T> &&args)
      -> b
  {
    for ( auto &&i : args ) {
      init = op(std::move(init), i);
    }
    return init;
  }

  /// @brief check if the number is signed or not
  /// @return int
  template <class Ty>
  requires (std::is_integral_v<Ty> || std::is_floating_point_v<Ty>)
  constexpr inline
  auto is_neg(Ty val) -> int
  {
    if ( val < static_cast<Ty>(0) ) return true;
    return false;
  }

  /// @brief returns ln of `x`
  template<class Ty>
  requires (std::is_floating_point_v<Ty>)
  [[nodiscard]]
  inline constexpr
  auto ln( Ty val, src_loc src = src_loc::current() )
    -> Ty
  {
    if ( is_neg(val) ) {
      Apology( [&] { return error{ src.file_name(), src.function_name()
                              , err_codes::neg_arg, src.line() }; } );
    }
    if ( val == 1 ) { return 0; }
    if ( val <= 0 ) { return std::numeric_limits<Ty>::quiet_NaN(); }
    if ( val >= 2 ) { return ln(val/2.) + static_cast<Ty>(0.69314718056); }
    if ( val >= 1024) { return ln(val/1024.) + static_cast<Ty>(6.9314718056); }
    val -= 1;
    Ty res{0.};
    Ty x_pow {val};
    for ( std::size_t i {1}; i < 100; ++i ) {
      if ( i & 1 ) {
        res += x_pow / i;
      } else {
        res -= x_pow / i;
      }
      x_pow *= val;
    }
    return res;
  }


  /**
   * @brief calculates `floor` of float values
   * @return Ty
   */
  template<class Ty>
  requires std::is_floating_point_v<Ty>
  [[nodiscard]]
  inline constexpr
  auto floor(Ty val)
      -> auto
  {
    constexpr Ty max_llong {static_cast<Ty>(LLONG_MAX)};
    constexpr Ty min_llong {static_cast<Ty>(LLONG_MIN)};
    if ( val >= max_llong || val <= min_llong || val == 0 ) {
      return val;
    }
    const long long temp_n {static_cast<long long>(val)};
    const Ty temp_d {static_cast<Ty>(temp_n)};
    if (temp_d == val || val >= 0) { return temp_d; }
    else {return  (temp_d) - static_cast<Ty>(1);}
  }

  /// @brief stores division of an `element` of type `Ty`
  template<class Ty>
  [[nodiscard]]
  inline constexpr
      // numerator, denominator
  auto div(Ty num, Ty denom, src_loc src = src_loc::current()) -> Division<Ty>
  {
    if ( denom == 0 ) {
      Apology( [&] { return error{ src.file_name(), src.function_name()
                                      , err_codes::zero, src.line() }; } );
    }
    Division<Ty> res{};
    res.quot = num/denom;
    res.rem = num - (res.quot * denom);
    return res;
  }

  /// @brief gives ceil of a float value
  template<class Ty>
  requires std::is_floating_point_v<Ty>
  [[nodiscard]]
  inline constexpr
  auto ceil(Ty val)
      -> Ty
  {
    const std::int64_t temp {static_cast<std::int64_t>(val)};
    if (is_neg(val) || val == static_cast<Ty>(temp) ) { return static_cast<Ty> (temp); }
    return static_cast<Ty> (temp + 1);
  }

  /// @brief rounds float numbers
  template<class Ty>
  requires std::is_floating_point_v<Ty>
  [[nodiscard]]
  inline constexpr
  auto round(Ty val)
      -> Ty
  {
    return (val > static_cast<Ty>(0.)) ? ceil(val-static_cast<Ty>(.5)) :
                            floor(val+static_cast<Ty>(.5));
  }

/// @brief gives the sqrt of a integral or a a float number
  template<class Ty>
  requires (std::is_integral_v<Ty> || std::is_floating_point_v<Ty>)
  [[nodiscard]]
  inline constexpr
  auto sqrt(const Ty val, src_loc src = src_loc::current())
      -> Ty
  {
    if ( is_neg(val) ) {
      Apology( [&] { return error{ src.file_name(), src.function_name()
                              , err_codes::neg_arg, src.line() }; } );
    }
    return kraken::num_methods::newton(
      1., 20, [&val](auto &&x) {
        return (x*x) - val;
      }, [](auto &&y){
        return (static_cast<Ty>(2.)*y);
      });
  }

  /// @brief returns a*a
  template<class T>
  [[nodiscard]] inline constexpr
  auto sqr(const T a)
    -> T
  {
    return a*a;
  }

  /**
   * @brief get the number of elements after the decimal point
   * @param val
   * @return std::uint64_t
   */
  template<class Ty>
  requires std::is_floating_point_v<Ty>
  [[nodiscard]]
  inline constexpr auto decimal_places(Ty val)
    -> std::uint64_t
  {
    std::uint64_t dec_count{};
    val = abs(val);
    auto temp {val - floor(val)};
    Ty factor {10};
    auto eps {std::numeric_limits<Ty>::epsilon() * temp};
    while ((temp > eps && temp < (1 - eps)) && dec_count < std::numeric_limits<Ty>::max_digits10)
    {
      temp = val * factor;
      temp -= floor(temp);
      factor *= 10;
      eps = std::numeric_limits<Ty>::epsilon() * val * factor;
      ++dec_count;
    }
    return dec_count;
  }

  /**
   * @brief chops of the fractional part of any floating-point number
   * @param val
   * @return std::uint64_t
   */
  template<class Ty>
  requires std::is_floating_point_v<Ty>
  [[nodiscard]]
  inline constexpr auto trunc(Ty val)
    -> std::uint64_t
  {
    //check if float fits into integer
    if ( std::numeric_limits<std::uint64_t>::digits < std::numeric_limits<Ty>::digits) {
        // check if float is smaller than max std::uint64_t
        if( (val < static_cast<Ty>( std::numeric_limits<std::uint64_t>::max())) &&
            (val > static_cast<Ty>( std::numeric_limits<std::uint64_t>::min())) ) {
          return static_cast<std::uint64_t>(val); //safe to cast
        } else {
          return std::numeric_limits<std::uint64_t>::max();
        }
    } else {
        //It is safe to cast
        return static_cast<std::uint64_t>(val);
    }
  }

  /**
   * @brief computes base to power
   * @param base
   * @param power
   * @param eps
   */
  template<class Ty, class P>
  [[nodiscard]] inline constexpr
  auto pow(Ty base, P power, const Ty eps = std::numeric_limits<Ty>::epsilon())
      -> Ty
  {
    if constexpr ( std::is_integral_v<Ty> && std::is_integral_v<P>) {
        if ( power > 0 ) {
        Ty res {static_cast<Ty>(1)};
        Ty base_copy {std::move(base)};
        for ( ;; ) {
          if ( power & 1 ) { res *= base_copy; }
          power >>= 1;
          if ( !power ) { break; }
          base_copy *= base_copy;
        }
        return res;
      }
      Ty res {static_cast<Ty>(1)};
      Ty base_copy {std::move(base)};
      power *= -1;
      for ( ;; ) {
        if ( power &  1 ) { res *= base_copy; }
        power >>= 1;
        if ( !power ) { break; }
        base_copy *= base_copy;
      }
      return static_cast<Ty>(1.)/res;
    }
    if ( power < 0 )    { return 1. / pow(base, -power, eps); }
    if ( power >= 10 )  { return sqr( pow(base, power/static_cast<Ty>(2.),
                                                        eps/static_cast<Ty>(2.)) ); }
    if ( power >= 1 )   { return base * pow(base, power-static_cast<Ty>(1.), eps); }
    if ( eps >= static_cast<Ty>(1.) )     { return sqrt(base) ; }
    return sqrt( pow(base, power*static_cast<Ty>(2.), eps*static_cast<Ty>(2.)) );
  }

  /**
   * @brief pow fuction works with only for containers
   * @param base container
   * @param power
   */
  template<class Cont, class P>
  requires std::is_class_v<Cont>
  constexpr
  auto pow_container(Cont &container, const P& power)
      -> void
  {
    for (auto &i : container) {
      i = std::move( cal::pow(i, power) );
    }
  }

  template<class Cont, class P>
  requires std::is_class_v<Cont>
  constexpr
  auto pow_container(Cont &container, P&& power)
      -> void
  {
    for (auto &i : container) {
      i = std::move( cal::pow(i, power) );
    }
  }

  ///@brief  returns the square root of the sum of squares of its arguments
  template<class Ty>
  requires (std::is_floating_point_v<Ty> || std::is_integral_v<Ty>)
  [[nodiscard]]
  inline constexpr
  auto hypot(Ty x, Ty y, Ty z)
    -> Ty
  {
    return sqrt((x*x) + (y*y) + (z*z));
  }

  ///@brief  returns the square root of the sum of squares of its arguments
  template<class Ty>
  requires (std::is_floating_point_v<Ty> || std::is_integral_v<Ty>)
  [[nodiscard]]
  inline constexpr
  auto hypot(Ty x, Ty y)
    -> Ty
  {
    return sqrt((x*x) + (y*y));
  }

  ///@brief  returns the square root of the sum of squares of its arguments
  template<class Ty>
  requires (std::is_floating_point_v<Ty> || std::is_integral_v<Ty>)
  [[nodiscard]]
  inline constexpr
  auto hypot( const std::initializer_list<Ty>& args)
      -> auto
  {
    Ty sum {static_cast<Ty>(1.)};
    for ( const auto i : args ) {
      sum += (i*i);
    }
    return sqrt(sum);
  }

  /**
   * @brief calculate `max` with two numbers
   * @param  a  A thing of arbitrary type.
   * @param  b  Another thing of arbitrary type.
   * @return Ty
   */
  template<class T>
  requires std::is_floating_point_v<T> || std::is_integral_v<T>
  [[nodiscard]]
  inline constexpr
  auto max(const T a, const T b)
      -> T
  {
    return a > b ? a : b;
  }

  /// @brief calculates max in a init-list
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

  /**
   * @brief calculate `max` with arbitrary numbers
   * @param  a...  A thing of arbitrary type
   * @return Ty
   */
  template<class First, class Second, class ...Args>
  [[nodiscard]]
  inline constexpr
  auto max(First a, Second b,
            Args... args)
     -> First
  {
    if constexpr ( sizeof...(args) == 0 ) {
      return a > b ? a : b;
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
    -> auto
  {
    auto high {container[0]};
    for (auto &i : container) {
      if ( i > high ) { high = i; }
    }
    return high;
  }

  /**
   * @brief calculate `min` with two numbers
   * @param  a  A thing of arbitrary type.
   * @param  b  Another thing of arbitrary type.
   * @return Ty
   */
  template<class T>
  requires std::is_floating_point_v<T> || std::is_integral_v<T>
  [[nodiscard]]
  inline constexpr
  auto min(const T a, const T b)
      -> T
  {
    return a > b ? b : a;
  }

  /// @brief calculates min in a init-list
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

  /**
   * @brief calculate `min` with arbitrary numbers
   * @param  a...  A thing of arbitrary number of arguments
   * @return Ty
   */
  template<class First, class Second, class ...Args>
  [[nodiscard]]
  inline constexpr
  auto min(First a, Second b,
                Args... args)
     -> First
  {
    if constexpr ( sizeof...(args) == 0 ) {
      return a > b ? b : a;
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
    -> auto
  {
    auto low {container[0]};
    for (auto &i : container) {
      if ( i < low ) { low = i; }
    }
    return low;
  }

  /**
   * @brief calculate `min_max` with two numbers
   * @param  a  A thing of arbitrary type.
   * @param  b  Another thing of arbitrary type.
   * @return Min_Max
   */
  template<class T>
  requires std::is_floating_point_v<T> || std::is_integral_v<T>
  [[nodiscard]]
  inline constexpr
  auto min_max(const T a, const T b)
      -> Min_Max<T>
  {
    if ( a > b) {
      return { b, a };
    }
    return { a, b };
  }


  /// @brief calculates `min_max` in a init-list
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

  /**
   * @brief calculate `min_max` with arbitrary numbers
   * @param  a...  A thing of arbitrary number of arguments
   * @return Min_Max
   */
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

  /// @brief returns log base2 of `x`
  template<class Ty>
  [[nodiscard]] constexpr
  auto log2(Ty val, src_loc src = src_loc::current())
    -> Ty
  {
    if ( is_neg(val) ) {
      Apology( [&] { return error{ src.file_name(), src.function_name()
                                  , err_codes::neg_arg, src.line() }; } );
    }
    constexpr std::size_t sysbits {(std::numeric_limits<unsigned char>::digits * sizeof(void*))};
    if constexpr ( std::is_integral_v<Ty> && sysbits == 64 ) {
      return (static_cast<std::uint64_t>(63ull -
                    std::countl_zero(static_cast<std::uint64_t>(val))));
    } else if constexpr ( std::is_integral_v<Ty> && sysbits == 32 ) {
      return (static_cast<std::uint32_t>(31ul -
                    std::countl_zero(static_cast<std::uint32_t>(val))));
    }
    else return ln(val) * constants::log2e_v<Ty>;
  }

  /// @brief returns log base10 of `x`
  template<class Ty>
  [[nodiscard]] constexpr
  auto log10(Ty val, src_loc src = src_loc::current())
    -> Ty
  {
    if ( is_neg(val) ) {
      Apology( [&] { return error{ src.file_name(), src.function_name()
                                    , err_codes::neg_arg, src.line() }; } );
    }
    if constexpr ( std::is_integral_v<Ty> ) {
      return log2(static_cast<std::uint64_t>(val)) / log2(10ull);
    }
    else return ln(val) * constants::log10e_v<Ty>;
  }

  ///@brief computes the greatest common divisor
  ///@link https://lemire.me/blog/2013/12/26/fastest-way-to-compute-the-greatest-common-divisor/ @endlink
  template <class A, class B>
  requires (std::is_integral_v<A> && std::is_integral_v<B>)
  [[nodiscard]]
  inline constexpr
  auto gcd(A a, B b) -> A
  {
    int shift{};
    if (a == 0) return b;
    if (b == 0) return a;
    shift = std::countr_zero(static_cast<std::uint64_t>(a | b));
    a >>= std::countr_zero(static_cast<std::uint64_t>(a));
    do {
      b >>= std::countr_zero(static_cast<std::uint64_t>(b));
      if (a > b) {
        std::swap(a,b);
      }
      b -= a;
    } while (b != 0);
    return a << shift;
  }

  ///@brief computes the least common multiples
  template <class A, class B>
  requires (std::is_integral_v<A> && std::is_integral_v<B>)
  [[nodiscard]]
  inline constexpr
  auto lcm(A a, B b) -> A
  {
    return (a != 0 && b != 0)
      ? (a / gcd(a, b)) * b
      : 0;
  }

  ///@brief gives the `fibonacci` of `x`
  template<class Ty>
  requires std::is_integral_v<Ty>
  [[nodiscard]]
  inline constexpr
  auto fibonacci(const Ty val, src_loc src = src_loc::current())
    -> std::size_t
  {
    if ( is_neg(val) ) {
      Apology( [&] { return error{ src.file_name(), src.function_name()
                                  , err_codes::neg_arg, src.line() }; } );
    }
    if (val == 0) return 0ull;
    else if (val == 1) return 1ull;
    //
    constexpr double sqrt_5 { sqrt(5.) };
    return static_cast<std::size_t>
            (round( pow(constants::ϕ,
                static_cast<double>(val)) / sqrt_5 ));
  }

  template<class Ty>
  requires std::is_integral_v<Ty>
  [[nodiscard]] constexpr
  auto factorial(Ty val, src_loc src = src_loc::current())
    -> std::size_t
  {
    if ( is_neg(val) ) {
      Apology( [&] { return error{ src.file_name(), src.function_name()
                                    , err_codes::neg_arg, src.line() }; } );
    }
    if ( val <= 1 ) return 1;
    return val * factorial(val - 1);
  }

} // namespace kraken

#endif // NUMERIC_HPP
