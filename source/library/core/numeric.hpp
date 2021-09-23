#ifndef NUMERIC_HPP
#define NUMERIC_HPP

#include <algorithm>      // std::move, std::forward
#include <type_traits>
#include <cassert>        // assert
#include <initializer_list>
#include <limits>         // quite_NaN, epslon()
#include <limits.h>       // LLONG_MAX, LLONG_MIN
#include "matrix.hpp"     // matrix_<>
#include "constants.hpp"
#include <bit>            // std::countl_zero

template <class Ty>
struct Min_Max {
  const Ty Min{};
  const Ty Max{};
};

namespace kraken::num_methods {
  template<class A, class Op1, class Op2>
  requires (std::is_floating_point_v<A>)
  [[nodiscard]]
  constexpr
  auto newton(const A init, const std::size_t max_it, Op1 &&fx, Op2 &&fx_ )
    -> A
  {
    constexpr auto myAbs = [](const auto& x)
    {
      return x > 0 ? x : -1 * (x);
    };
    A x0 {init}; // initial guess
    A x1 {}; // initial guess
    const A tolerance {1e-7}; // 7 digit accuracy is desired
    for ( std::size_t i {1}; i < max_it; ++i ) {
      A y  {fx(x0)};
      A y_ {static_cast<A>(fx_(x0))};
      if ( myAbs(y_) < std::numeric_limits<double>::epsilon() ) {
        break;
      }
      x1 = x0-(y/y_); // Do Newton's computation
      if ( myAbs(x1-x0) <= tolerance ) { // Stop when the result is within the desired tolerance
        break;
      }
      x0=x1;
    }
    return x1;
  }
}

template <typename It> class My_Iota {
private:
  It m_begin{};
  It m_end{};

public:
  constexpr auto begin() const -> It { return m_begin; }
  constexpr auto end() const -> It { return m_end; }
  constexpr My_Iota(const It a, const It b) noexcept : m_begin(a), m_end(b) {}
};

namespace kraken {
    namespace op {
    struct plus {
      constexpr auto operator()(auto &&...args) noexcept { return (args + ...); }
    };
    struct multiplies {
      constexpr auto operator()(auto &&...args) noexcept { return (args * ...); }
    };
    struct minus {
      constexpr auto operator()(auto &&...args) noexcept { return (args - ...); }
    };
    struct devide {
      template <class... T>
      requires(
          std::is_floating_point_v<std::decay_t<T>> &&...) constexpr auto
      operator()(T &&...args) noexcept {
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
    auto restore( T &target, const T &copy_org ) noexcept
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
  extern constexpr
  auto acc(b &&init, Binary_op Op,
                  T &container) noexcept
      -> b
   {
    for (auto &&item : container /*std::ranges::views::all(container)*/) {
      init = Op(item, std::move(init));
    }
    return init;
  }

  //
  template <class b, class Binary_op, class It_begin, class It_end>
  [[nodiscard]]
  extern constexpr
  auto acc(b &&init, Binary_op Op,
                  const It_begin it_begin,
                    const It_end it_end) noexcept
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
                  It_end &&it_end) noexcept
      -> b
  {
    return acc(init, op::plus{}, it_begin, it_end);
  }

  ///
  template <class b, class T>
  [[nodiscard]]
  inline constexpr
  auto acc(b &&init, const T &container) noexcept
      -> b
  {
    return acc(init, op::plus{}, container);
  }

  /// @brief  short name for => calculate
  template <class Init, class Binray_op, class... Args>
  requires(std::is_same_v<Init, Args> &&...)
  [[nodiscard]]
  constexpr
  auto calcu(Init &&init, Binray_op op,
    Args &&...args)  noexcept
      -> Init
  {
    return (op(args..., std::move(init)));
  }

  //
  template <class b, class T, class Binary_op>
  [[nodiscard]]
  inline constexpr
  auto calcu(b &&init, Binary_op op,
                    std::initializer_list<T> &&args) noexcept
      -> b
  {
    for ( auto &&i : args ) {
      init = op(std::move(init), i);
    }
    return init;
  }

  /**
   * @brief pow fuction works with only for integral types
   * @param base lvalue
   * @param power (positive)
   * @return Ty
   */
  template<class Ty, class P>
  requires (std::is_integral_v<Ty> && std::is_integral_v<P>)
  [[nodiscard]]
  inline constexpr
  auto pow( Ty base, P power ) noexcept
      -> Ty
  {
    assert("Ther power input must not be negative!" && power >= 0);
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

  /**
   * @brief pow function only floating point types
   * @param base
   * @param power negative power
   * @return Ty
   */
  template<class Ty>
  requires std::is_floating_point_v<Ty>
  [[nodiscard]]
  inline constexpr
  auto pow(Ty base, int64_t power ) noexcept
      -> Ty
  {
    assert("Ther power input must be negative!" && power < 0);
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

  /// @brief returns ln of `x`
  template<class Ty>
  requires (std::is_floating_point_v<Ty>)
  [[nodiscard]]
  inline constexpr
  auto ln( Ty val )
    -> Ty
  {
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

  /**
   * @brief calculates `floor` of float values
   * @return Ty
   */
  template<class Ty>
  requires std::is_floating_point_v<Ty>
  [[nodiscard]]
  inline constexpr
  auto floor(Ty val) noexcept
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

  /// @brief gives the absolute of a value
  template<class Ty>
  requires (!std::is_class_v<Ty>)
  [[nodiscard]]
  inline constexpr
  auto abs(const Ty val) noexcept
      -> Ty
  {
    if  (val < 0) return -val;
    else { return val; }
  }

  /// @brief gives ceil of a float value
  template<class Ty>
  requires std::is_floating_point_v<Ty>
  [[nodiscard]]
  inline constexpr
  auto ceil(Ty val) noexcept
      -> Ty
  {
    const std::int64_t temp {static_cast<std::int64_t>(val)};
    if (val < 0 || val == static_cast<Ty>(temp) ) { return static_cast<Ty> (temp); }
    return static_cast<Ty> (temp + 1);
  }

  /// @brief rounds float numbers
  template<class Ty>
  requires std::is_floating_point_v<Ty>
  [[nodiscard]]
  inline constexpr
  auto round(Ty val) noexcept
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
  auto sqrt(const Ty val) noexcept
      -> Ty
  {
    return kraken::num_methods::newton(
      (static_cast<Ty>(1.)),
      20, [&val](auto &&x) {
        return (x*x) - val;
      }, [](auto &&y){
        return (static_cast<Ty>(2.)*y);
      });
  }

  /// @brief returns a*a
  template<class T>
  requires (std::is_floating_point_v<T>)
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
   * @brief computes pow of floating point
   * @param base
   * @param power
   * @param eps
   */
  template<class Ty, class B>
  requires (std::is_floating_point_v<Ty> && std::is_floating_point_v<B>)
  [[nodiscard]] inline constexpr
  auto powf(Ty base, B power, Ty eps = std::numeric_limits<double>::epsilon())
      -> Ty
  {
    if ( power < 0 )    { return 1. / powf(base, -power, eps); }
    if ( power >= 10 )  { return sqr( powf(base, power/static_cast<Ty>(2.),
                                                        eps/static_cast<Ty>(2.)) ); }
    if ( power >= 1 )   { return base * powf(base, power-static_cast<Ty>(1.), eps); }
    if ( eps >= static_cast<Ty>(1.) )     { return sqrt(base) ; }
    return sqrt( powf(base, power*static_cast<Ty>(2.), eps*static_cast<Ty>(2.)) );
  }

  template<class Ty>
  requires (std::is_floating_point_v<Ty> || std::is_integral_v<Ty>)
  [[nodiscard]]
  inline constexpr
  auto hypot(Ty x, Ty y, Ty z) noexcept
    -> Ty
  {
    return sqrt((x*x) + (y*y) + (z*z));
  }

  template<class Ty>
  requires (std::is_floating_point_v<Ty> || std::is_integral_v<Ty>)
  [[nodiscard]]
  inline constexpr
  auto hypot(Ty x, Ty y) noexcept
    -> Ty
  {
    return sqrt((x*x) + (y*y));
  }

  template<class Ty>
  requires (std::is_floating_point_v<Ty> || std::is_integral_v<Ty>)
  [[nodiscard]]
  inline constexpr
  auto hypot( const std::initializer_list<Ty>& args) noexcept
      -> auto
  {
    Ty sum {static_cast<Ty>(1.)};
    for ( auto&& i : args ) {
      sum += (i*i);
    }
    return sqrt(std::forward<Ty>(sum));
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
  auto max(const T a, const T b) noexcept
      -> T
  {
    return a > b ? a : b;
  }

  /// @brief calculates max in a init-list
  template<class Ty>
  [[nodiscard]]
  inline constexpr
  auto max( std::initializer_list<Ty>&& args) noexcept
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
  auto max(const std::initializer_list<Ty>& args) noexcept
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
            Args... args) noexcept
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
  auto min(const T a, const T b) noexcept
      -> T
  {
    return a > b ? b : a;
  }

  /// @brief calculates min in a init-list
  template<class Ty>
  [[nodiscard]]
  inline constexpr
  auto min( std::initializer_list<Ty>&& args) noexcept
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
  auto min(const std::initializer_list<Ty>& args) noexcept
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
                Args... args) noexcept
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
  auto min_max(const T a, const T b) noexcept
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
  auto min_max(const std::initializer_list<Ty>& args) noexcept
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
  auto min_max(std::initializer_list<Ty> &&args) noexcept
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
                Args ...args) noexcept
     -> Min_Max<First>
  {
    return { min({a,b,args...}) , max({a,b,args...}) };
  }

  /// @brief: `finds min_max` in a container
  template<class Cont>
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
  auto log2(Ty val)
    -> Ty
  {
    assert(val > 0ull && "- value must be greater than `0`");
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
  auto log10(Ty val)
    -> Ty
  {
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

} // namespace math

#endif // NUMERIC_HPP
