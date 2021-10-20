#include "../source/library/core/numeric.hpp"
#include "../source/library/core/constants.hpp"
#include "../Catch2/catch.hpp"

#include <cstdio>

using namespace kraken;

constexpr matrix_<int, 1, 5> numeric_test ( 1, 2, 3, 4, 5 );

TEST_CASE("CHECK ACCUMULATE") {
  REQUIRE( cal::acc(0, numeric_test) == 15 ); // plus by default - or check next way:
  REQUIRE( cal::acc(0, numeric_test.begin(), numeric_test.end()) == 15 ); // plus by default
  REQUIRE( cal::acc(0, op::minus{}, numeric_test) == 3 );
  REQUIRE( cal::acc(1, op::multiplies{}, numeric_test) == 120 );
}

TEST_CASE("CHECK CALCULATE") {
// variadic
  REQUIRE( cal::calcu(0 , op::plus{} ,1 , 2, 3, 4, 5) == 15);
  REQUIRE( cal::calcu(1 , op::multiplies{}, 1, 2, 3, 4, 5) == 120);
// init list
  REQUIRE( cal::calcu(0 , op::plus{}, {1 , 2, 3, 4, 5}) == 15);
  REQUIRE( cal::calcu(1 , op::minus{} , {2, -3, +4, -5}) == 3);
  REQUIRE( cal::calcu(1 , op::multiplies{}, {1, 2, 3, 4, 5}) == 120);
}

TEST_CASE("LN(X)") {
  REQUIRE( cal::equal(cal::ln(constants::e_v<float>), 1.f) == true );
  REQUIRE( cal::equal( cal::ln(4.), 1.38629436112 ) == true );
  REQUIRE( cal::equal( cal::ln(7.), 1.94591014906, 0.0000001 ) == true );
}

TEST_CASE("SQRT(x)") {
  REQUIRE(cal::sqrt(4) == 2);
  REQUIRE(cal::sqrt(25) == 5);
  REQUIRE(cal::equal(cal::sqrt(3.), 1.73205080757, 0.000000000001));
}

TEST_CASE("SQR(x)") {
  REQUIRE(cal::sqr(2) == 4);
  REQUIRE(cal::sqr(4.5) == 20.25);
}

TEST_CASE("BASE TO POWER OF POSITIVE INTEGER") {
  REQUIRE(cal::pow(3, 4) == 81);
  REQUIRE(cal::pow(2, 3) == 8);
  REQUIRE(cal::pow(8, 4) == 4096);
}

TEST_CASE("BASE TO POWER OF NEGATIVE-INTEGER") {
  REQUIRE( cal::equal(cal::pow(3., -4), 0.01234567901, 0.000000001) );
}

TEST_CASE("BASE TO POWER OF FLOATING-POINT") {
  REQUIRE( cal::equal(cal::pow(3., 1.5), 5.19615242271, 0.000000000001) );
}

TEST_CASE("CONTAINER TO POWER") {
  matrix_<int, 1, 3> actual { 1, 2, 3 };
  cal::pow_container(actual, 2);
  const matrix_<int, 1, 3> expected ( 1, 4, 9 );
  REQUIRE(expected == actual);
}

TEST_CASE("LOG BASE 2 OF X") {
  REQUIRE(cal::log2(32) == 5u);
  REQUIRE(cal::log2(82) == 6u);
  REQUIRE(cal::approx_equal(cal::log2(32.), 5., 0.1));
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
  constexpr matrix_<int, 1, 4> nums ( 33, 87, 3, 66 );
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
  constexpr matrix_<int, 1, 4> nums ( 33, 87, 3, 66 );
  REQUIRE(cal::min_max(nums).Min == 3);
  REQUIRE(cal::min_max(nums).Max == 87);
}

TEST_CASE("GCD") {
  REQUIRE(cal::gcd(42, 56) == 14);
  REQUIRE(cal::gcd(24826148, 45296490) == 526);
  REQUIRE(cal::gcd(0, 9) == 9);
  REQUIRE(cal::gcd(12, 0) == 12);
}

TEST_CASE("LCM") {
  REQUIRE(cal::lcm(45, 88) == 3960);
  REQUIRE(cal::lcm(12, 33) == 132);
}

TEST_CASE("FLOOR") {
  REQUIRE(cal::floor(45.453) == 45);
  REQUIRE(cal::floor(45.753) == 45);
  REQUIRE(cal::floor(-1.3) == -2);
  REQUIRE(cal::floor(-3.2) == -4);
}

TEST_CASE("CEIL") {
  REQUIRE(cal::ceil(45.453) == 46);
  REQUIRE(cal::ceil(45.753) == 46);
  REQUIRE(cal::ceil(-1.3) == -1);
  REQUIRE(cal::ceil(-3.2) == -3);
}

TEST_CASE("NUMBER OF DECIMAL PLACES") {
  REQUIRE(cal::decimal_places(45.453) == 3);
  REQUIRE(cal::decimal_places(45.453343434) == 9);
  REQUIRE(cal::decimal_places(45.753) == 3);
  REQUIRE(cal::decimal_places(-1.3) == 1);
  REQUIRE(cal::decimal_places(-3.2) == 1);
}

TEST_CASE("TRUNC") {
  REQUIRE(cal::trunc(10.25) == 10);
  REQUIRE(cal::trunc(2251799813685240.0) == 2251799813685240);
  REQUIRE(cal::trunc(-34.25) == -34);
  REQUIRE(cal::trunc(345.4324324234124) == 345);
}

TEST_CASE("ROUND") {
  REQUIRE(cal::round(10.25) == 10);
  REQUIRE(cal::round(10.55) == 11);
  REQUIRE(cal::round(-34.25) == -34);
  REQUIRE(cal::round(-34.66) == -35);
}

TEST_CASE("HYPOT") {
  REQUIRE(cal::hypot(4, 3) == 5);
  REQUIRE(cal::approx_equal(cal::hypot(2., 3.) , 3.605, 0.001));
  REQUIRE(cal::hypot( {2,3,4,5} ) == 7 );
}

TEST_CASE("FIBONACCI") {
  REQUIRE(cal::fibonacci(13) == 233);
  REQUIRE(cal::fibonacci(8) == 21);
  REQUIRE(cal::fibonacci(23) == 28657);
  REQUIRE(cal::fibonacci(19) == 4181);
  REQUIRE(cal::fibonacci(43) == 433494437);
  REQUIRE(cal::fibonacci(53) == 53316291173);
  REQUIRE(cal::fibonacci(72) == 498454011879265);
  REQUIRE(cal::fibonacci(91) == 4660046610375549952); // big, not very accuarate
  REQUIRE(cal::fibonacci(92) == 7540113804746366976); // big, not very accuarate
  REQUIRE(cal::fibonacci(52) == 32951280099);
}

TEST_CASE("FACTORIAL") {
  REQUIRE(cal::factorial(4) == 24);
  REQUIRE(cal::factorial(8) == 40320);
  REQUIRE(cal::factorial(12) == 479001600);
  REQUIRE(cal::factorial(14) == 87178291200);
  REQUIRE(cal::factorial(20) == 2432902008176640000);
}

TEST_CASE("DIV") {
  const auto& [quot, rem] = cal::div(645, 67);
  REQUIRE(quot == 9);
  REQUIRE(rem == 42);
}

TEST_CASE("IS_NEG") {
  REQUIRE(cal::is_neg(-0.0435345) == true);
  REQUIRE(cal::is_neg(0.0435345) == false);
  REQUIRE(cal::is_neg(4) == false);
  REQUIRE(cal::is_neg(-1) == true);
  REQUIRE(cal::is_neg(0) == false);
}

TEST_CASE("IS_PRIME") {
  REQUIRE(cal::is_prime(5) == true);
  REQUIRE(cal::is_prime(257) == true);
  REQUIRE(cal::is_prime(7919) == true);
  REQUIRE(cal::is_prime(115249) == true);
  REQUIRE(cal::is_prime(115243) == false);
}

TEST_CASE("COMPARING FLOAT NUMBERS") {
  REQUIRE(cal::equal((.1f+.2f), .399f) == false);
  REQUIRE(cal::equal((.1f+.2f), .3f) == true);
  REQUIRE(cal::equal(0.000000f, 0.f) == true);
  REQUIRE(cal::approx_equal( constants::π_v<float>, 3.14f, .01f ) == true);
  ///
  REQUIRE(cal::less_than(-.01f, .01f) == true);
  REQUIRE(cal::less_or_equal(.0000000001f, .01f) == true);
  REQUIRE(cal::less_or_equal(.01f, .01f) == true);
  ///
  REQUIRE(cal::greater_than(.0f, .01f) == false);
  REQUIRE(cal::greater_than(.34f, .01f) == true);
  REQUIRE(cal::greater_or_equal(.34f, .01f) == true);
  REQUIRE(cal::greater_or_equal(.01f, .01f) == true);
  REQUIRE(cal::greater_or_equal(.01f, .45f) == false);
}

TEST_CASE("COMPARING DOUBLE NUMBERS") {
  REQUIRE(cal::equal((.1+.2), .399) == false);
  REQUIRE(cal::equal((.1+.2), .3) == true);
  REQUIRE(cal::equal(0.000000, 0.) == true);
  REQUIRE(cal::approx_equal( constants::π, 3.14, .01 ) == true);
  ///
  REQUIRE(cal::less_than(-.01, .01) == true);
  REQUIRE(cal::less_than(.01, .01) == false);
  REQUIRE(cal::less_or_equal(.0000000001, .01) == true);
  REQUIRE(cal::less_or_equal(.01, .01) == true);
  ///
  REQUIRE(cal::greater_than(.0, .01) == false);
  REQUIRE(cal::greater_than(.34, .01) == true);
  REQUIRE(cal::greater_or_equal(.34, .01) == true);
  REQUIRE(cal::greater_or_equal(.01, .01) == true);
  REQUIRE(cal::greater_or_equal(.01, .45) == false);
}
