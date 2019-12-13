/*!
 * \file
 * \brief Class tools::Frozenbits_generator_GA.
 */
#ifndef FROZENBITS_GENERATOR_GA_HPP_
#define FROZENBITS_GENERATOR_GA_HPP_

#include <string>
#include <limits>
#include <vector>

#include "Module/Decoder/Polar_MK/SC/Decoder_polar_MK_SC_naive.hpp"
#include "Tools/Code/Polar/Polar_code.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

namespace aff3ct
{
namespace tools
{
class Frozenbits_generator_GA : public Frozenbits_generator
{
private:
	std::vector<double> z;

	static constexpr double alpha = -0.4527;
	static constexpr double beta  =  0.0218;
	static constexpr double gamma =  0.8600;

	static constexpr double a =  1.0  / alpha;
	static constexpr double b = -beta / alpha;
	static constexpr double c =  1.0  / gamma;

	static constexpr double phi_pivot     = 0.867861;
	static constexpr double phi_inv_pivot = 0.6845772418;

	static constexpr double bisection_max = std::numeric_limits<double>::max();

	std::vector<bool> fake_frozen_bits;
	module::Decoder_polar_MK_SC_naive<int64_t, double> decoder_sc;

public:
	Frozenbits_generator_GA(const int K, const int N, const Polar_code& code,
	                        const std::string &dump_channels_path = "", const bool dump_channels_single_thread = true);

	virtual ~Frozenbits_generator_GA() = default;

	virtual Frozenbits_generator_GA* clone() const;

protected:
	void evaluate();
	static double phi    (double t);
	static double phi_inv(double t);
	virtual void check_noise();

	void recursive_override_frozen_bits(const Generic_node<module::Contents_MK_SC<int64_t, double>>* node_curr);
	void recursive_store_DE(const Generic_node<module::Contents_MK_SC<int64_t, double>>* node_curr, double *z) const;

	static double square_plus_DE(const double& ll, const double& lr);
};
}
}

#endif /* FROZENBITS_GENERATOR_GA_HPP_ */
