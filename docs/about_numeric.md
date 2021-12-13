# This file contains helpful notes about `numeric.hpp` file

## Questions you might ask :-

### - Most of the functions are already exist in `<cmath>` what is the difference?

- In `numeric.hpp` all functions are built in `cpp20` standard and the special guests are `constexpr` and `type_traits` so, some of these functions are actually faster and more accurate! + you get compile time error checks!

### - All the functions are built with `constexpr`?

- Yes!

## What's inside?

- 1- implementation of `newton's method`
- 2- basic `iota` class given by [Alf P. Steinbach][]
- 3- `kraken::cal` namespace `cal` contains functions that do some calculations
- inside `kraken::cal`:
  - `acc`Is short for `accumulate`: It can compute `scalar operations (+,-,*)` of a container with no need of iterators
  - `calcu` Is short for `calculate`: It can compute `scalar operations (+,-,*)` on a `init-list` or `variadic` argument ( not recommended for `subtraction` )
  - `abs`
  - `floor`
  - `ceil`
  - `ln` : log base `e` of x
  - `log2`: for `int` values it uses super-fast `63 or 31 - std::countl-zero(x)`, for floating-point it uses: ln(x) * log2(e)
  - `log10`: for `int` values it uses super-fast `63 or 31 - std::countl-zero(x)`, for floating-point it uses: log2(x) / log2(10)
  - `pow`
  - `pow_container`
  - `hypot`
  - `round`
  - `sqr`
  - `sqrt`: works using `newton's method`
  - `max`
  - `min`
  - `min_max`: gives a pair of `min` and `max`
  - `gcd`
  - `lcm`
  - `trunc`: chops of the fractional part of any floating-point number
  - `decimal_places`: counts the number of elements after the decimal point
  - `fibonacci`
  - `div` gives a pair of `quotient` and `remainder`
  - `factorial` **recursive**
  - `is_neg`
  - `is_prime`
  - `cos`
  - `sin`
  - `tan`
  - `sec`
  - `cosc`
  - `cot`
  - `arc_sin` Handbook of Mathematical Functions, M. Abramowitz and I.A. Stegun, Ed.
  - `arc_cos` range [0,pi], expecting x to be in the range [-1,+1].
  - `arc_tan`
  - `frac`
  - `fmod`
  - `to_radian`
  - `to_degree`

## Note: for usage please check the `tests` folder

[Alf P. Steinbach]: https://github.com/alf-p-steinbach
