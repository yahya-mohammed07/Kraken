#include "../source/library/core/matrix.hpp"
#include "../source/library/core/numeric_methods.hpp"
#include <catch2/catch_test_macros.hpp>
#include <iomanip>

TEST_CASE("CHECK GAUSS-ELIMINATION AND DETERMINNAT") {
  constexpr matrix_<float, 3, 3> matrix{1.f, 2.f, 3.f, 4.f, 5.f,
                                        6.f, 7.f, 8.f, 9.f};
  auto mat = lab::num_methods::gauss_elimination(matrix);
  REQUIRE(mat.at<1, 0>() == 0.f);
  REQUIRE(mat.at<2, 0>() == 0.f);
  REQUIRE(mat.at<2, 1>() == 0.f);
  auto deter = lab::num_methods::determined(matrix);
  REQUIRE(deter == 0.f);
}

TEST_CASE("CHECK LEAST-SQUARES") {
  constexpr matrix_<float, 1, 8> xi{0, 1, 1, 2, 3, 4, 2, 5};
  constexpr matrix_<float, 1, 8> yi{3, 4, 6, 9, 11, 15, 10, 16};
  const auto actual = lab::num_methods::least_squares(xi, yi);
  constexpr float expected = 5.75641f;
  REQUIRE(expected == actual);
}

TEST_CASE("CRAMER'S RULE") {
  matrix_<float, 3, 3> mat
  {
    3, -1, 2,
    1, 2, 3,
    2, -2, -1
  };
  constexpr matrix_<float, 1, 3> right_side{12, 11, 2};
  auto actual =
    std::move(lab::num_methods::cramer(mat, right_side));
  constexpr matrix_<float, 1, 3> expected
  {2.99999928474f, 1.00000035763f, 1.99999976158f};
  REQUIRE(expected == actual);
}

TEST_CASE("SIMPSON'S RULE") {
  constexpr auto actual =
      lab::num_methods::simpson(6, 1.f, 3.f,
                [](auto &&i) { return i * i; });
  constexpr float expected = 8.6666669846f;
  REQUIRE(expected == actual);
}

TEST_CASE("NEWTON'S METHOD") {
  constexpr auto val = 2;
  constexpr auto actual = lab::num_methods::newton(1.f, 20.f, [&val](auto x) {
                                          return (x*x) - val;
                                        }, [](auto x){
                                          return (2.*x);
                                        });
  constexpr float expected = 1.4142135623730951;
  REQUIRE(expected == actual);
}