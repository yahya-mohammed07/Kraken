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

#include "common/abs.hpp" // abs
#include "common/basic_bit_cast.hpp"
#include "common/comp_decimal_point_nums.hpp" // comparing numbers with decimal point
#include "common/max_min.hpp"                 // max & min
#include "common/my_concepts.hpp"
#include "common/newton.hpp" // newton
#include "constants.hpp"
#include "matrix.hpp" // matrix_<>
#include <bit>        // std::countl_zero
#include <bitset>

template <class Num, class Demon> struct Division {
  Num Quot{};  // quotient
  Demon Rem{}; // reminder
};

namespace kraken::op {
struct plus {
  constexpr auto operator()(auto &&...args) noexcept { return (args + ...); }
};
struct multiplies {
  constexpr auto operator()(auto &&...args) noexcept { return (args * ...); }
};
struct minus {
  constexpr auto operator()(auto &&...args) noexcept { return (args - ...); }
};
struct divide {
  template <class... T>
  requires(std::is_floating_point_v<std::decay_t<T>> &&...) constexpr auto
  operator()(T &&...args) noexcept {
    return (args / ...);
  }
};

/// @brief helper function gives back the original values of container after
/// being minupilated
/// @param target the container that has been minupilated
/// @param copy_org copied version of that container
template <class T>
constexpr auto restore(T &target, const T &copy_org) -> void {
  target = std::move(copy_org);
}
} // namespace kraken::op

/// @brief pure calculations for mostly any container or a collection
namespace kraken::cal {

/// @brief  short name for => accumulate
/// @brief Computes the sum of the given value init
/// and a container. uses the given binary function op
/// @param init 	initial value of the sum
/// @param op function op
/// @param container the container
/// @return Ty
template <class Ty, class Op, class Cont>
requires real_num<Ty>
[[nodiscard]] constexpr auto acc(Ty init, Op &&op,
                                 const Cont &container) noexcept -> Ty {
  for (auto &&item : container) {
    init = op(item, std::move(init));
  }
  return init;
}

/// @brief  short name for => accumulate
/// @brief Computes the sum of the given value init
/// and a [first, last] iterators. uses the given binary function op
/// @param init 	initial value of the sum
/// @param op function op
/// @param container the container
/// @return Ty
template <class Ty, class Op, class It_begin, class It_end>
[[nodiscard]] constexpr auto acc(Ty &&init, Op &&op, const It_begin it_begin,
                                 const It_end it_end) noexcept -> Ty {
  for (auto &&item : My_Iota(it_begin, it_end)) {
    init = op(item, init);
  }
  return init;
}

/// @brief  short name for => accumulate
/// @brief Computes the sum of the given value init
/// and a [first, last] iterators. uses operator+ to sum up the elements
/// @param init 	initial value of the sum
/// @param container the container
/// @return A
template <class A, class It_begin, class It_end>
[[nodiscard]] constexpr auto acc(A &&init, It_begin &&it_begin,
                                 It_end &&it_end) noexcept -> A {
  for (auto &&i : My_Iota(it_begin, it_end)) {
    init += i;
  }
  return init;
}
/// @brief  short name for => accumulate
/// @brief Computes the sum of the given value init
/// and a container. uses operator+ to sum up the elements
/// @param init 	initial value of the sum
/// @param container the container
/// @return A
template <class A, class T>
[[nodiscard]] constexpr auto acc(A &&init, const T &container) -> A {
  for (auto &&i : container) {
    init += std::move(i);
  }
  return init;
}

/// @brief short for calculate
/// @brief Computes the sum of the given value init
/// and a `n` args. uses the given binary function op
/// @param init 	initial value of the sum
/// @param op function op
/// @param args `n` args
template <class Init, class Op, class... Args>
requires(std::is_same_v<Init, Args> &&...)
    [[nodiscard]] constexpr auto calcu(Init &&init, Op op, Args &&...args)
        -> Init {
  return (op(args..., init));
}

/// @brief short for calculate
/// @brief Computes the sum of the given value init
/// and a initializer list. uses the given binary function op
/// @param init 	initial value of the sum
/// @param op function op
/// @param args an initializer list
/// return A
template <class A, class T, class Op>
[[nodiscard]] constexpr auto calcu(A &&init, Op op,
                                   std::initializer_list<T> &&args) -> A {
  for (auto &&i : My_Iota(args.begin(), args.end())) {
    init = op(init, i);
  }
  return init;
}

/// @brief checks if the integral number is negative or not
/// @param val
/// @return int
template <class Ty> constexpr is_int auto is_neg(Ty val) noexcept {
  return (val < 0) ? true : false;
}

/// @brief checks if the integral number is negative or not
/// @param val
/// @return int
/// @note special case
template <>
inline constexpr is_int auto is_neg<std::int32_t>(std::int32_t val) noexcept {
  return (val ^ +0) < +0;
}

/// @brief checks if the floating point number has a sign-bit
/// @param val
/// @return bool
template <class Ty>
requires is_float<Ty>
constexpr auto sign(const Ty val) noexcept -> bool {
  const double l_val{std::move(static_cast<double>(val))};
  return (
      bit_cast<std::uint64_t>(l_val) >>
          (sizeof(std::uint64_t) * std::numeric_limits<unsigned char>::digits -
           1) ==
      1);
}

/// @brief returns ln of `x`
/// @param val
/// @param src works if `Apology` macro is defined
/// @return Ty
template <class Ty> [[nodiscard]] constexpr is_float auto ln(Ty val) {
  if (equal(val, static_cast<Ty>(1))) {
    return static_cast<Ty>(0.);
  }
  if (less_or_equal(val, static_cast<Ty>(0.))) {
    return std::numeric_limits<Ty>::quiet_NaN();
  }
  if (greater_or_equal(val, static_cast<Ty>(2.))) {
    constexpr Ty ln_2{static_cast<Ty>(0.69314718056)};
    return ln(val / static_cast<Ty>(2.)) + ln_2;
  }
  if (greater_or_equal(val, static_cast<Ty>(1024.))) {
    constexpr Ty ln_1024{static_cast<Ty>(6.9314718056)};
    return ln(val / static_cast<Ty>(1024.)) + ln_1024;
  }
  val -= 1;
  Ty res{};
  Ty x_pow{val};
  for (std::size_t i{1}; i < 100UL; ++i) {
    if (i & 1UL) {
      res += x_pow / static_cast<Ty>(i);
    } else {
      res -= x_pow / static_cast<Ty>(i);
    }
    x_pow *= val;
  }
  return res;
}

/// @brief calculates `floor` of float values
/// @param val
/// @return Ty
template <class Ty> [[nodiscard]] constexpr is_float auto floor(Ty val) {
  constexpr Ty max_llong{
      static_cast<Ty>(std::numeric_limits<long long>::max())};
  constexpr Ty min_llong{
      static_cast<Ty>(std::numeric_limits<long long>::min())};
  if (greater_or_equal(val, max_llong) || less_or_equal(val, min_llong) ||
      equal(val, static_cast<Ty>(0.))) {
    return val;
  }
  const auto temp_n{static_cast<std::int64_t>(val)};
  const Ty temp_d{static_cast<Ty>(temp_n)};
  if (equal(temp_d, val) || greater_or_equal(val, static_cast<Ty>(0.))) {
    return temp_d;
  }
  return (temp_d) - static_cast<Ty>(1.);
}

/// @brief stores division of an `element` of type `Ty`
/// @param num numerator
/// @param denom denominator
/// @param src works if `Apology` macro is defined
/// @return Division<Num, Denom>
template <class Num, class Denom>
[[nodiscard]] constexpr auto div(Num num, Denom denom) -> Division<Num, Denom> {
  Division<Num, Denom> res{};
  res.Quot = num / denom;
  res.Rem = num - (res.Quot * denom);
  return res;
}

/// @brief gives ceil of a float value
/// @param val
/// @return Ty
template <class Ty> [[nodiscard]] constexpr is_float auto ceil(Ty val) {
  const auto temp{static_cast<std::int64_t>(val)};
  if (sign(val) || equal(val, static_cast<Ty>(temp))) {
    return static_cast<Ty>(temp);
  }
  return static_cast<Ty>(temp + 1);
}

/// @brief rounds float numbers
template <class Ty> [[nodiscard]] constexpr is_float auto round(Ty val) {
  return (val > static_cast<Ty>(0.)) ? ceil(val - static_cast<Ty>(.5))
                                     : floor(val + static_cast<Ty>(.5));
}

/// @brief gives the sqrt of a integral or a a float number
/// @param src works if `Apology` macro is defined
/// @param times number of iteration you want to be used by the newton method
/// @return Ty
template <class Ty>
[[nodiscard]] constexpr real_num auto sqrt(const Ty val,
                                           const std::size_t times = 20) {
  return static_cast<Ty>(kraken::num_methods::newton(
      1., times, [&val](auto &&x) { return (x * x) - val; },
      [](auto &&y) { return (static_cast<Ty>(2.) * y); }));
}

/// @brief returns a*a
/// @param a
/// @return T
template <class T> [[nodiscard]] constexpr auto sqr(const T a) noexcept -> T {
  return a * a;
}

/// @brief get the number of elements after the decimal point
/// @param val
/// @return std::uint64_t
template <class Ty>
requires is_float<Ty>
[[nodiscard]] constexpr auto decimal_places(Ty val) -> std::uint64_t {
  std::uint64_t dec_count{};
  val = abs(val);
  auto temp{val - floor(val)};
  Ty factor{10};
  auto eps{std::numeric_limits<Ty>::epsilon() * temp};
  while ((greater_than(temp, eps) &&
          less_than(temp, (static_cast<Ty>(1.) - eps))) &&
         dec_count < std::numeric_limits<Ty>::max_digits10) {
    temp = val * factor;
    temp -= floor(temp);
    factor *= 10;
    eps = std::numeric_limits<Ty>::epsilon() * val * factor;
    ++dec_count;
  }
  return dec_count;
}

/// @brief chops of the fractional part of any floating-point number
///        and returns that remaining value
/// @param val the value you want to truncate
/// @return std::uint64_t
template <class Ty>
requires is_float<Ty>
[[nodiscard]] constexpr auto trunc(Ty val) -> std::uint64_t {
  // check if float fits into integer
  if (std::numeric_limits<std::uint64_t>::digits <
      std::numeric_limits<Ty>::digits) {
    // check if float is smaller than max std::uint64_t
    if ((val < static_cast<Ty>(std::numeric_limits<std::uint64_t>::max())) &&
        (val > static_cast<Ty>(std::numeric_limits<std::uint64_t>::min()))) {
      return static_cast<std::uint64_t>(val); // safe to cast
    }
    return std::numeric_limits<std::uint64_t>::max();
  }
  // It is safe to cast
  return static_cast<std::uint64_t>(val);
}

/// @brief returns the fractional portion of a scalar
/// @param val
template <class Ty> [[nodiscard]] constexpr is_float auto frac(Ty val) {
  val = abs(val);
  return val - static_cast<Ty>(trunc(val)); // 3.14 - 3 = .14
}

/// @brief returns the remainder of x/y with the same sign as x
/// @param a
/// @param b
template <class Ty>
[[nodiscard]] constexpr is_float auto fmod(const Ty a, const Ty b) {
  const Ty t{frac(abs(a) / abs(b)) * abs(b)};
  return (less_than(a, static_cast<Ty>(0.)) ? -t : t);
}

/// @brief computes base to power
/// @param base
/// @param power
/// @param eps
template <class Ty, class P>
[[nodiscard]] constexpr auto
pow(Ty base, P power, const Ty eps = std::numeric_limits<Ty>::epsilon()) -> Ty {
  if constexpr (both_integral<Ty, P>) {
    if (power > 0) {
      Ty res{static_cast<Ty>(1)};
      Ty base_copy{std::move(base)};
      for (;;) {
        if (power & 1) {
          res *= base_copy;
        }
        power >>= 1;
        if (!power) {
          break;
        }
        base_copy *= base_copy;
      }
      return res;
    }
    Ty res{static_cast<Ty>(1)};
    Ty base_copy{std::move(base)};
    power *= -1;
    for (;;) {
      if (power & 1) {
        res *= base_copy;
      }
      power >>= 1;
      if (!power) {
        break;
      }
      base_copy *= base_copy;
    }
    return static_cast<Ty>(1.) / res;
  } else {
    if (power < 0) {
      return static_cast<Ty>(1.) / pow(base, -power, eps);
    }
    if (power >= 10) {
      return sqr(
          pow(base, power / static_cast<Ty>(2.), eps / static_cast<Ty>(2.)));
    }
    if (power >= 1) {
      return base * pow(base, power - static_cast<Ty>(1.), eps);
    }
    if (eps >= static_cast<Ty>(1.)) {
      return sqrt(base);
    }
    return sqrt(
        pow(base, power * static_cast<Ty>(2.), eps * static_cast<Ty>(2.)));
  }
}

/// @brief  returns the square root of the sum of squares of its arguments
/// @param x
/// @param y
/// @param z
/// @return Ty
template <class Ty>
[[nodiscard]] constexpr real_num auto hypot(Ty x, Ty y, Ty z) {
  return sqrt((x * x) + (y * y) + (z * z));
}

///@brief  returns the square root of the sum of squares of its arguments
/// @param x
/// @param y
/// @return Ty
template <class Ty> [[nodiscard]] constexpr real_num auto hypot(Ty x, Ty y) {
  return sqrt((x * x) + (y * y));
}

///@brief  returns the square root of the sum of squares of its arguments
/// @param args provide an initializer list
template <class Ty>
[[nodiscard]] constexpr real_num auto
hypot(const std::initializer_list<Ty> &args) {
  Ty sum{static_cast<Ty>(1.)};
  for (const auto i : args) {
    sum += (i * i);
  }
  return sqrt(sum);
}

/// @brief returns log base2 of `x`
/// @param val the value you want yo get it's log2
/// @param src works if `Apology` macro is defined
/// @return Ty
template <class Ty> [[nodiscard]] constexpr auto log2(const Ty val) -> Ty {
  return ln(val) * constants::log2e_v<Ty>;
}

/// @brief returns log base2 of `x`
/// @param val the value you want yo get it's log2
/// @param src works if `Apology` macro is defined
/// @return Ty
/// @note special case
template <>
[[nodiscard]] constexpr auto log2<std::int32_t>(const std::int32_t val)
    -> std::int32_t {
  constexpr std::size_t sysbits{
      (std::numeric_limits<unsigned char>::digits * sizeof(void *))};
  return (static_cast<std::int32_t>(
      (sysbits - 1U) - static_cast<std::size_t>(
                           std::countl_zero(static_cast<std::size_t>(val)))));
}

/// @brief returns log base10 of `x`
/// @param val the value you want yo get it's log10
/// @param src works if `Apology` macro is defined
/// @return Ty
template <class Ty> [[nodiscard]] constexpr auto log10(const Ty val) -> Ty {
  return ln(val) * constants::log10e_v<Ty>;
}

/// @brief returns log base10 of `x`
/// @param val the value you want yo get it's log10
/// @param src works if `Apology` macro is defined
/// @return Ty
/// @note special case
template <>
[[nodiscard]] constexpr inline auto log10<std::int32_t>(const std::int32_t val)
    -> std::int32_t {
  return log2(val) / log2(10);
}

/// @brief computes the greatest common divisor
/// @param a
/// @param b
/// @return A
template <class A, class B>
requires(both_integral<A, B>)
    [[nodiscard]] constexpr auto gcd(A a, B b) noexcept -> A {
  int shift{};
  if (a == 0) {
    return b;
  }
  if (b == 0) {
    return a;
  }
  shift = std::countr_zero(static_cast<std::uint64_t>(a | b));
  a >>= std::countr_zero(static_cast<std::uint64_t>(a));
  do {
    b >>= std::countr_zero(static_cast<std::uint64_t>(b));
    if (a > b) {
      std::swap(a, b);
    }
    b -= a;
  } while (b != 0);
  return a << shift;
}

/// @brief computes the least common multiples
/// @param a
/// @param b
/// @return A
template <class A, class B>
requires(both_integral<A, B>)
    [[nodiscard]] constexpr auto lcm(A a, B b) noexcept -> A {
  return (a != 0 && b != 0) ? (a / gcd(a, b)) * b : 0;
}

/// @brief gives `nth` `fibonacci` number
/// @param val `nth` value
/// @param src works if `Apology` macro is defined
/// @return Ty
template <class Ty>
requires is_int<Ty>
[[nodiscard]] constexpr auto fibonacci(const Ty val) -> std::size_t {
  if (val == 0) {
    return 0UL;
  }
  if (val == 1) {
    return 1UL;
  }
  //
  const double sqrt_5{2.23606797749978969};
  return static_cast<std::size_t>(
      round(pow(constants::phi, static_cast<double>(val)) / sqrt_5));
}

/// @brief give the factorial of a number
/// @param val that number
/// @param src works if `Apology` macro is defined
/// @return Ty
[[nodiscard]] constexpr auto factorial(std::size_t val) -> std::size_t {
  if (val <= 1) {
    return 1;
  }
  return val * factorial(val - 1);
}

/// @brief checks if the number is prime
/// @return bool
template <class Ty>
requires is_int<Ty>
[[nodiscard]] constexpr auto is_prime(const Ty num) noexcept -> bool {
  if (num <= 1) {
    return false;
  } // ... - 1 //
  if (num <= 3) {
    return true;
  } // 2 - 3 //
  if (num % 2 == 0 || num % 3 == 0) {
    return false;
  } // multiples of 2 or 3
  for (Ty i{5}; (i * i) < num; i += 6) {
    if (num % i == 0 || num % (i + 2) == 0) {
      return false;
    }
  }
  return true;
}

/// @brief converts degree to radian
/// @return Ty
template <class Ty>
[[nodiscard]] constexpr is_float auto to_radian(const Ty degree) noexcept {
  return degree * constants::pi_v<Ty> / static_cast<Ty>(180.);
}

/// @brief converts radian to degree
/// @return Ty
template <class Ty>
requires is_float<Ty>
[[nodiscard]] constexpr auto to_degree(const Ty radian) noexcept -> Ty {
  return radian * static_cast<Ty>(180.) / constants::pi_v<Ty>;
}

/// @brief gets the sine value of an angle
/// @param angle (IN DEGREE) the angle that you want to find its Sine value
/// @return Ty
template <class Ty>
[[nodiscard]] constexpr is_float auto sin(Ty angle) noexcept {
  angle = to_radian(angle);
  const Ty b{static_cast<Ty>(4.) / constants::pi_v<Ty>};
  const Ty c{static_cast<Ty>(-4.) /
             (constants::pi_v<Ty> * constants::pi_v<Ty>)};
  const Ty p{static_cast<Ty>(.225)};
  const Ty temp{b * angle +
                c * angle *
                    (less_than(angle, static_cast<Ty>(0.)) ? -angle : angle)};
  return p * (temp * (less_than(temp, static_cast<Ty>(0.)) ? -temp : temp) -
              temp) +
         temp;
}

/// @brief gets the cosine value of an angle
/// @param angle (IN DEGREE) the angle that you want to find its Cosine value
/// @link thanks to: https://stackoverflow.com/a/28050328
/// @return Ty
template <class Ty>
[[nodiscard]] constexpr is_float auto cos(Ty angle) noexcept {
  angle = to_radian(angle);
  const Ty tp = static_cast<Ty>(1.) / (2. * constants::pi_v<Ty>);
  angle *= tp;
  angle -= static_cast<Ty>(.25) + floor(angle + static_cast<Ty>(.25));
  angle *= static_cast<Ty>(16.) * (abs(angle) - static_cast<Ty>(.5));
  angle += static_cast<Ty>(.225) * angle * (abs(angle) - static_cast<Ty>(1.));
  return angle;
}

/// @brief gets the Tangent value of an angle
/// @param angle (IN DEGREE)the angle that you want to find its Tangent value
/// @return Ty
template <class Ty>
[[nodiscard]] constexpr is_float auto tan(const Ty angle) noexcept {
  return sin(angle) / cos(angle);
}

/// @brief gets the cosecant value of an angle
/// @param angle (IN DEGREE) the angle that you want to find its cosecant value
/// @return Ty
template <class Ty>
[[nodiscard]] constexpr is_float auto cosc(Ty angle) noexcept {
  return static_cast<Ty>(1.) / sin(angle);
}

/// @brief gets the secant value of an angle
/// @param angle (IN DEGREE) the angle that you want to find its secant value
/// @return Ty
template <class Ty>
[[nodiscard]] constexpr is_float auto sec(Ty angle) noexcept {
  return static_cast<Ty>(1.) / cos(angle);
}

/// @brief gets the cotan value of an angle
/// @param angle (IN DEGREE) the angle that you want to find its cotan value
/// @return Ty
template <class Ty>
[[nodiscard]] constexpr is_float auto cot(Ty angle) noexcept {
  return static_cast<Ty>(1.) / tan(angle);
}

/// @brief gets the arc cosine value of an angle
/// @param angle (IN DEGREE) the angle that you want to find its arc cosine
/// value
/// @link https://stackoverflow.com/a/36387954/13060681
/// @note acos(x) ≈ π/2 + (ax + bx³) / (1 + cx² + dx⁴)
/// @note range [0,pi], expecting x to be in the range [-1,+1].
/// @return Ty
template <class Ty>
[[nodiscard]] constexpr is_float auto arc_cos(Ty angle) noexcept {
  const Ty a{static_cast<Ty>(-0.939115566365855)};
  const Ty b{static_cast<Ty>(0.9217841528914573)};
  const Ty c{static_cast<Ty>(-1.2845906244690837)};
  const Ty d{static_cast<Ty>(0.295624144969963174)};
  return constants::pi_v<Ty> / static_cast<Ty>(2.) +
         (a * angle + b * angle * angle * angle) /
             (static_cast<Ty>(1.) + c * angle * angle +
              d * angle * angle * angle * angle);
}

/// @brief gets the arc sine value of an angle
/// @param angle (IN DEGREE) the angle that you want to find its arc sine value
/// @note Handbook of Mathematical Functions, M. Abramowitz and I.A. Stegun, Ed.
/// @return Ty
template <class Ty>
[[nodiscard]] constexpr is_float auto arc_sin(Ty angle) noexcept {
  const Ty negate{Ty(less_than(angle, static_cast<Ty>(0.)))};
  angle = abs(angle);
  Ty res{static_cast<Ty>(-0.0187293)};
  res *= angle;
  res += static_cast<Ty>(0.0742610);
  res *= angle;
  res -= static_cast<Ty>(0.2121144);
  res *= angle;
  res += static_cast<Ty>(1.5707288);
  res = constants::pi_v<Ty> * static_cast<Ty>(0.5) -
        sqrt(static_cast<Ty>(1.0) - angle) * res;
  return res - 2 * negate * res;
}

/// @brief gets the arc sine value of an angle
/// @param angle (IN DEGREE) the angle that you want to find its arc sine value
/// @return Ty
template <class Ty>
[[nodiscard]] constexpr is_float auto arc_tan(Ty angle) noexcept {
  const Ty double_angle{angle * angle};
  const Ty A{static_cast<Ty>(0.0776509570923569)};
  const Ty B{static_cast<Ty>(-0.287434475393028)};
  const Ty C{(constants::pi_v<Ty> / static_cast<Ty>(4.)) - A - B};
  return ((A * double_angle + B) * double_angle + C) * angle;
}

/// @brief returns a pair of the integral and the fractional part of a
/// floating-point value
/// @param x the float value
/// @return Ty
template <class Ty>
requires is_float<Ty>
[[nodiscard]] constexpr auto modf(const Ty x) noexcept {
  return std::pair(trunc(x), frac(x));
}

/// @brief computes the 1's complement of a number
/// @param val the number you want to get it's 1's complement
/// @tparam n_bits number of bits required
/// @return std::bitset<n_bits>
template <const std::size_t n_bits>
[[nodiscard]] constexpr auto ones_complement(const std::size_t val) {
  const std::bitset<n_bits> b_set{val};
  return (~b_set);
}

/// @brief computes the 2's complement of a number
/// @param val the number you want to get it's 2's complement
/// @tparam n_bits number of bits required
/// @return std::bitset<n_bits>
template <const std::size_t n_bits>
[[nodiscard]] constexpr auto twos_complement(const std::size_t val) {
  const std::bitset<n_bits> b{1ULL};
  return (ones_complement<n_bits>(val) | b);
}

/// @brief computes the 1's complement of a number
/// @param val the number you want to get it's 1's complement
/// @param n_bits number of bits required
/// @return std::size_t
[[nodiscard]] constexpr auto ones_complement(const std::size_t val,
                                             const std::size_t n_bits) noexcept
    -> std::size_t {
  return ((1ULL << n_bits) - 1ULL) ^ val;
}

/// @brief computes the 2's complement of a number
/// @param val the number you want to get it's 2's complement
/// @param n_bits number of bits required
/// @return std::size_t
[[nodiscard]] constexpr auto twos_complement(const std::size_t val,
                                             const std::size_t n_bits) noexcept
    -> std::size_t {
  return ones_complement(val, n_bits) + 1ULL;
}

/// @brief Computes e (Euler's number, 2.7182818...) raised to the given power
/// arg
/// @param power value of floating-point
/// @return Ty
template <class Ty> [[nodiscard]] constexpr is_float auto exp(const Ty power) {
  return pow(constants::e_v<Ty>, power);
}

/// @brief Computes the gamma function of arg
/// @param arg value of a floating-point
/// @return Ty
/// @link https://rosettacode.org/wiki/Gamma_function
template <class Ty>
[[nodiscard]] constexpr is_float auto gamma(const Ty arg) noexcept {
  const Ty a{12};
  std::array<Ty, 12> c{};
  Ty k_factrl{static_cast<Ty>(1.)};
  c[0] = sqrt(static_cast<Ty>(2.0) * constants::pi_v<Ty>);
  for (Ty k{static_cast<Ty>(1)}; k < a; ++k) {
    c[static_cast<std::size_t>(k)] =
        exp(a - k) * pow(a - k, k - static_cast<Ty>(0.5)) / k_factrl;
    k_factrl *= -k;
  }
  Ty accumulate{c[0]};
  for (Ty k{static_cast<Ty>(1)}; k < a; ++k) {
    accumulate += c[static_cast<std::size_t>(k)] / (arg + k);
  }
  accumulate *= exp(-(arg + a)) * pow(arg + a, arg + static_cast<Ty>(0.5));
  return accumulate / arg;
}

/// @brief computes beta function on (a, b)
/// @param a
/// @param b
/// @return Ty
template <class Ty>
[[nodiscard]] constexpr is_float auto beta(const Ty x, const Ty y) noexcept {
  return (gamma(x) * gamma(y)) / gamma(x + y);
}

/// @brief Finds the `Median` of a given container
/// @param container some container
/// @return Cont::value_type
template <class Cont>
[[nodiscard]] constexpr auto median(Cont container) ->
    typename Cont::value_type {
  if (!container.size()) {
    throw 0;
  }
  const auto n{container.size() / 2};
  std::nth_element(container.begin(), container.begin() + n, container.end());
  const auto med{container[n]};
  if (!(container.size() & 1)) {
    return (*max_range(container.begin(), container.begin() + n) + med) / 2;
  }
  return med;
}
} // namespace kraken::cal

#endif // NUMERIC_HPP
