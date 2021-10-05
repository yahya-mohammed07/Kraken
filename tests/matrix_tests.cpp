#include "../source/library/core/matrix.hpp"
#define CATCH_CONFIG_MAIN
#include "../Catch2/catch.hpp"


inline constexpr
matrix_<float, 3, 3> matrix_test
(
  1.f,2.f,3.f,
  4.f,5.f,6.f,
  7.f,8.f,9.f
);

TEST_CASE("CHECK empty") {
  REQUIRE(matrix_test.empty() == false);
}

TEST_CASE("CHECK SIZE") {
  REQUIRE(matrix_test.size() == 9);
}

TEST_CASE("CHECK ROW") {
  REQUIRE(matrix_test.row() == 3);
  REQUIRE(matrix_test.row_col().row == 3);
}

TEST_CASE("CHECK COLUMN") {
  REQUIRE(matrix_test.col() == 3);
  REQUIRE(matrix_test.row_col().col == 3);
}

TEST_CASE ("TRANSPOSE FOR SQUARED MATRIX") {
  matrix_<float, 3, 3> actual
  (
    1.f, 2.f, 3.f,
    4.f, 5.f, 6.f,
    7.f, 8.f, 9.f
  );
  actual.transpose_squared();

  constexpr matrix_<float, 3, 3> expected
  (
    1.f, 4.f, 7.f,
    2.f, 5.f, 8.f,
    3.f, 6.f, 9.f
  );
  REQUIRE(expected == actual);
}

TEST_CASE ("TRANSPOSE FOR NON-SQUARED MATRIX") {
  constexpr matrix_<float, 3, 4> arr
  (
    0.f, 3.f, 6.f, 9.f,
    1.f, 4.f, 7.f, 10.f,
    2.f, 5.f, 8.f, 11.f
  );
  constexpr auto actual = arr.transpose_triangular();

  constexpr matrix_<float, 4, 3> expected
  (
    0.f, 1.f, 2.f,
    3.f, 4.f, 5.f,
    6.f, 7.f, 8.f,
    9.f, 10.f, 11.f
  );
  REQUIRE(expected == actual);
}

TEST_CASE ("FILL-METHOD") {
  matrix_<int, 3, 3> nums;
  nums.fill(1);
  for ( auto &&i : nums ) {
    REQUIRE( i == 1 );
  }
}

// matrix multiplications
TEST_CASE ("NON-SQUARED MULTIPLICATION") {
  constexpr matrix_<int, 2, 3> y
  (
    0, 3, 5,
    5, 5, 2
  );
  constexpr matrix_<int, 3, 2> x
  (
    3, 4,
    3, -2,
    4, -2
  );
  constexpr auto actual = ( y * x );

  constexpr matrix_<int, 2, 2> expected
  (
    29, -16,
    38,  6
  );
  REQUIRE(expected == actual);
}

TEST_CASE("SQUARED MATRIX MULTIPLICATION") {
  constexpr matrix_<int, 2, 2> k
  (
    0, 3,
    2, 5
  );
  constexpr matrix_<int, 2, 2> j
  (
    3, 4,
    5, 6
  );
  constexpr auto actual = ( k * j );

  constexpr matrix_<int, 2, 2> expected
  (
    15,	18,
    31,	38
  );
  REQUIRE(expected == actual);
}

TEST_CASE("SQUARED MATRIX x NON-SQUARED MATRIX") {
  constexpr matrix_<int, 2, 2> a
  (
    1, 2,
    3, 4
  );
  constexpr matrix_<int, 2, 4> b
  (
    3, 5, 2, 3,
    4, 5, 3, 2
  );
  constexpr auto actual = ( a * b );

  constexpr matrix_<int, 2, 4> expected
  (
    11, 15, 8, 7,
    25, 35, 18, 17
  );
  REQUIRE(expected == actual);
}

TEST_CASE("MATRIX x SCALAR") {
  matrix_<int, 2, 2> arr
  (
    2, 3,
    4, 5
  );
  const auto actual = ( arr * 2);

  constexpr matrix_<int, 2, 2> expected
  (
    4, 6,
    8, 10
  );
  REQUIRE(expected == actual);
}

TEST_CASE ("SQUARED MATRIX ROTATION") {
  matrix_<int, 3, 3> actual
  (
    1, 2, 3,
    4, 5, 6,
    7, 8, 9
  );
  actual.transpose_squared();
  actual.rotate_squared();

  constexpr matrix_<int, 3, 3> expected
  (
    7, 4, 1,
    8, 5, 2,
    9, 6, 3
  );
  REQUIRE(expected == actual);
}

TEST_CASE("NON-SQUARED MATRIX ROTATION") {
  constexpr matrix_<int, 3, 5> arr
  (
    3, 4, 5, 6, 8,
    5, 4, 3, 2, 6,
    3, 3, 7, 8, 9
  );
  auto actual = arr.transpose_triangular();
  actual.rotate_triangular<actual.row(), actual.col()>();

  constexpr matrix_<int, 5, 3> expected
  (
    8, 6, 9,
    6, 2, 8,
    5, 3, 7,
    4, 4, 3,
    3, 5, 3
  );
  REQUIRE(expected == actual);
}

TEST_CASE("MATRIX ADDITION") {
  constexpr matrix_<int, 3, 3> a
  (
    3, 4, 5,
    5, 4, 3,
    3, 3, 7
  );

  constexpr matrix_<int, 3, 3> b
  (
    3, 4, 5,
    5, 4, 3,
    3, 3, 7
  );

  constexpr auto actual = (a + b);
  constexpr matrix_<int, 3, 3> expected
  (
    6, 8, 10,
    10, 8, 6,
    6, 6, 14
  );
  REQUIRE(expected == actual);
}

TEST_CASE("MATRIX SUBTRACTION") {
  constexpr matrix_<int, 3, 3> a
  (
    3, 4, 5,
    5, 4, 3,
    3, 3, 7
  );

  constexpr matrix_<int, 3, 3> b
  (
    3, 4, 5,
    5, 4, 3,
    3, 3, 7
  );
  constexpr auto actual = (a - b);
  constexpr matrix_<int, 3, 3> expected
  (
    0, 0, 0,
    0, 0, 0,
    0, 0, 0
  );
  REQUIRE(expected == actual);
}

TEST_CASE("MATRIX ADDITION WITH SCALAR") {
  constexpr  matrix_<int, 3, 3> a
  (
    3, 4, 5,
    5, 4, 3,
    3, 3, 7
  );

  constexpr auto actual = (a + 2);
  constexpr matrix_<int, 3, 3> expected
  (
    5, 6, 7,
    7, 6, 5,
    5, 5, 9
  );
  REQUIRE(expected == actual);
}

TEST_CASE("MATRIX SUBTRACTION WITH SCALAR") {
  constexpr matrix_<int, 3, 3> a
  (
    3, 4, 5,
    5, 4, 3,
    3, 3, 7
  );

  constexpr auto actual = (a - 2);
  constexpr matrix_<int, 3, 3> expected
  (
    1, 2, 3,
    3, 2, 1,
    1, 1, 5
  );
  REQUIRE(expected == actual);
}
