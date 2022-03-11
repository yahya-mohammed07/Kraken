#ifndef ABS_HPP
#define ABS_HPP

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

#include "basic_bit_cast.hpp"
#include "my_concepts.hpp"
#include <cstdint>
#include <limits>

namespace kraken::cal
{
  /// @brief gives the absolute of a value
  /// @param val
  /// @return Ty
  template <class Ty>
  [[nodiscard]]
  constexpr
  auto abs(const Ty val) noexcept
    -> Ty
  {
    return val < static_cast<Ty>(0) ? -(val) : val;
  }

  /// @note special case for ints
  template <>
  [[nodiscard]]
  constexpr inline
  auto abs<std::int32_t>(std::int32_t val) noexcept
    -> std::int32_t
  {
    const std::int32_t mask{
        val >>
        (sizeof(std::int32_t) * std::numeric_limits<unsigned char>::digits - 1)};
    return (val ^ mask) - mask;
  }
} // namespace kraken::cal

#endif // ABS_HPP