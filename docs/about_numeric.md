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
  - `calcu` Is short for `calculate`: It can compute `scalar operations (+,-,*)` on a `init-list` or `variadic` argument
  - `abs`
  - `floor`
  - `ceil`
  - `ln` : log of base `e`
  - `log2`: for `int` values it uses super-fast table lockup, for floating-point it uses: ln(x) * log2(e)
  - `log10`: for `int` values it uses super-fast table lockup, for floating-point it uses: log2(x) / log2(10)
  - `pow`: made using Exponentiation by squaring
  - `powf`: made using sqrt and recursion
  - `pow_container`
  - `hypot`
  - `round`
  - `sqr`
  - `sqrt`: made using `newton's method`
  - `max` : has `5` overloads
  - `min` : has `5` overloads

## Note: for usage please check the `tests` folder

[Alf P. Steinbach]: https://github.com/alf-p-steinbach
