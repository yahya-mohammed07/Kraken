#ifndef MAX_MIN_HPP
#define MAX_MIN_HPP

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

namespace kraken::cal {

  /**
   * @brief calculate `max` with two numbers
   * @param  a  A thing of arbitrary type.
   * @param  b  Another thing of arbitrary type.
   * @return Ty
   */
  template<class T>
  [[nodiscard]]
  inline constexpr
  auto max(const T a, const T b)
      -> const T
  {
    return a > b ? a : b;
  }

  /**
   * @brief calculate `min` with two numbers
   * @param  a  A thing of arbitrary type.
   * @param  b  Another thing of arbitrary type.
   * @return Ty
   */
  template<class T>
  [[nodiscard]]
  inline constexpr
  auto min(const T a, const T b)
      -> const T
  {
    return a > b ? b : a;
  }

}

#endif // MAX_MIN_HPP