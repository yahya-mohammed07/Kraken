#ifndef MATRIX_HPP
#define MATRIX_HPP

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

#include <iomanip>            // std::setw
#include <algorithm>          // std::swap
#include <array>              // std::array
#include <initializer_list>
#include <cassert>            // assert
#include <type_traits>
#include <ostream>            // std::ostream
#include <iterator>
//

struct row_col
{
  std::size_t row{};
  std::size_t col{};
};

template <class Ty, std::size_t ROW, std::size_t COL>
  requires (!std::is_class_v<Ty>)
class matrix_
{
public:
  using value_type              = Ty;
  using pointer                 = value_type*;
  using const_pointer           = const value_type*;
  using reference               = value_type&;
  using const_reference         = const value_type&;
  using iterator                =  value_type*;
  using const_iterator          = const value_type*;
  using size_type               = std::size_t;
  using reverse_iterator        = std::reverse_iterator<iterator>;
  using const_reverse_iterator  = std::reverse_iterator<const_iterator>;
  static_assert(ROW > 0UL && COL > 0UL, "ROW-COL must be greater than `0`");

private:
  std::array<value_type, (ROW*COL)>  m_data{};

public:

  constexpr matrix_() noexcept = default;

  /// @brief copy constructor
  constexpr matrix_(const matrix_ &copy) noexcept
  {
    m_data  = copy.m_data;
  }

  /// @brief move constructor
  constexpr matrix_(matrix_ &&move) noexcept
  {
    m_data  = std::move(move.m_data);
    //
    move.m_data = {};
  }

  /// @brief init-list constructor
  constexpr matrix_(const std::initializer_list<value_type> &init) noexcept
  {
    for (size_type j {0} ; const auto &i : init) {
      m_data.at(j) = std::move(i);
      ++i;
    }
  }

  /// @brief init-list constructor
  constexpr matrix_(std::initializer_list<value_type> &&init) noexcept
  {
    for (size_type j {0} ; auto &&i : init) {
      m_data.at(j) = i;
      ++j;
    }
  }

  /// @brief variadic constructor
  template< class ...T >
  explicit
  consteval matrix_( const T ...args )
  {
    if constexpr ( sizeof...(T) == 1) {
      fill( std::forward<value_type>(args)... );
    }
    else {
      m_data = {std::move(args)...};
    }
  }

  constexpr ~matrix_() = default;

  ///
  [[nodiscard]] constexpr
  const_iterator begin() const noexcept             { return m_data.begin(); }
  [[nodiscard]] constexpr
  const_iterator end() const noexcept               { return m_data.end();   }
  [[nodiscard]] constexpr
  const_reverse_iterator rbegin() const noexcept    { return m_data.rbegin(); }
  [[nodiscard]] constexpr
  const_reverse_iterator rend() const noexcept      { return m_data.rend();   }
  ///
  [[nodiscard]] constexpr
  iterator begin() noexcept                         { return m_data.begin(); }
  [[nodiscard]] constexpr
  iterator end() noexcept                           { return m_data.end();   }
  [[nodiscard]] constexpr
  reverse_iterator rbegin() noexcept                { return m_data.rbegin(); }
  [[nodiscard]] constexpr
  reverse_iterator rend() noexcept                  { return m_data.rend();   }
  /// @methods:

  /// @get: column
  [[nodiscard]] constexpr
  auto col() const
      -> size_type
  {
    return COL;
  }

  /// @get: row
  [[nodiscard]] constexpr
  auto row() const
      -> size_type
  {
    return ROW;
  }

  /// @get: row and column
  [[nodiscard]] constexpr
  auto row_col() const
      -> row_col
  {
    return {ROW, COL};
  }

  [[nodiscard]] constexpr
  auto size() const
    -> size_type
  {
    return ROW*COL;
  }

  /// @brief yes
  /// @return bool
  [[nodiscard]] constexpr
  auto empty() const
    -> bool
  {
    return ROW*COL == 0UL;
  }

  /// @brief get/modify element at given row-col
  /// @tparam row row-number
  /// @tparam col column-number
  /// @return reference
  template<const size_type row, size_type col>
  [[nodiscard]] constexpr
  auto at()
      -> reference
  {
    static_assert( (row >= 0 && col >= 0) && (row < ROW && col < COL) );
    return m_data[(row * COL) + col];
  }

  /// @brief get element at given row-col
  /// @tparam row row-number
  /// @tparam col column-number
  /// @return value_type
  template<const size_type row, size_type col>
  [[nodiscard]] constexpr
  auto at() const
      -> value_type
  {
    static_assert( (row >= 0 && col >= 0) && (row < ROW && col < COL) );
    return m_data[(row * COL) + col];
  }

  /// @brief get/modify element at given row-col
  /// @param row row-number
  /// @param col column-number
  /// @return reference
  [[nodiscard]] constexpr
  auto at(const size_type &row, const size_type &col)
      -> reference
  {
    return m_data[(row * COL) + col];
  }

  /// @brief get element at given row-col
  /// @param row row-number
  /// @param col column-number
  /// @return value_type
  [[nodiscard]] constexpr
  auto at(const size_type &row, const size_type &col) const
      -> value_type
  {
    return m_data[(row * COL) + col];
  }

  /// @brief swaps row in given matrix
  /// @param start which row
  /// @param with change with row
  constexpr
  auto swap_rows(const size_type &start, const size_type &with)
      -> void
  {
    for (size_type i{0}; i < ROW; ++i) {
      std::swap(at(start, i), at(with, i));
    }
  }

  /// @brief swaps column in given matrix
  /// @param start which column
  /// @param with change with column
  constexpr
  auto swap_cols(const size_type &start, const size_type &with)
      -> void
  {
    for (size_type i{0}; i < COL; ++i) {
      std::swap(at(i, start), at(i, with));
    }
  }

  /// @brief sorts using insertion-sort if and only-if the size IS less than 256
  /// else it will sort using std::sort
  /// @param order bool value `true` for ascending and `false` for descending
  /// @return nothing
  constexpr
  auto sort(const bool order = true)
      -> void
  {
    if ( size() < 256UL ) { // insertion-sort
      if ( order ) {
        for ( size_type i {0}; i < size(); ++i) {
          size_type j {i};
          while ( j != 0 && m_data[j-1] > m_data[j] ) {
            std::swap(m_data[j-1] , m_data[j]);
            --j;
          }
        }
        return;
      }
      for ( size_type i {0}; i < size(); ++i) {
        size_type j {i};
        while ( j != 0 && m_data[j-1] < m_data[j] ) {
          std::swap(m_data[j-1] , m_data[j]);
          --j;
        }
      }
      return;
    }
    if ( order ) { std::sort( begin(), end() ); return; }
    std::sort( begin(), end(), std::greater<value_type>{} );
  }

  /// @brief changes its rows into columns and its columns into rows
  /// @return nothing
  constexpr
  auto transpose_squared() -> void
  {
    for (size_type i {0}; i < ROW-1; ++i) {
      for (size_type j {i+1}; j < COL; ++j) {
        std::swap( at(i,j), at(j,i) );
      }
    }
  }

  /// @brief changes its rows into columns and its columns into rows
  /// @return a tmatrix
  [[nodiscard]] constexpr
  auto transpose_triangular() const
  {
    matrix_<value_type, COL, ROW> temp{};
    for ( size_type i {}; i < ROW; ++i ) {
      for ( size_type j {}; j < COL; ++j ) {
        temp[(ROW*j)+i] = std::move(m_data[(COL*i)+j]);
      }
    }
    return temp;
  }

  /// @brief fills the container with a certain value
  /// @return nothing
  constexpr
  auto fill( value_type&& value ) -> void
  {
    for ( auto &&i : m_data ) {
      i = std::move(value);
    }
  }

  /// @brief rotate the matrix clock-wise or anti-clock-wise
  /// default is clock-wise, put false for anti-clock-wise
  /// @tparam row - new row after transposing
  /// @tparam col - new column after transposing
  /// @tparam clock_wise - default = true
  /// @return nothing
  template<const size_type row, const size_type col
                      ,const bool clock_wise = true>
  constexpr
  auto rotate_triangular()
    -> void
  {
    if constexpr ( clock_wise ) {
      for ( size_type i {0}; i < col; ++i ) {
        size_type start {0};
        size_type end   {row-1};
        while ( start < end ) {
          std::swap( at(start, i), at(end, i) );
          ++start;
          --end;
        }
      }
      return;
    }
    for ( size_type i {0}; i < row; ++i ) {
      size_type start {0};
      size_type end   {col-1};
      while ( start < end ) {
        std::swap( at(i, start), at(i, end) );
        ++start;
        --end;
      }
    }
  }

  /// @brief rotate the matrix clock-wise or anti-clock-wise
  /// default is clock-wise, put false for anti-clock-wise
  /// @tparam clock_wise - default = true
  /// @return nothing
  template<const bool clock_wise = true>
  constexpr
  auto rotate_squared()
    -> void
  {
    if constexpr ( clock_wise ) {
      size_type k {0};
      for ( size_type i {0}; i < ROW; ++i ) {
        k = ROW-1;
        for ( size_type j {0}; j < k; ++j ) {
          std::swap( at(i, j), at(i, k) );
          --k;
        }
      }
      return;
    }
    size_type k {0};
    for ( size_type i {0}; i < ROW; ++i ) {
      k = ROW-1;
      for ( size_type j {0}; j < k; ++j ) {
        std::swap( at(j, i), at(k, i) );
        --k;
      }
    }
  }

/// @operators: on matrices
  /// @brief assigns two matrices
  /// @return matrix
  constexpr matrix_& operator=(const matrix_ &other) noexcept
  {
    if (this == &other) {
      return *this;
    }
    m_data = {other.m_data};
    return *this;
  }

  constexpr matrix_& operator=(matrix_ &&other) noexcept
  {
    if (this == &other) {
      return *this;
    }
    m_data = std::move(other.m_data);
    other.m_data = {};
    return *this;
  }

  constexpr matrix_ &operator+=(const matrix_ &rhs) noexcept
  {
    for ( size_type j{} ; const auto& i : rhs ) {
      m_data[j] += i;
      ++j;
    }
    return *this;
  }

  /// @brief adds two matrix containers
  /// @return matrix
  constexpr friend matrix_ operator+(matrix_ lhs ,const matrix_ &rhs) noexcept
  {
    lhs += rhs;
    return lhs;
  }

  constexpr matrix_ &operator+=(const value_type scalar) noexcept
  {
    for ( auto && i : *this ) { i += scalar; }
    return *this;
  }

  /// @brief adds a matrix containers with a scalar
  /// @return matrix
  constexpr friend matrix_ operator+(matrix_ lhs , const value_type scalar) noexcept
  {
    lhs += scalar;
    return lhs;
  }

  /// @brief multiplies a matrix containers with a `scalar`
  /// @return matrix
  [[nodiscard]] constexpr
  matrix_& operator*( value_type val)  noexcept
  {
    for ( auto &&i : *this ) {
      i *= val;
    }
    return *this;
  }

  /// @brief multiplies two matrix containers
  /// `col` of a must be equal to `row` b
  /// @return matrix
  [[nodiscard]] constexpr
  matrix_ operator*(const matrix_ &rhs)  noexcept
  {
    if constexpr ( ROW == 1UL ) {
      for ( size_type j{} ; const auto& i : rhs ) {
        m_data[j] *= i;
        ++j;
      }
      return *this;
    }
    matrix_<value_type , ROW, COL> temp{};
   for (size_type i {0}; i < ROW; ++i) {
      for (size_type k {0}; k < rhs.row(); ++k) {
        for (size_type j {0}; j < COL; ++j) {
          temp.at(i,j) += (at(i,k) * rhs.at(k,j));
        }
      }
    }
    return temp;
  }

  /// @brief multiplies two matrix containers
  /// `col` of a must be equal to `row` b
  /// @return matrix
  template<const size_type L>
  [[nodiscard]] constexpr
  matrix_<value_type , ROW, L> operator*(const matrix_<value_type , COL, L> &rhs) const noexcept
  {
    matrix_<value_type , ROW, L> temp{};
   for (size_type i {0}; i < ROW; ++i) {
      for (size_type k {0}; k < rhs.row(); ++k) {
        for (size_type j {0}; j < L; ++j) {
          temp.at(i,j) += (at(i,k) * rhs.at(k,j));
        }
      }
    }
    return temp;
  }

  constexpr matrix_ &operator-=(const matrix_ &rhs) noexcept
  {
    for ( size_type j{} ; const auto& i : rhs ) {
      m_data[j] -= i;
      ++j;
    }
    return *this;
  }

  /// @brief subtructs two matrix containers
  /// @return matrix
  constexpr friend matrix_ operator-(matrix_ lhs ,const matrix_ &rhs) noexcept
  {
    lhs -= rhs;
    return lhs;
  }

  constexpr matrix_ &operator-=(const value_type scalar) noexcept
  {
    for ( auto && i : *this ) { i -= scalar; }
    return *this;
  }

  /// @brief adds a matrix containers with a scalar
  /// @return matrix
  constexpr friend matrix_ operator-(matrix_ lhs , const value_type scalar) noexcept
  {
    lhs -= scalar;
    return lhs;
  }

  /// @brief get/modify an element in a given index
  /// @brief Subscript access to the data contained in the %matrix
  /// @param i index of the element for which data should be accessed.
  /// @return value_type&
  constexpr value_type& operator[](const size_type i) noexcept
  {
    return m_data[i];
  }

  /// @brief get an element in a given index
  /// @brief Subscript access to the data contained in the %matrix
  /// @param i index of the element for which data should be accessed.
  /// @return value_type
  constexpr value_type operator[](const size_type i) const noexcept
  {
    return m_data[i];
  }

  constexpr bool operator!=(const matrix_ &rhs) const noexcept
  {
    return m_data != rhs.m_data;
  }
  constexpr bool operator==(const matrix_ &rhs) const noexcept
  {
    return m_data == rhs.m_data;
  }

  /// @brief prints data in matrix form
  constexpr
  friend std::ostream &operator<<(std::ostream &os, const matrix_ & mat)
  {
    for (size_type count{}; auto &&i : mat) {
      if (count % COL == 0) { os << '\n'; count=0; }
      if constexpr ( std::is_floating_point_v<value_type> ) {
        os << std::setw(7);
      } else {
        os << i << ' ';
      }
      ++count;
    }
    return os;
  }
}; // end of class matrix_

#endif // MATRIX_HPP
