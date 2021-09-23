#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <algorithm>          // std::swap
#include <array>              // std::array
#include <initializer_list>
#include <cstdio>             // std::puts, std::printf
#include <iostream>           // std::cout
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

template <class T, std::size_t ROW, std::size_t COL>
  requires (!std::is_class_v<T> && !std::is_same_v<T, char>)
class matrix_
{
  static_assert(ROW > 0 && COL > 0, "ROW-COL must be greater than `0`");

private:
  std::array<T, (ROW*COL)>  m_data{};

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
  constexpr matrix_(const std::initializer_list<T> &init) noexcept
  {
    for (std::size_t j {0} ; const auto &i : init) {
      m_data.at(j) = std::move(i);
      ++i;
    }
  }

  /// @brief init-list constructor
  constexpr matrix_(std::initializer_list<T> &&init) noexcept
  {
    for (std::size_t j {0} ; auto &&i : init) {
      m_data.at(j) = i;
      ++j;
    }
  }

  /// @brief variadic constructor
  consteval matrix_( auto ...args )
  {
    m_data = {std::move(args)...};
  }

  /// @brief read-only
  [[nodiscard]] constexpr
  auto begin() const noexcept   { return m_data.begin(); }
  [[nodiscard]] constexpr
  auto end() const noexcept     { return m_data.end();   }
  [[nodiscard]] constexpr
  auto rbegin() const noexcept   { return m_data.rbegin(); }
  [[nodiscard]] constexpr
  auto rend() const noexcept     { return m_data.rend();   }
  /// @brief modifiable
  [[nodiscard]] constexpr
  auto begin() noexcept   { return m_data.begin(); }
  [[nodiscard]] constexpr
  auto end() noexcept     { return m_data.end();   }
  [[nodiscard]] constexpr
  auto rbegin() noexcept   { return m_data.rbegin(); }
  [[nodiscard]] constexpr
  auto rend() noexcept     { return m_data.rend();   }
  /// @methods:

  /// @get: column
  [[nodiscard]] constexpr
  auto col() const
      -> std::size_t
  {
    return COL;
  }

  /// @ge: row
  [[nodiscard]] constexpr
  auto row() const
      -> std::size_t
  {
    return ROW;
  }

  [[nodiscard]] constexpr
  auto row_col() const
      -> row_col
  {
    return {ROW, COL};
  }

  [[nodiscard]] constexpr
  auto size() const
    -> std::size_t
  {
    return ROW*COL;
  }

  /**
   * @brief yes
   * @return bool
   */
  [[nodiscard]] constexpr
  auto empty() const
    -> bool
  {
    return ROW*COL == 0ul;
  }

  /**
   * @brief get/change element at given row-col
   * @tparam row row-number
   * @tparam col column-number
   * @return T&
   */
  template<const std::size_t row, std::size_t col>
  [[nodiscard]] constexpr
  auto at()
      -> auto &
  {
    static_assert( (row >= 0 && col >= 0) && (row < ROW && col < COL) );
    return m_data[(row * COL) + col];
  }

  /**
   * @brief get/change element at given row-col
   * @tparam row row-number
   * @tparam col column-number
   * @return T
   */
  template<const std::size_t row, std::size_t col>
  [[nodiscard]] constexpr
  auto at() const
      -> auto
  {
    static_assert( (row >= 0 && col >= 0) && (row < ROW && col < COL) );
    return m_data[(row * COL) + col];
  }
  /**
   * @brief get/change element at given row-col
   * @param row row-number
   * @param col column-number
   * @return T&
   */
  [[nodiscard]] constexpr
  auto at(const std::size_t &row, const std::size_t &col)
      -> auto &
  {
    assert( (row >= 0 && col >= 0) && (row < ROW && col < COL) );
    return m_data[(row * COL) + col];
  }

  /**
   * @brief get/change element at given row-col
   * @param row row-number
   * @param col column-number
   * @return T
   */
  [[nodiscard]] constexpr
  auto at(const std::size_t &row, const std::size_t &col) const
      -> auto
  {
    assert( (row >= 0 && col >= 0) && (row < ROW && col < COL) );
    return m_data[(row * COL) + col];
  }

  /**
   * @brief print matrix items, prints with %.11f if T is float
   * @return nothing
   */
  constexpr
  auto print() const
      -> void
  {
    for (std::size_t count{}; auto &&i : *this) {
      if (count == COL) { std::printf("%c", '\n'); count={}; }
      if constexpr ( std::is_floating_point_v<T> ) {
        std::printf("%.11f ", i);
      }
      else std::cout << i << ' ';
      ++count;
    }
  }
  /**
   * @brief swaps row in given matrix
   * @param start which row
   * @param with change with row
   * @return nothing
   */
  constexpr
  auto swap_rows(const std::size_t &start, const std::size_t &with)
      -> void
  {
    for (std::size_t i {0}; i < ROW; ++i)
      std::swap(at(start, i), at(with, i));
  }
  /**
   * @brief swaps col in given matrix
   * @param start which col
   * @param with change with col
   */
  constexpr
  auto swap_cols(const std::size_t &start, const std::size_t &with)
      -> void
  {
    for (std::size_t i {0}; i < COL; ++i)
      std::swap(at(i, start), at(i, with));
  }

  /**
   * @brief sorts using insertion-sort if and only-if the size was less than 256
   * else it will sort using std::sort
   * @param order bool value -true ascending -false descending
   * @return nothing
   */
  constexpr
  auto sort(const bool order = true)
      -> void
  {
    if ( size() < 256ul ) { // insertion-sort
      if ( order ) {
        for ( std::size_t i {0}; i < size(); ++i) {
          std::size_t j {i};
          while ( j != 0 && m_data[j-1] > m_data[j] ) {
            std::swap(m_data[j-1] , m_data[j]);
            --j;
          }
        }
        return;
      }
      for ( std::size_t i {0}; i < size(); ++i) {
        std::size_t j {i};
        while ( j != 0 && m_data[j-1] < m_data[j] ) {
          std::swap(m_data[j-1] , m_data[j]);
          --j;
        }
      }
      return;
    }
    if ( order ) { std::sort( begin(), end() ); return; }
    std::sort( begin(), end(), std::greater<T>{} );
  }

  /**
   * @brief changes its rows into columns and its columns into rows
   * @return nothing
   */
  constexpr
  auto transpose_squared() -> void
  {
    for (std::size_t i {0}; i < ROW-1; ++i) {
      for (std::size_t j {i+1}; j < COL; ++j) {
        std::swap( at(i,j), at(j,i) );
      }
    }
  }

  /**
   * @brief changes its rows into columns and its columns into rows
   * @return a tmatrix
   */
  [[nodiscard]] constexpr
  auto transpose_triangular() const -> auto
  {
    matrix_<T, COL, ROW> temp;
    for ( std::size_t i {0}; i < ROW; ++i ) {
      for ( std::size_t j {0}; j < COL; ++j ) {
        temp[(ROW*j)+i] = std::move(m_data[(COL*i)+j]);
      }
    }
    return temp;
  }
  /**
   * @brief fills the container with a certain value
   * @return nothing
   */
  constexpr
  auto fill( T&& value ) -> void
  {
    for ( auto &&i : m_data ) {
      i = std::move(value);
    }
  }

  /**
   * @brief rotate the matrix clock-wise or anti-clock-wise
   * default is clock-wise, put false for anti-clock-wise
   * @tparam row - new row after transposing
   * @tparam col - new column after transposing
   * @tparam clock_wise - default = true
   * @return nothing
   */
  template<const std::size_t row, const std::size_t col
                      ,const bool clock_wise = true>
  constexpr
  auto rotate_triangular()
    -> void
  {
    if constexpr ( clock_wise ) {
      for ( std::size_t i {0}; i < col; ++i ) {
        std::size_t start {0};
        std::size_t end   {row-1};
        while ( start < end ) {
          std::swap( at(start, i), at(end, i) );
          ++start;
          --end;
        }
      }
      return;
    }
    for ( std::size_t i {0}; i < row; ++i ) {
      std::size_t start {0};
      std::size_t end   {col-1};
      while ( start < end ) {
        std::swap( at(i, start), at(i, end) );
        ++start;
        --end;
      }
    }
  }

  /**
   * @brief rotate the matrix clock-wise or anti-clock-wise
   * default is clock-wise, put false for anti-clock-wise
   * @tparam clock_wise - default = true
   * @return nothing
   */
  template<const bool clock_wise = true>
  constexpr
  auto rotate_squared()
    -> void
  {
    if constexpr ( clock_wise ) {
      std::size_t k {0};
      for ( std::size_t i {0}; i < ROW; ++i ) {
        k = ROW-1;
        for ( std::size_t j {0}; j < k; ++j ) {
          std::swap( at(i, j), at(i, k) );
          --k;
        }
      }
      return;
    }
    std::size_t k {0};
    for ( std::size_t i {0}; i < ROW; ++i ) {
      k = ROW-1;
      for ( std::size_t j {0}; j < k; ++j ) {
        std::swap( at(j, i), at(k, i) );
        --k;
      }
    }
    return;
  }

/// @operators: on matrices
  /**
   * @brief assigns two matrices
   * @return matrix
   */
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

  /**
   * @brief adds two matrix containers
   * @return matrix
   */
  constexpr matrix_& operator+(const matrix_ &other) noexcept
  {
    for ( std::size_t j{} ; const auto& i : other ) {
      m_data[j] += i;
      ++j;
    }
    return *this;
  }

  /**
   * @brief adds two matrix containers
   * @return matrix
   */
  constexpr matrix_& operator+(matrix_ &&other) noexcept
  {
    for ( std::size_t j{} ; auto&& i : other ) {
      m_data[j] += i;
      ++j;
    }
    return *this;
  }

  /**
   * @brief adds a matrix containers with a scalar
   * @return matrix
   */
  constexpr matrix_& operator+(const T &val) noexcept
  {
    for ( std::size_t j{} ; const auto& i : *this ) {
      m_data[j] += val;
      ++j;
    }
    return *this;
  }

  /**
   * @brief adds a matrix containers with a scalar
   * @return matrix
   */
  constexpr matrix_& operator+(T &&val) noexcept
  {
    for ( std::size_t j{} ; auto &&i : *this ) {
      m_data[j] += val;
      ++j;
    }
    return *this;
  }

  /**
   * @brief multiplies a matrix containers with a `scalar`
   * @return matrix
   */
  [[nodiscard]] constexpr
  matrix_& operator*(const T &val)  noexcept
  {
    for ( std::size_t j{} ; const auto& i : *this ) {
      m_data[j] *= val;
      ++j;
    }
    return *this;
  }

  /**
   * @brief multiplies a matrix containers with a `scalar`
   * @return matrix
   */
  [[nodiscard]] constexpr
  matrix_& operator*( T &&val)  noexcept
  {
    for ( std::size_t j{} ; auto &&i : *this ) {
      m_data[j] *= val;
      ++j;
    }
    return *this;
  }

  /**
   * @brief multiplies two matrix containers
   * `col` of a must be equal to `row` b
   * @return matrix
   */
  [[nodiscard]] constexpr
  matrix_ operator*(const matrix_ &rhs)  noexcept
  {
    if constexpr ( ROW == 1ull ) {
      for ( std::size_t j{} ; const auto& i : rhs ) {
        m_data[j] *= i;
        ++j;
      }
      return *this;
    }
    matrix_<T , ROW, COL> temp{};
    for (int i {0}; i < ROW; i++)
    {
      for (int j {0}; j < rhs.col(); j++)
      {
        temp.at(i,j) = 0;
        for (int k {0}; k < rhs.row(); k++)
        {
          temp.at(i,j) += (at(i,k) * rhs.at(k,j));
        }
      }
    }
    return temp;
  }

  /**
   * @brief multiplies two matrix collections
   * `col` of a must be equal to `row` b
   * @return matrix
   */
  [[nodiscard]] constexpr
  matrix_ operator*(matrix_ &&rhs) noexcept
  {
    if constexpr ( ROW == 1ull ) {
      for ( std::size_t j{} ; const auto& i : rhs ) {
        m_data[j] *= i;
        ++j;
      }
      return *this;
    }
    matrix_<T , ROW, COL> temp{};
    for (int i {0}; i < ROW; i++)
    {
      for (int j {0}; j < rhs.col(); j++)
      {
        temp.at(i,j) = 0;
        for (int k {0}; k < rhs.row(); k++)
        {
          temp.at(i,j) += (at(i,k) * rhs.at(k,j));
        }
      }
    }
    return temp;
  }


  /**
   * @brief multiplies two matrix containers
   * `col` of a must be equal to `row` b
   * @return matrix
   */
  template<const std::size_t L>
  [[nodiscard]] constexpr
  matrix_<T , ROW, L> operator*(const matrix_<T , COL, L> &rhs) const noexcept
  {
    matrix_<T , ROW, L> temp{};
    for (int i {0}; i < ROW; i++)
    {
      for (int j {0}; j < rhs.col(); j++)
      {
        temp.at(i,j) = 0;
        for (int k {0}; k < rhs.row(); k++)
        {
          temp.at(i,j) += (at(i,k) * rhs.at(k,j));
        }
      }
    }
    return temp;
  }

  /**
   * @brief multiplies two matrix collections
   * `col` of a must be equal to `row` b
   * @return matrix
   */
  template<const std::size_t L>
  [[nodiscard]] constexpr
  matrix_<T , ROW, L> operator*(matrix_<T , COL, L> &&rhs) const noexcept
  {
    matrix_<T , ROW, L> temp{};
    for (int i {0}; i < row(); i++)
    {
      for (int j {0}; j < rhs.col(); j++)
      {
        temp.at(i,j) = 0;
        for (int k {0}; k < rhs.row(); k++)
        {
          temp.at(i,j) += (at(i,k) * rhs.at(k,j));
        }
      }
    }
    return temp;
  }

  /**
   * @brief subtructs two matrix containers
   * @return matrix
   */
  constexpr matrix_& operator-(const matrix_ &other) noexcept
  {
    for ( std::size_t j{} ; const auto& i : other ) {
      m_data[j] -= i;
      ++j;
    }
    return *this;
  }

  /**
   * @brief subtructs two matrix containers
   * @return matrix
   */
  constexpr matrix_& operator-( matrix_ &&other) noexcept
  {
    for ( std::size_t j{} ; auto&& i : other ) {
      m_data[j] -= i;
      ++j;
    }
    return *this;
  }

  /**
   * @brief subtructs a matrix containers with a scalar
   * @return matrix
   */
  constexpr matrix_& operator-(const T &val) noexcept
  {
    for ( std::size_t j{} ; const auto& i : *this ) {
      m_data[j] -= val;
      ++j;
    }
    return *this;
  }

  /**
   * @brief subtructs a matrix containers with a scalar
   * @return matrix
   */
  constexpr matrix_& operator-(T &&val) noexcept
  {
    for ( std::size_t j{} ; auto &&i : *this ) {
      m_data[j] -= val;
      ++j;
    }
    return *this;
  }

  /**
   * @brief element access
   * @brief Subscript access to the data contained in the %matrix
   * @param i The index of the element for which data should be accessed.
   * @return constexpr T&
   */
  constexpr T& operator[](const std::size_t& i) noexcept
  {
    return m_data[i];
  }

  /**
   * @brief element access
   * @brief Subscript access to the data contained in the %matrix
   * @param i The index of the element for which data should be accessed.
   * @return constexpr T&
   */
  constexpr T& operator[](std::size_t&& i) noexcept
  {
    return m_data[i];
  }

  /**
   * @brief element access
   * @brief Subscript access to the data contained in the %matrix
   * @param i The index of the element for which data should be accessed.
   * @return constexpr T&
   */
  constexpr T operator[](const std::size_t& i) const noexcept
  {
    return m_data[i];
  }
  /**
   * @brief element access
   * @brief Subscript access to the data contained in the %matrix
   * @param i The index of the element for which data should be accessed.
   * @return constexpr T&
   */
  constexpr T operator[](std::size_t&& i) const noexcept
  {
    return m_data[i];
  }

  constexpr bool operator!=(const matrix_ &rhs) const noexcept {
    return m_data != rhs.m_data;
  }
  constexpr bool operator==(const matrix_ &rhs) const noexcept {
    return m_data == rhs.m_data;
  }

  constexpr
  friend std::ostream &operator<<(std::ostream &os, const matrix_ & mat) {
    for (std::size_t count{}; auto &&i : mat) {
      if (count == COL) { std::printf("%c", '\n'); count={}; }
      if constexpr ( std::is_floating_point_v<T> ) {
        std::printf("%.11f ", i);
      }
      else os << i << ' ';
      ++count;
    }
    return os;
  }
}; // end of class matrix_

#endif // MATRIX_HPP
