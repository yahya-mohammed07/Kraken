# This file contains helpful notes about `constants.hpp` file

## - What's inside?`

- Math and physics constants as letters

## - Usage:-

```cpp
  std::cout << "Ω= " << constants::omega << '\n';
  std::cout << "e= " << constants::e << '\n';
  std::cout << "ψ= " << constants::sai << '\n';
  std::cout << "π= " << constants::pi << '\n';
  std::cout << "π= " << constants::pi_v<float> << '\n';
  std::cout << "π= " << constants::pi_v<long double> << '\n';
  std::cout << "γ= " << constants::gama << '\n';)
  constexpr auto pi_f = std::move(constants::pi_v<float>);
  std::cout << pi_f << '\n';
  constexpr auto speed_of_light = std::move(constants::c);
```
