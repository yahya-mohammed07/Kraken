#ifndef BASIC_BIT_CAST_HPP
#define BASIC_BIT_CAST_HPP

#include <cstring>
#include <type_traits>
#include <utility>

namespace kraken::cal {
/// @link https://en.cppreference.com/w/cpp/numeric/bit_cast
template <class To, class From>
requires(sizeof(To) == sizeof(From)) &&
    std::is_trivially_copyable_v<From> &&std::is_trivially_copyable_v<
        To> constexpr auto bit_cast(const From &src) noexcept -> To {
  To dst{};
  std::memcpy(&dst, &src, sizeof(To));
  return std::move(dst);
}
} // namespace kraken::cal
#endif // BASIC_BIT_CAST_HPP