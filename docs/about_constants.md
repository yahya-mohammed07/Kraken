# This file contains helpful notes about `constants.hpp` file

## - What's inside?`

- Math and physics constants as symbols

## - Reasons for storing the constants as symbols :-

- I'ts cool!
- Some people find symbols more readable than writing the `full-name` of that symbol e.g: `Gauss–Kuzmin–Wirsing constant` as a symbol : `λ_gauss`. so yeah that's better

## - Usage :-

```cpp
  std::cout << "Ω= " << constants::Ω << '\n';
  std::cout << "e= " << constants::e << '\n';
  std::cout << "ψ= " << constants::ψ << '\n';
  std::cout << "π= " << constants::π << '\n';
  std::cout << "π= " << constants::π_v<float> << '\n';
  std::cout << "π= " << constants::π_v<long double> << '\n';
  std::cout << "γ= " << constants::γ << '\n';)
```

- You should always use `constants::` to get the symbols
  - `constants::ϕ` or `constants::ϕ_v<floating_point_type>`

- note if you did not like the symbols you can always make a text based `variable` like this:

  - ```cpp
    `inside main()`
      constexpr auto pi_f = std::move(constants::π_v<float>);
      std::cout << pi_f << '\n';
    //
    `outside main()`
      inline constexpr auto speed_of_light = std::move(constants::c);

    ```
