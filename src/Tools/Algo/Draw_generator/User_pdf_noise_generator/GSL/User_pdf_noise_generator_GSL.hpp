#ifdef CHANNEL_GSL

#ifndef User_pdf_noise_generator_GSL_HPP_
#define User_pdf_noise_generator_GSL_HPP_

#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>
#include <memory>

#include "Tools/Math/interpolation.h"

#include "../User_pdf_noise_generator.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R = float>
class User_pdf_noise_generator_GSL : public User_pdf_noise_generator<R>
{
private:
	std::unique_ptr<gsl_rng,decltype(&gsl_rng_free)> rng;

	R (*interp_function)(const R*, const R*, const unsigned, const R);

public:
	explicit User_pdf_noise_generator_GSL(const tools::Distributions<R>& dists, const int seed = 0, Interpolation_type inter_type = Interpolation_type::NEAREST);
	virtual ~User_pdf_noise_generator_GSL() = default;

	virtual void set_seed(const int seed);

	virtual void generate(                 R *draw, const unsigned length, const R noise_power);
	virtual void generate(const R* signal, R *draw, const unsigned length, const R noise_power);
};

template <typename R = float>
using User_pdf_gen_GSL = User_pdf_noise_generator_GSL<R>;
}
}

#endif /* User_pdf_noise_generator_GSL_HPP_ */


#endif // GSL