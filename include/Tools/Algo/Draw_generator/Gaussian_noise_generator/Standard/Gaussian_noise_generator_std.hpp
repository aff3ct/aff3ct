/*!
 * \file
 * \brief Class tools::Gaussian_noise_generator_std.
 */
#ifndef GAUSSIAN_NOISE_GENERATOR_STD_HPP_
#define GAUSSIAN_NOISE_GENERATOR_STD_HPP_

#include <random>

#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/Gaussian_noise_generator.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float>
class Gaussian_noise_generator_std : public Gaussian_noise_generator<R>
{
private:
//	std::minstd_rand            rd_engine; // LCG
	std::mt19937                rd_engine; // Mersenne Twister 19937
	std::normal_distribution<R> normal_dist;

public:
	explicit Gaussian_noise_generator_std(const int seed = 0);
	virtual ~Gaussian_noise_generator_std() = default;
	virtual Gaussian_noise_generator_std<R>* clone() const;

	virtual void set_seed(const int seed);
	virtual void generate(R *noise, const unsigned length, const R sigma, const R mu = 0.0);
};

template <typename R = float>
using Gaussian_gen_std = Gaussian_noise_generator_std<R>;
}
}

#endif /* GAUSSIAN_NOISE_GENERATOR_STD_HPP_ */
