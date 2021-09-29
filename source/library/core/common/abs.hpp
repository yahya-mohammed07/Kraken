#ifndef ABS_HPP
#define ABS_HPP

#include <type_traits>

namespace kraken::cal {
  /// @brief gives the absolute of a value
  template<class Ty>
  requires (!std::is_class_v<Ty>)
  [[nodiscard]]
  inline constexpr
  auto abs(const Ty val)
      -> Ty
  {
    if  (val < static_cast<Ty>(0)) return -val;
    else { return val; }
  }
}

#endif // ABS_HPP