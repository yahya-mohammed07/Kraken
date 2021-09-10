#ifndef NUMERIC_HPP
#define NUMERIC_HPP

#include <algorithm> // std::move, std::forward
#include <type_traits>
#include <cassert>  // assert
#include <initializer_list>
#include <limits> // quite_NaN
#include "matrix.hpp"
#include "constants.hpp"

namespace lab::num_methods {
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
    A x0 = init; // initial guess
    A x1 = {}; // initial guess
    A tolerance = 1e-7; // 7 digit accuracy is desired
    A eps = 1e-14; // Do not divide by a number smaller than this
    for ( std::size_t i = 1; i < max_it; ++i ) {
      A y = fx(x0);
      A y_ = fx_(x0);
      if ( myAbs(y_) < eps ) {
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

namespace lab {
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
namespace lab::cal {

  /// @brief  short name for => accumulate
  template <class V, class Binary_op, class T>
  [[nodiscard]]
  extern constexpr
  auto acc(V &&init, Binary_op Op,
                  T &container) noexcept
      -> V
   {
    for (auto &&item : container /*std::ranges::views::all(container)*/) {
      init = Op(item, std::move(init));
    }
    return init;
  }

  //
  template <class V, class Binary_op, class It_begin, class It_end>
  [[nodiscard]]
  extern constexpr
  auto acc(V &&init, Binary_op Op,
                  const It_begin it_begin,
                    const It_end it_end) noexcept
      -> V
  {
    for (auto &&item : My_Iota(it_begin, it_end)) {
      init = Op(item, std::move(init));
    }
    return init;
  }

  //
  template <class V, class It_begin, class It_end>
  [[nodiscard]]
  inline constexpr
  auto acc(V &&init, It_begin &&it_begin,
                  It_end &&it_end) noexcept
      -> V
  {
    return acc(init, op::plus{}, it_begin, it_end);
  }

  ///
  template <class V, class T>
  [[nodiscard]]
  inline constexpr
  auto acc(V &&init, const T &container) noexcept
      -> V
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
  template <class V, class T, class Binary_op>
  [[nodiscard]]
  inline constexpr
  auto calcu(V &&init, Binary_op op,
                    std::initializer_list<T> &&args) noexcept
      -> V
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
    Ty res = static_cast<Ty>(1);
    Ty base_copy = {std::move(base)};
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
    Ty res = static_cast<Ty>(1);
    Ty base_copy = std::move(base);
    power *= -1;
    for ( ;; ) {
      if ( power &  1 ) { res *= base_copy; }
      power >>= 1;
      if ( !power ) { break; }
      base_copy *= base_copy;
    }
    return static_cast<Ty>(1.)/res;
  }

  //
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
    Ty x_pow = val;
    for ( std::size_t i = 1; i < 100; ++i ) {
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
      -> Ty
  {
    constexpr Ty max_llong = static_cast<Ty> (9223372036854775807);
    if ( val >= max_llong || val <= -(max_llong) || val == 0 ) {
      return val;
    }
    const long long temp_n = static_cast<long long>(val);
    Ty temp_d = static_cast<Ty>(temp_n);
    if (temp_d == val || val >= 0) { return temp_d; }
    else {return  (temp_d) - static_cast<Ty>(1);}
  }

  /// @brief gives the absolute of a value
  template<class Ty>
  requires (!std::is_class_v<Ty>)
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
    std::int64_t temp = {static_cast<std::int64_t>(val)};
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
    return val > static_cast<Ty>(0.) ? ceil(std::forward<Ty>(val-static_cast<Ty>(.5))) :
    floor(std::forward<Ty>(val+static_cast<Ty>(.5)));
  }

/// @brief gives the sqrt of a integral or a a float number
  template<class Ty>
  requires (std::is_integral_v<Ty> || std::is_floating_point_v<Ty>)
  [[nodiscard]]
  inline constexpr
  auto sqrt(const Ty val) noexcept
      -> Ty
  {
    return lab::num_methods::newton(
      (static_cast<Ty>(1.)),
      20, [&val](auto &&x) {
        return (x*x) - val;
      }, [](auto &&y){
        return (static_cast<Ty>(2.)*y);
      });
  }

  template<class T>
  requires (std::is_floating_point_v<T>)
  [[nodiscard]] inline constexpr
  auto sqr(const T a)
    -> T
  {
    return a*a;
  }

  /**
   * @brief under testing...
   * @tparam Ty
   * @tparam P
   */
  template<class Ty, class B>
  requires (std::is_floating_point_v<Ty> && std::is_floating_point_v<B>)
  [[nodiscard]] inline constexpr
  auto powf(Ty base, B power, Ty eps = .000'000'000'00'1f)
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
    Ty sum = static_cast<Ty>(1.);
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
    Ty begin = *args.begin();
    for ( auto && i : args) {
      if ( i > begin ) { begin = i; }
    }
    return begin;
  }

  /// @brief calculates max in a init-list
  template<class Ty>
  [[nodiscard]]
  inline constexpr
  auto max(const std::initializer_list<Ty>& args) noexcept
      -> Ty
  {
    Ty begin = *args.begin();
    for (const auto & i : args) {
      if ( i > begin ) { begin = i; }
    }
    return begin;
  }

  /**
   * @brief calculate `max` with arbitrary numbers
   * @param  a...  A thing of arbitrary type
   * @return Ty
   */
  template<class First, class Second, class ...Args>
  inline constexpr
  auto max(First && a, Second && b,
            Args &&... args) noexcept
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
  inline constexpr
  auto max(Cont &container)
    -> std::size_t
  {
    auto begin = *container.begin();
    for (auto &i : container) {
      if ( i > begin ) { begin = i; }
    }
    return begin;
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
    Ty begin = *args.begin();
    for ( auto && i : args) {
      if ( i < begin ) { begin = i; }
    }
    return begin;
  }

  /// @brief calculates min in a init-list
  template<class Ty>
  [[nodiscard]]
  inline constexpr
  auto min(const std::initializer_list<Ty>& args) noexcept
      -> Ty
  {
    Ty begin = *args.begin();
    for (const auto & i : args) {
      if ( i < begin ) { begin = i; }
    }
    return begin;
  }

  /**
   * @brief calculate `min` with arbitrary numbers
   * @param  a...  A thing of arbitrary type
   * @return Ty
   */
  template<class First, class Second, class ...Args>
  inline constexpr
  auto min(First && a, Second && b,
                Args &&... args) noexcept
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
  inline constexpr
  auto min(Cont &container)
    -> std::size_t
  {
    auto begin = *container.begin();
    for (auto &i : container) {
      if ( i < begin ) { begin = i; }
    }
    return begin;
  }


  /// @link from https://stackoverflow.com/a/11398748
  template<class Ty>
  [[nodiscard]] constexpr
  auto log2(Ty val)
    -> Ty
  {
    assert(val>0ull && "- value must be greater than 0");
    if constexpr ( std::is_integral_v<Ty> ) {
      constexpr int tab64[64] = {
        63,  0, 58,  1, 59, 47, 53,  2,
        60, 39, 48, 27, 54, 33, 42,  3,
        61, 51, 37, 40, 49, 18, 28, 20,
        55, 30, 34, 11, 43, 14, 22,  4,
        62, 57, 46, 52, 38, 26, 32, 41,
        50, 36, 17, 19, 29, 10, 13, 21,
        56, 45, 25, 31, 35, 16,  9, 12,
        44, 24, 15,  8, 23,  7,  6,  5
      };
      val |= val >> 1;
      val |= val >> 2;
      val |= val >> 4;
      val |= val >> 8;
      val |= val >> 16;
      val |= static_cast<std::size_t>(val) >> 32;
      return tab64[(static_cast<std::size_t>((val - (val >> 1))*0x07EDD5E59A4E28C2)) >> 58];
    }
    else return ln(val) * constants::log2e_v<Ty>;
  }

  template<class Ty>
  [[nodiscard]] constexpr
  auto log10(Ty val)
    -> Ty
  {
    if constexpr ( std::is_integral_v<Ty> ) {
      return log2(static_cast<std::size_t>(val)) / log2(10ull);
    }
    else return ln(val) * constants::log10e_v<Ty>;
  }

} // namespace math

#endif // NUMERIC_HPP
