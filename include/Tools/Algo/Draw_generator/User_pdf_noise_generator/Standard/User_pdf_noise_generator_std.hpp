/*!
 * \file
 * \brief Class tools::User_pdf_noise_generator_std.
 */
#ifndef USER_PDF_NOISE_GENERATOR_STD_HPP_
#define USER_PDF_NOISE_GENERATOR_STD_HPP_

#include <random>

#include "Tools/Math/interpolation.h"
#include "Tools/Algo/Draw_generator/User_pdf_noise_generator/User_pdf_noise_generator.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R = float>
class User_pdf_noise_generator_std : public User_pdf_noise_generator<R>
{
private:
	std::mt19937                      rd_engine; // Mersenne Twister 19937
	std::uniform_real_distribution<R> uniform_dist;

	R (*interp_function)(const R*, const R*, const unsigned, const R);

public:
	explicit User_pdf_noise_generator_std(const tools::Distributions<R>& dists, const int seed = 0, Interpolation_type inter_type = Interpolation_type::NEAREST);
	virtual ~User_pdf_noise_generator_std() = default;
	virtual User_pdf_noise_generator_std<R>* clone() const;

	virtual void set_seed(const int seed);

	virtual void generate(                 R *draw, const unsigned length, const R noise_power);
	virtual void generate(const R* signal, R *draw, const unsigned length, const R noise_power);
};

template <typename R = float>
using User_pdf_gen_std = User_pdf_noise_generator_std<R>;
}
}

#endif /* USER_PDF_NOISE_GENERATOR_STD_HPP_ */
