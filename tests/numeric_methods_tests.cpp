#include "../Catch2/catch.hpp"
#include "../source/library/core/matrix.hpp"
#include "../source/library/core/numeric_methods.hpp"

TEST_CASE("CHECK GAUSS-ELIMINATION AND DETERMINED") {
  constexpr matrix_<float, 3, 3> matrix(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f,
                                        9.f);

  constexpr auto mat = kraken::num_methods::gauss_elimination(matrix);
  REQUIRE(mat.at<1, 0>() == 0.f);
  REQUIRE(mat.at<2, 0>() == 0.f);
  REQUIRE(mat.at<2, 1>() == 0.f);
  constexpr auto deter = kraken::num_methods::determined(matrix);
  REQUIRE(deter == 0.f);
}

TEST_CASE("CHECK LEAST-SQUARES") {
  constexpr matrix_<float, 1, 8> xi(0.f, 1.f, 1.f, 2.f, 3.f, 4.f, 2.f, 5.f);
  constexpr matrix_<float, 1, 8> yi(3.f, 4.f, 6.f, 9.f, 11.f, 15.f, 10.f, 16.f);
  constexpr auto actual = kraken::num_methods::least_squares(xi, yi);
  constexpr float expected = 5.75641f;
  REQUIRE(expected == actual);
}

TEST_CASE("CRAMER'S RULE") {
  matrix_<float, 3, 3> mat(3.f, -1.f, 2.f, 1.f, 2.f, 3.f, 2.f, -2.f, -1.f);
  constexpr matrix_<float, 1, 3> right_side(12.f, 11.f, 2.f);
  const auto actual = kraken::num_methods::cramer(mat, right_side);
  constexpr matrix_<float, 1, 3> expected(2.99999928474f, 1.00000023842f,
                                          1.99999976158f);
  REQUIRE(expected == actual);
}

TEST_CASE("SIMPSON'S RULE") {
  constexpr auto actual =
      kraken::num_methods::simpson(6, 1.f, 3.f, [](auto &&i) { return i * i; });
  constexpr float expected = 8.6666669846f;
  REQUIRE(expected == actual);
}

TEST_CASE("NEWTON'S METHOD") {
  constexpr auto val = 2;
  constexpr auto actual = kraken::num_methods::newton(
      1.f, 20.f, [&val](auto x) { return (x * x) - val; },
      [](auto x) { return (2. * x); });
  constexpr float expected = 1.4142135623730951f;
  REQUIRE(expected == actual);
}

TEST_CASE("Newton's Forward Difference Formula") {
  std::vector<float> xi{1891, 1901, 1911, 1921, 1931};
  std::vector<float> yi{46, 66, 81, 93, 101};
  const auto actual = kraken::num_methods::newtown_forward(xi, yi, 1895.f);
  const auto expected{54.8528f};
  REQUIRE(expected == actual);
}

TEST_CASE("Lagrange Interpolation") {
  constexpr std::array<float, 4> xi{2, 3, 4, 5};
  constexpr std::array<float, 4> yi{10, 30, 68, 150};
  constexpr auto actual{kraken::num_methods::lagrange(xi, yi, 1.f)};
  constexpr auto expected{-18.f};
  REQUIRE(expected == actual);
}
