#ifdef CHANNEL_MKL

#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "User_pdf_noise_generator_MKL.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename R>
User_pdf_noise_generator_MKL<R>
::User_pdf_noise_generator_MKL(const tools::Distributions<R>& dists, const int seed, Interpolation_type inter_type)
: User_pdf_noise_generator<R>(dists), stream_state(nullptr), is_stream_alloc(false), interp_function(nullptr)
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
void User_pdf_noise_generator_MKL<R>
::set_seed(const int seed)
{
	if (is_stream_alloc) vslDeleteStream(&stream_state);

	//vslNewStream(&stream_state, VSL_BRNG_MT2203, seed);
	vslNewStream(&stream_state, VSL_BRNG_SFMT19937, seed);

	is_stream_alloc = true;
}

template <typename R>
void User_pdf_noise_generator_MKL<R>
::generate(const R* signal, R *draw, const unsigned length, const R noise_power)
{
	auto dis = this->distributions.get_distribution(noise_power);

	vsRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream_state, length, draw, (R)0, (R)1);


	for (unsigned i = 0; i < length; i++)
	{
		const auto& cdf_y = signal[i] ? dis.get_cdf_y()[1] : dis.get_cdf_y()[0];
		const auto& cdf_x = signal[i] ? dis.get_cdf_x()[1] : dis.get_cdf_x()[0];
		draw[i] = interp_function(cdf_y.data(), cdf_x.data(), cdf_x.size(), draw[i]);
	}
}

#if defined(MULTI_PREC) | defined(PREC_64_BIT)

namespace aff3ct
{
namespace tools
{

template <>
void User_pdf_noise_generator_MKL<double>
::generate(const double* signal, double *draw, const unsigned length, const double noise_power)
{
	auto dis = this->distributions.get_distribution(noise_power);

	vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream_state, length, draw, (double)0, (double)1);


	for (unsigned i = 0; i < length; i++)
	{
		const auto& cdf_y = signal[i] ? dis.get_cdf_y()[1] : dis.get_cdf_y()[0];
		const auto& cdf_x = signal[i] ? dis.get_cdf_x()[1] : dis.get_cdf_x()[0];
		draw[i] = interp_function(cdf_y.data(), cdf_x.data(), cdf_x.size(), draw[i]);
	}
}
}
}

#endif

template <typename R>
void User_pdf_noise_generator_MKL<R>
::generate(R *draw, const unsigned length, const R noise_power)
{
	throw unimplemented_error(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::User_pdf_noise_generator_MKL<R_32>;
template class aff3ct::tools::User_pdf_noise_generator_MKL<R_64>;
#else
template class aff3ct::tools::User_pdf_noise_generator_MKL<R>;
#endif
// ==================================================================================== explicit template instantiation

#endif // MKL
