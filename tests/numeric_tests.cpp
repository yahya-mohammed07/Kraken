#include "../source/library/core/numeric.hpp"
#include "../source/library/core/constants.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace kraken;

constexpr std::array<int, 5> numeric_test { 1, 2, 3, 4, 5 };

TEST_CASE("CHECK ACCUMULATE") {
  REQUIRE( cal::acc(0, numeric_test) == 15 ); // plus by default - or check next way:
  REQUIRE( cal::acc(0, numeric_test.begin(), numeric_test.end()) == 15 ); // plus by default
  REQUIRE( cal::acc(0, op::minus{}, numeric_test) == 3 );
  REQUIRE( cal::acc(1, op::multiplies{}, numeric_test) == 120 );
}

TEST_CASE("CHECK CALCULATE") {
// variadic
  REQUIRE( cal::calcu(0 , op::plus{} ,1 , 2, 3, 4, 5) == 15);
  //REQUIRE( cal::calcu(1 , op::minus{} , 2, -3, +4, -5) == 3); // FIXME: or not
  REQUIRE( cal::calcu(1 , op::multiplies{}, 1, 2, 3, 4, 5) == 120);
// init list
  REQUIRE( cal::calcu(0 , op::plus{}, {1 , 2, 3, 4, 5}) == 15);
  REQUIRE( cal::calcu(1 , op::minus{} , {2, -3, +4, -5}) == 3);
  REQUIRE( cal::calcu(1 , op::multiplies{}, {1, 2, 3, 4, 5}) == 120);
}

TEST_CASE("LN(X)") {
  REQUIRE(cal::ln(constants::e_v<float>) == 1.f);
  REQUIRE(cal::ln(4.f) == 1.38629436112f);
  REQUIRE(cal::ln(7.f) == 1.94591014906f);
}

TEST_CASE("SQRT(x)") {
  REQUIRE(cal::sqrt(4.f) == 2);
  REQUIRE(cal::sqrt(3.f) == 1.73205080757f);
}

TEST_CASE("SQR(x)") {
  REQUIRE(cal::sqr(2.) == 4);
}

TEST_CASE("BASE TO POWER OF POSITIVE INTEGER") {
  REQUIRE(cal::pow(3, 4) == 81);
  REQUIRE(cal::pow(2, 3) == 8);
  REQUIRE(cal::pow(8, 4) == 4096);
}

TEST_CASE("BASE TO POWER OF NEGATIVE-INTEGER") {
  REQUIRE(cal::pow(3.f, -4) == 0.01234567901f);
}

TEST_CASE("BASE TO POWER OF FLOATING-POINT") {
  REQUIRE( cal::powf(3.f, 1.5f) == 5.19615242271f);
}

TEST_CASE("CONTAINER TO POWER") {
  matrix_<int, 1, 3> actual { 1, 2, 3 };
  cal::pow_container(actual, 2);
  const matrix_<int, 1, 3> expected { 1, 4, 9 };
  REQUIRE(expected == actual);
}

TEST_CASE("LOG BASE 2 OF X") {
  REQUIRE(cal::log2(32) == 5u);
  REQUIRE(cal::log2(82) == 6u);
  REQUIRE(cal::log2(32.f) == 5.f);
}

TEST_CASE("LOG BASE 10 OF X") {
  REQUIRE(cal::log10(100) == 2u);
}

TEST_CASE("MAX") {
  REQUIRE(cal::max(90, 87) == 90);
  REQUIRE(cal::max(33, 87, 3, 66) == 87);
  REQUIRE(cal::max({33, 87, 3, 66}) == 87);
  constexpr matrix_<int, 1, 4> nums {33, 87, 3, 66};
  REQUIRE(cal::max(nums) == 87);
}

TEST_CASE("MIN") {
  REQUIRE(cal::min(90, 87) == 87);
  REQUIRE(cal::min(33, 87, 3, 66) == 3);
  REQUIRE(cal::min({33, 87, 3, 66}) == 3);
  constexpr matrix_<int, 1, 4> nums {33, 87, 3, 66};
  REQUIRE(cal::min(nums) == 3);
}

TEST_CASE("MIN_MAX") {
  REQUIRE(cal::min_max(90, 87).Min == 87);
  REQUIRE(cal::min_max(90, 87).Max == 90);
  //
  REQUIRE(cal::min_max(33, 87, 3, 66).Min == 3);
  REQUIRE(cal::min_max(33, 87, 3, 66).Max == 87);
  //
  REQUIRE(cal::min_max({33, 87, 3, 66}).Min == 3);
  REQUIRE(cal::min_max({33, 87, 3, 66}).Max == 87);
  //
  constexpr matrix_<int, 1, 4> nums {33, 87, 3, 66};
  REQUIRE(cal::min_max(nums).Min == 3);
  REQUIRE(cal::min_max(nums).Max == 87);
}