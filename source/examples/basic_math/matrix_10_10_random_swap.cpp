#include "../../source/library/all.hpp"
#include <random>

using std::random_device, std::mt19937, std::uniform_int_distribution;

auto main() -> int {
  matrix_<int, 10, 10> arr;
  const auto &[row, col]{arr.row_col()};
  //
  for (size_t i = 0; i < row; ++i) {
    for (size_t j = 0; j < col; ++j) {
      arr.at(i, j) = static_cast<int>((i + j) % col);
    }
  }
  {
    auto rd = random_device();
    auto rand_bits = mt19937(rd());
    auto index_from = uniform_int_distribution(0ull, row - 1ull);
    //
    for (size_t i = 0; i < row; ++i) {
      arr.swap_rows(i, index_from(rand_bits));
      arr.swap_cols(i, index_from(rand_bits));
    }
  }
  arr.print();
}
