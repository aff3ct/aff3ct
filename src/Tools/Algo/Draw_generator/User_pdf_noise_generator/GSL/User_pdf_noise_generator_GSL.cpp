#ifdef CHANNEL_GSL

#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "User_pdf_noise_generator_GSL.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename R>
User_pdf_noise_generator_GSL<R>
::User_pdf_noise_generator_GSL(const tools::Distributions<R>& dists, const int seed, Interpolation_type inter_type)
: User_pdf_noise_generator<R>(dists), rng(gsl_rng_alloc(gsl_rng_mt19937), gsl_rng_free), interp_function(nullptr)
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
void User_pdf_noise_generator_GSL<R>
::set_seed(const int seed)
{
	gsl_rng_set(rng.get(), seed);
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
		const auto uni_draw = gsl_ran_flat(rng.get(), (R)0, (R)1);
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
#ifdef MULTI_PREC
template class aff3ct::tools::User_pdf_noise_generator_GSL<R_32>;
template class aff3ct::tools::User_pdf_noise_generator_GSL<R_64>;
#else
template class aff3ct::tools::User_pdf_noise_generator_GSL<R>;
#endif
// ==================================================================================== explicit template instantiation


#endif // GSL