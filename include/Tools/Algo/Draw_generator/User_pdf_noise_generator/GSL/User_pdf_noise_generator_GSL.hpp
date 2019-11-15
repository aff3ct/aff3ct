/*!
 * \file
 * \brief Class tools::User_pdf_noise_generator_fast.
 */
#ifdef AFF3CT_CHANNEL_GSL

#ifndef User_pdf_noise_generator_GSL_HPP_
#define User_pdf_noise_generator_GSL_HPP_

#include "Tools/Math/interpolation.h"
#include "Tools/Algo/Draw_generator/User_pdf_noise_generator/User_pdf_noise_generator.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R = float>
class User_pdf_noise_generator_GSL : public User_pdf_noise_generator<R>
{
private:
	void* rng; // gsl_rng* type

	R (*interp_function)(const R*, const R*, const unsigned, const R);

public:
	explicit User_pdf_noise_generator_GSL(const tools::Distributions<R>& dists, const int seed = 0, Interpolation_type inter_type = Interpolation_type::NEAREST);
	virtual ~User_pdf_noise_generator_GSL();
	virtual User_pdf_noise_generator_GSL<R>* clone() const;

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