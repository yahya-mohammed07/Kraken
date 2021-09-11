#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <concepts>
#include <type_traits>
#include <numbers>


/// @brief math and physics constants in one place

namespace kraken::constants {

  template <class Ty>
  concept check_float = std::is_floating_point_v<Ty>;

  ///@brief Pi, Archimedes' constant or Ludolph's number
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty π_v = static_cast<Ty>(3.14159'26535'89793'23846L);
  ///@brief Pi, Archimedes' constant or Ludolph's number
  inline constexpr double π = π_v<double>;
  ///@brief 1/π
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty inv_π_v = static_cast<Ty>(0.318309886183790671537767526745028724L);
  ///@brief 1/π
  inline constexpr double inv_π = inv_π_v<double>;
  ///@brief 1/sqrt(π)
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty inv_sqrt_π_v =
            static_cast<Ty>(0.564189583547756286948079451560772586L);
  ///@brief 1/sqrt(π)
  inline constexpr double inv_sqrt_π = inv_sqrt_π_v<double>;
  ///@brief Bernstein's constant
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty β_v = static_cast<Ty>(0.28016'94990'23869'13303L);
  ///@brief Bernstein's constant
  inline constexpr double β = β_v<double>;
  ///@brief Embree–Trefethen constant
  inline constexpr float β_star = 0.70258f;
  ///@brief e, Napier's constant, or Euler's number
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty e_v = static_cast<Ty>(2.71828'18284'59045'23536'02874L);
  ///@brief e, Napier's constant, or Euler's number
  inline constexpr double e = e_v<double>;
  ///@brief Feigenbaum constant
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty α_v = static_cast<Ty>(2.50290'78750'95892'82228L);
  ///@brief Feigenbaum constant
  inline constexpr double α = α_v<double>;
  ///@brief Reciprocal Fibonacci constant
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty ψ_v = static_cast<Ty>(3.35988'56662'43177'55317'201137L);
  ///@brief Reciprocal Fibonacci constant
  inline constexpr double ψ = ψ_v<double>;
  ///@brief Feigenbaum constant
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty δ_v = static_cast<Ty>(4.66920'16091'02990'67185'32038L);
  ///@brief Feigenbaum constant
  inline constexpr double δ = δ_v<double>;
  ///@brief omega constant
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty Ω_v = static_cast<Ty>(0.56714'32904'09783'87299'99686L);
  ///@brief omega constant
  inline constexpr double Ω = Ω_v<double>;
  ///@brief Conway's constant
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty λ_conway_v =
      static_cast<Ty>(1.30357'72690'34296'39125'70991L);
  ///@brief Conway's constant
  inline constexpr double λ_conway = λ_conway_v<double>;
  ///@brief Gauss–Kuzmin–Wirsing constant
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty λ_gauss_v =
      static_cast<Ty>(0.30366'30028'98732'65859'74481L);
  ///@brief Gauss–Kuzmin–Wirsing constant
  inline constexpr auto λ_gauss = λ_gauss_v<double>;
  ///@brief Golomb–Dickman constant
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty λ_golomb_v =
      static_cast<Ty>(0.62432'99885'43550'87099'29363L);
  ///@brief Golomb–Dickman constant
  inline constexpr auto λ_golomb = λ_golomb_v<double>;
  ///@brief Apéry's constant
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty ζ_3_v = static_cast<Ty>(1.20205'69031'59594'28539'97381L);
  ///@brief Apéry's constant
  inline constexpr auto ζ_3 = ζ_3_v<double>;
  ///@brief Euler–Mascheroni constant
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty γ_v = static_cast<Ty>(0.57721'56649'01532'86060L);
  ///@brief Euler–Mascheroni constant
  inline constexpr auto γ = γ_v<double>;
  ///@brief Golden ratio
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty ϕ_v = static_cast<Ty>(1.61803'39887'49894'84820'45868L);
  ///@brief Golden ratio
  inline constexpr double ϕ = ϕ_v<double>;
  ///@brief Elementary charge in `C`
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty e_charge_v = static_cast<Ty>(1.602176634e-19L);
  ///@brief Elementary charge in `C`
  inline constexpr double e_charge = e_charge_v<double>;
  ///@brief Planck constant in `J*Hz`
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty h_v = static_cast<Ty>(6.62607015e-34L);
  ///@brief Planck constant in `J*Hz`
  inline constexpr double h = h_v<double>;
  ///@brief speed of light in `m/s`
  inline constexpr std::size_t c = 299792458;
  ///@brief Gravity of earth in `m/s^2`
  inline constexpr float g = 9.80665f;
  ///@brief Newtonian constant of gravitation in `m^3⋅kg^-1`
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty G_v = static_cast<Ty>(6.67430 * 15e-11L);
  ///@brief Newtonian constant of gravitation in `m^3⋅kg^-1`
  inline constexpr auto G = G_v<double>;
  ///@brief Magnetic permeability of a vacuum in `N*s2/C2`
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty μ0_v = static_cast<Ty>(4 * 3.14159'26535'89793'23846e-7L);
  ///@brief Magnetic permeability of a vacuum in `N*s2/C2`
  inline constexpr double μ0 = μ0_v<double>;
  ///@brief log_2e
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty log2e_v =
      static_cast<Ty>(1.442695040888963407359924681001892137L);
  ///@brief log_2e
  inline constexpr double log2e = log2e_v<double>;
  ///@brief log_10e
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty log10e_v = static_cast<Ty> (0.434294481903251827651128918916605082L);
  ///@brief log_10e
  inline constexpr double log10e = log2e_v<double>;
  ///@brief log_e 2
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty ln2_v = static_cast<Ty> (0.693147180559945309417232121458176568L);
  ///@brief log_e 2
  inline constexpr double ln2 = ln2_v<double>;
  ///@brief log_e 10
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty ln10_v = static_cast<Ty> (2.302585092994045684017991454684364208L);
  ///@brief log_e 10
  inline constexpr double ln10 = ln10_v<double>;
  ///@brief sqrt(2)
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty sqrt2_v = static_cast<Ty> (1.414213562373095048801688724209698079L);
  ///@brief sqrt(2)
  inline constexpr double sqrt2 = sqrt2_v<double>;
  ///@brief sqrt(3)
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty sqrt3_v = static_cast<Ty> (1.732050807568877293527446341505872367L);
  ///@brief sqrt(3)
  inline constexpr double sqrt3 = sqrt3_v<double>;
  ///@brief 1/sqrt(3)
  template <class Ty>
  requires check_float<Ty>
  inline constexpr Ty inv_sqrt3_v = static_cast<Ty> (0.577350269189625764509148780501957456L);
  ///@brief 1/sqrt(3)
  inline constexpr double inv_sqrt3 = inv_sqrt3_v<double>;
} // namespace math::constants

#endif // CONSTANTS_HPP
