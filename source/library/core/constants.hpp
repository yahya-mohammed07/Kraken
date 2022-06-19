#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

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

#include "common/my_concepts.hpp"

/// @brief math and physics constants in one place!
namespace kraken::constants {

///@brief Pi, Archimedes' constant or Ludolph's number π
template <class Ty>
inline constexpr is_float auto pi_v{static_cast<Ty>(3.14159'26535'89793'23846)};
///@brief 1/π
template <class Ty>
inline constexpr is_float auto inv_pi_v{
    static_cast<Ty>(0.318309886183790671537767526745028724)};
///@brief 1/sqrt(π)
template <class Ty>
inline constexpr is_float auto inv_sqrt_pi_v{
    static_cast<Ty>(0.564189583547756286948079451560772586)};
///@brief Bernstein's constant
template <class Ty>
inline constexpr is_float auto beta_v{
    static_cast<Ty>(0.28016'94990'23869'13303)};
///@brief e, Napier's constant, or Euler's number
template <class Ty>
inline constexpr is_float auto e_v{
    static_cast<Ty>(2.71828'18284'59045'23536'02874)};
///@brief Feigenbaum constant α
template <class Ty>
inline constexpr is_float auto alpha_v{
    static_cast<Ty>(2.50290'78750'95892'82228)};
///@brief Reciprocal Fibonacci constant ψ
template <class Ty>
inline constexpr is_float auto sai_v{
    static_cast<Ty>(3.35988'56662'43177'55317'201137)};
///@brief Feigenbaum constant δ
template <class Ty>
inline constexpr is_float auto delta_v{
    static_cast<Ty>(4.66920'16091'02990'67185'32038)};
///@brief omega constant Ω
template <class Ty>
inline constexpr is_float auto omega_v{
    static_cast<Ty>(0.56714'32904'09783'87299'99686)};
///@brief Conway's constant
template <class Ty>
inline constexpr is_float auto lambda_conway_v{
    static_cast<Ty>(1.30357'72690'34296'39125'70991)};
///@brief Golomb–Dickman constant
template <class Ty>
inline constexpr is_float auto lambda_golomb_v{
    static_cast<Ty>(0.62432'99885'43550'87099'29363)};
///@brief Apéry's constant ζ_3
template <class Ty>
inline constexpr is_float auto zeta_3_v{
    static_cast<Ty>(1.20205'69031'59594'28539'97381)};
///@brief Gauss–Kuzmin–Wirsing constant
template <class Ty>
inline constexpr is_float auto lambda_gauss_v{
    static_cast<Ty>(0.30366'30028'98732'65859'74481)};
///@brief Euler–Mascheroni constant γ
template <class Ty>
inline constexpr is_float auto gamma_v{
    static_cast<Ty>(0.57721'56649'01532'86060)};
///@brief Golden ratio ϕ
template <class Ty>
inline constexpr is_float auto phi_v{
    static_cast<Ty>(1.61803'39887'49894'84820'45868)};
///@brief Elementary charge in `C`
template <class Ty>
inline constexpr is_float auto e_charge_v{static_cast<Ty>(1.602176634e-19)};
///@brief Planck constant in `J*Hz`
template <class Ty>
inline constexpr is_float auto h_v{static_cast<Ty>(6.62607015e-34)};
///@brief Newtonian constant of gravitation in `m^3⋅kg^-1`
template <class Ty>
inline constexpr is_float auto G_v{static_cast<Ty>(6.67430 * 15e-11)};
///@brief Magnetic permeability of a vacuum in `N*s2/C2` μ0
template <class Ty>
inline constexpr is_float auto mu0_v{
    static_cast<Ty>(4 * 3.14159'26535'89793'23846e-7)};
///@brief log_2e
template <class Ty>
inline constexpr is_float auto log2e_v{
    static_cast<Ty>(1.442695040888963407359924681001892137)};
///@brief log_10e
template <class Ty>
inline constexpr is_float auto log10e_v{
    static_cast<Ty>(0.434294481903251827651128918916605082)};
///@brief log_e 2
template <class Ty>
inline constexpr is_float auto ln2_v{
    static_cast<Ty>(0.693147180559945309417232121458176568)};
///@brief log_e 10
template <class Ty>
inline constexpr is_float auto ln10_v{
    static_cast<Ty>(2.302585092994045684017991454684364208)};
///@brief sqrt(2)
template <class Ty>
inline constexpr is_float auto sqrt2_v{
    static_cast<Ty>(1.414213562373095048801688724209698079)};
///@brief sqrt(3)
template <class Ty>
inline constexpr is_float auto sqrt3_v{
    static_cast<Ty>(1.732050807568877293527446341505872367)};
///@brief 1/sqrt(3)
template <class Ty>
inline constexpr is_float auto inv_sqrt3_v{
    static_cast<Ty>(0.577350269189625764509148780501957456)};
/// @brief avogadro constant
template <class Ty>
inline constexpr is_float auto avogadro_v{static_cast<Ty>(6.0221413e23)};
///

///@brief Pi, Archimedes' constant or Ludolph's number
inline constexpr auto pi{pi_v<double>};
///@brief 1/π
inline constexpr auto inv_pi{inv_pi_v<double>};
///@brief 1/sqrt(π)
inline constexpr auto inv_sqrt_pi{inv_sqrt_pi_v<double>};
///@brief Bernstein's constant β
inline constexpr auto B{beta_v<double>};
///@brief Embree–Trefethen constant β*
inline constexpr float B_star{0.70258F};
///@brief e, Napier's constant, or Euler's number
inline constexpr auto e{e_v<double>};
///@brief Feigenbaum constant α
inline constexpr auto alpha{alpha_v<double>};
///@brief Reciprocal Fibonacci constant ψ
inline constexpr auto sai{sai_v<double>};
///@brief Feigenbaum constant δ
inline constexpr auto delta{delta_v<double>};
///@brief omega constant Ω
inline constexpr auto omega{omega_v<double>};
///@brief Conway's constant
inline constexpr auto lambda_conway{lambda_conway_v<double>};
///@brief Gauss–Kuzmin–Wirsing constant
inline constexpr auto lambda_gauss{lambda_gauss_v<double>};
///@brief Golomb–Dickman constant
inline constexpr auto lambda_golomb{lambda_golomb_v<double>};
///@brief Apéry's constant ζ_3
inline constexpr auto zeta_3{zeta_3_v<double>};
///@brief Euler–Mascheroni constant γ
inline constexpr auto gamma{gamma_v<double>};
///@brief Golden ratio ϕ
inline constexpr auto phi{phi_v<double>};
///@brief Elementary charge in `C`
inline constexpr auto e_charge{e_charge_v<double>};
///@brief Planck constant in `J*Hz`
inline constexpr auto h{h_v<double>};
///@brief speed of light in `m/s`
inline constexpr std::size_t c{299792458ULL};
///@brief Gravity of earth in `m/s^2`
inline constexpr float g{9.80665F};
///@brief Newtonian constant of gravitation in `m^3⋅kg^-1`
inline constexpr auto G{G_v<double>};
///@brief Magnetic permeability of a vacuum in `N*s2/C2` μ0
inline constexpr auto mu0{mu0_v<double>};
///@brief log_2e
inline constexpr auto log2e{log2e_v<double>};
///@brief log_10e
inline constexpr auto log10e{log2e_v<double>};
///@brief log_e 2
inline constexpr auto ln2{ln2_v<double>};
///@brief log_e 10
inline constexpr auto ln10{ln10_v<double>};
///@brief sqrt(2)
inline constexpr auto sqrt2{sqrt2_v<double>};
///@brief sqrt(3)
inline constexpr auto sqrt3{sqrt3_v<double>};
///@brief 1/sqrt(3)
inline constexpr auto inv_sqrt3{inv_sqrt3_v<double>};
/// @brief avogadro constant
inline constexpr auto avogadro{avogadro_v<double>};
} // namespace kraken::constants

#endif // CONSTANTS_HPP
