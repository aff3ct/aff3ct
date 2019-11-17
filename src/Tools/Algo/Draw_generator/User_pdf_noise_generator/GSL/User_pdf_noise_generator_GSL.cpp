#ifdef AFF3CT_CHANNEL_GSL

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Draw_generator/User_pdf_noise_generator/GSL/User_pdf_noise_generator_GSL.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename R>
User_pdf_noise_generator_GSL<R>
::User_pdf_noise_generator_GSL(const tools::Distributions<R>& dists, const int seed, Interpolation_type inter_type)
: User_pdf_noise_generator<R>(dists), rng((void*)gsl_rng_alloc(gsl_rng_mt19937)), interp_function(nullptr)
{
	this->set_seed(seed);

	switch (inter_type)
	{
		case Interpolation_type::LINEAR:
			interp_function = tools::linear_interpolation<R>;
			break;

		case Interpolation_type::NEAREST:
			interp_function = tools::nearest_interpolation<R>;
			break;
	}
}

template <typename R>
User_pdf_noise_generator_GSL<R>
::~User_pdf_noise_generator_GSL()
{
	gsl_rng_free((gsl_rng*)rng);
}

template <typename R>
User_pdf_noise_generator_GSL<R>* User_pdf_noise_generator_GSL<R>
::clone() const
{
	User_pdf_noise_generator_GSL<R>* eg = new User_pdf_noise_generator_GSL(*this);
	eg->rng = (void*)gsl_rng_alloc(gsl_rng_mt19937);
	eg->set_seed((int)gsl_rng_get((gsl_rng*)this->rng));
	return eg;
}

template <typename R>
void User_pdf_noise_generator_GSL<R>
::set_seed(const int seed)
{
	gsl_rng_set((gsl_rng*)rng, seed);
}

template <typename R>
void User_pdf_noise_generator_GSL<R>
::generate(const R* signal, R *draw, const unsigned length, const R noise_power)
{
	auto dis = this->distributions.get_distribution(noise_power);

	for (unsigned i = 0; i < length; i++)
	{
		const auto& cdf_y = signal[i] ? dis.get_cdf_y()[1] : dis.get_cdf_y()[0];
		const auto& cdf_x = signal[i] ? dis.get_cdf_x()[1] : dis.get_cdf_x()[0];
		const auto uni_draw = gsl_ran_flat((gsl_rng*)rng, (R)0, (R)1);
		draw[i] = interp_function(cdf_y.data(), cdf_x.data(), cdf_x.size(), uni_draw);
	}
}

template <typename R>
void User_pdf_noise_generator_GSL<R>
::generate(R *draw, const unsigned length, const R noise_power)
{
	throw unimplemented_error(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::User_pdf_noise_generator_GSL<R_32>;
template class aff3ct::tools::User_pdf_noise_generator_GSL<R_64>;
#else
template class aff3ct::tools::User_pdf_noise_generator_GSL<R>;
#endif
// ==================================================================================== explicit template instantiation


#endif // GSL