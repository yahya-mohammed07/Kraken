#include "../source/library/core/numeric.hpp"
#include "../Catch2/catch.hpp"

using namespace kraken;


TEST_CASE("ACCUMULATE") {
  constexpr matrix_<int, 1, 5> numeric_test ( 1, 2, 3, 4, 5 );
  REQUIRE( cal::acc(0, numeric_test) == 15 ); // plus by default - or check next way:
  REQUIRE( cal::acc(0, numeric_test.begin(), numeric_test.end()) == 15 ); // plus by default
  REQUIRE( cal::acc(0, op::minus{}, numeric_test) == 3 );
  REQUIRE( cal::acc(1, op::multiplies{}, numeric_test) == 120 );
}

TEST_CASE("CALCULATE") {
// variadic
  REQUIRE( cal::calcu(0 , op::plus{} ,1 , 2, 3, 4, 5) == 15);
  REQUIRE( cal::calcu(1 , op::multiplies{}, 1, 2, 3, 4, 5) == 120);
// init list
  REQUIRE( cal::calcu(0 , op::plus{}, {1 , 2, 3, 4, 5}) == 15);
  REQUIRE( cal::calcu(1 , op::minus{} , {2, -3, +4, -5}) == 3);
  REQUIRE( cal::calcu(1 , op::multiplies{}, {1, 2, 3, 4, 5}) == 120);
}

TEST_CASE("LN") {
  REQUIRE( cal::equal(cal::ln(constants::e_v<float>), 1.f) == true );
  REQUIRE( cal::equal( cal::ln(4.), 1.38629436112 ) == true );
  REQUIRE( cal::equal( cal::ln(7.), 1.94591014906, 0.0000001 ) == true );
}

TEST_CASE("SQRT") {
  REQUIRE(cal::sqrt(4) == 2);
  REQUIRE(cal::sqrt(25) == 5);
  REQUIRE(cal::equal(cal::sqrt(3.), 1.73205080757, 0.000000000001));
}

TEST_CASE("SQR") {
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

TEST_CASE("LOG BASE 2 OF X") {
  REQUIRE(cal::log2(32) == 5);
  REQUIRE(cal::log2(82) == 6);
  REQUIRE(cal::approx_equal(cal::log2(32.), 5., 0.1));
}

TEST_CASE("LOG BASE 10 OF X") {
  REQUIRE(cal::log10(100) == 2);
  REQUIRE(cal::log10(10) == 1);
}

TEST_CASE("MAX") {
  REQUIRE(cal::max(90, 87) == 90);
  REQUIRE(cal::max(33, 87, 3, 66) == 87);
  REQUIRE(cal::max({33, 87, 3, 66}) == 87);
   matrix_<int, 1, 4> nums {33, 87, 3, 66};
  REQUIRE(cal::max(nums) == 87);
  REQUIRE(*cal::max_range(nums.begin(), nums.end()) == 87);
}

TEST_CASE("MIN") {
  REQUIRE(cal::min(90, 87) == 87);
  REQUIRE(cal::min(33, 87, 3, 66) == 3);
  REQUIRE(cal::min({33, 87, 3, 66}) == 3);
  constexpr matrix_<int, 1, 4> nums ( 33, 87, 3, 66 );
  REQUIRE(cal::min(nums) == 3);
  REQUIRE(*cal::min_range(nums.begin(), nums.end()) == 3);
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

TEST_CASE("MODF") {
  const auto [integral, fractional] = cal::modf(10.25);
  REQUIRE(integral == 10);
  REQUIRE(fractional == .25);
}

TEST_CASE("1's complement") {
  REQUIRE(cal::ones_complement<4>(1) == 0b1110);
  REQUIRE(cal::ones_complement(1, 4) == 14);
}

TEST_CASE("2's complement") {
  REQUIRE(cal::twos_complement<4>(1) == 0b1111);
  REQUIRE(cal::twos_complement(1, 4) == 15);
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

TEST_CASE("ABSOLUTE") {
  REQUIRE(cal::abs(-4) == 4);
  REQUIRE(cal::abs( -(std::numeric_limits<long long>::max()) )
                                    == std::numeric_limits<long long>::max());
  REQUIRE(cal::equal(cal::abs(-4.034), 4.034));
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

TEST_CASE("IS NEGATIVE") {
  REQUIRE(cal::is_neg(4) == false);
  REQUIRE(cal::is_neg(-1) == true);
  REQUIRE(cal::is_neg(0) == false);
  REQUIRE(cal::is_neg(-std::numeric_limits<long long>::max()) == true);
  REQUIRE(cal::is_neg(std::numeric_limits<std::size_t>::max()) == false);
  REQUIRE(cal::is_neg(-std::numeric_limits<std::size_t>::max()) == false);
}

TEST_CASE("HAS A SIGN BIT") {
  REQUIRE(cal::sign(-0.0f) == true);
  REQUIRE(cal::sign(+0.0f) == false);
  REQUIRE(cal::sign(-0.0l) == true);
  REQUIRE(cal::sign(+0.0l) == false);
  REQUIRE(cal::sign(-0.0435345) == true);
  REQUIRE(cal::sign(0.0435345) == false);
}

TEST_CASE("IS_PRIME") {
  REQUIRE(cal::is_prime(5) == true);
  REQUIRE(cal::is_prime(257) == true);
  REQUIRE(cal::is_prime(7919) == true);
  REQUIRE(cal::is_prime(115249) == true);
  REQUIRE(cal::is_prime(115243) == false);
}

TEST_CASE("FRACTION") {
  REQUIRE( cal::not_equal(cal::frac(10.25), .30) );
  REQUIRE( cal::equal(cal::frac(2251799813685240.0) , .0 ));
  REQUIRE( cal::equal(cal::frac(-34.25), 0.25));
  REQUIRE( cal::equal(cal::frac(345.4324324234124), .4324324234124, 0.00001));
}

TEST_CASE("FMOD") {
  REQUIRE(cal::equal( cal::fmod(+5.1, +3.0), 2.1 ) );
  REQUIRE(cal::equal( cal::fmod(-5.1, +3.0), -2.1 ) );
  REQUIRE(cal::equal( cal::fmod(-5.1, -3.0), -2.1 ) );
}

TEST_CASE("COMPARING FLOAT NUMBERS") {
  REQUIRE(cal::equal((.1f+.2f), .399f) == false);
  REQUIRE(cal::equal((.1f+.2f), .3f) == true);
  REQUIRE(cal::equal(0.000000f, 0.f) == true);
  REQUIRE(cal::approx_equal( constants::pi_v<float>, 3.14f, .01f ) == true);
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
  REQUIRE(cal::approx_equal( constants::pi, 3.14, .01 ) == true);
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

TEST_CASE("TRIGONOMETRY FUNCTIONS") {
  REQUIRE( cal::equal( cal::sin(30.), .5) );
  REQUIRE( cal::equal( cal::sin(90.), 1.) );
  //
  REQUIRE( cal::equal( cal::cos(90.), .0) );
  REQUIRE( cal::equal( cal::cos(45.), .70781, .001) );
  REQUIRE( cal::equal( cal::cos(60.), 0.5) );
  //
  REQUIRE( cal::equal( cal::tan(45.), 1.) );
  REQUIRE( cal::equal( cal::tan(-135.), 1.) );
  REQUIRE( cal::equal( cal::tan(32.), .624, .001) );
  //
  REQUIRE( cal::equal( cal::sec(45.), 1.414213, .001) );
  REQUIRE( cal::equal( cal::sec(40.), 1.305, .001) );
  REQUIRE( cal::equal( cal::sec(135.), -1.414, .001) );
  //
  REQUIRE( cal::equal( cal::cosc(30.), 2.) );
  REQUIRE( cal::equal( cal::cosc(12.), 4.80, 0.01) );
  REQUIRE( cal::equal( cal::cosc(89.), 1.000, 0.001) );
  //
  REQUIRE( cal::equal( cal::cot(45.), 1.) );
  REQUIRE( cal::equal( cal::cot(72.),  0.3239, 0.01) );
  //
  REQUIRE( cal::equal( cal::arc_cos(-1.), 3.14, .1) );
  REQUIRE( cal::equal( cal::arc_cos(-.9), 2.69, .1) );
  REQUIRE( cal::equal( cal::arc_cos(-.8), 2.49, .1) );
  //
  REQUIRE( cal::equal( cal::arc_sin(-1.), -1.570796, .0001) );
  REQUIRE( cal::equal( cal::to_degree(cal::arc_sin(0.5)), 30.,0.01) );
  //
  REQUIRE( cal::equal( cal::arc_tan(-1.), -0.785398163, .00000001) );
  REQUIRE( cal::equal( cal::arc_tan(1.), 0.785398163, 0.0000001) );
}

TEST_CASE("EXP") {
  REQUIRE( cal::equal( cal::exp( 1.f ),  constants::e_v<float>) );
}

TEST_CASE("GAMMA") {
  REQUIRE( cal::equal( cal::gamma( 10. ),  362880., 0.001) );
  REQUIRE( cal::equal( cal::gamma( 0.5 ),  1.77245, 0.001) );
  REQUIRE( cal::equal( cal::gamma( 1. ),  1., 0.001) );
}

TEST_CASE("BETA") {
  REQUIRE( cal::equal( cal::beta( 10., 9.),  2.2853e-06, 0.001) );
  REQUIRE( cal::equal( cal::beta( 3., 7.),  0.00396825, 0.001) );
}

TEST_CASE("BIT CAST") {
  const double f64v{ 19880124.0 };
  const auto u64v { cal::bit_cast<std::uint64_t>(f64v) };
  REQUIRE( cal::bit_cast<double>(u64v) == f64v );
//
  const std::uint64_t u64v2{ 0x3fe9000000000000ull };
  const auto f64v2 { cal::bit_cast<double>(u64v2) };
  REQUIRE( cal::bit_cast<std::uint64_t>(f64v2) == u64v2 );
}

TEST_CASE("MEDIAN") {
  constexpr matrix_<int, 1, 7> nums { 12, 16, 12, 6, 18, 2, 4};
  REQUIRE( cal::median(nums) == 12 );
   matrix_<double, 1, 8> nums2 { 42, 7, 17, 14, 7, 24, 15, 29};
  REQUIRE( cal::median(nums2) == 16 );
}