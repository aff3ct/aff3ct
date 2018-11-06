#ifdef AFF3CT_CHANNEL_MKL

#include "Tools/Exception/exception.hpp"

#include "Gaussian_noise_generator_MKL.hpp"

using namespace aff3ct::tools;

template <typename R>
Gaussian_noise_generator_MKL<R>
::Gaussian_noise_generator_MKL(const int seed)
: Gaussian_noise_generator<R>(), stream_state(nullptr), is_stream_alloc(false)
{
	this->set_seed(seed);
}

template <typename R>
Gaussian_noise_generator_MKL<R>
::~Gaussian_noise_generator_MKL()
{
	if (is_stream_alloc)
		vslDeleteStream(&stream_state);
}

template <typename R>
void Gaussian_noise_generator_MKL<R>
::set_seed(const int seed)
{
	if (is_stream_alloc) vslDeleteStream(&stream_state);

	//vslNewStream(&stream_state, VSL_BRNG_MT2203, seed);
	vslNewStream(&stream_state, VSL_BRNG_SFMT19937, seed);

	is_stream_alloc = true;
}

template <typename R>
void Gaussian_noise_generator_MKL<R>
::generate(R *noise, const unsigned length, const R sigma, const R mu)
{
	throw runtime_error(__FILE__, __LINE__, __func__, "Adding white Gaussian noise is impossible on this data type.");
}

namespace aff3ct
{
namespace tools
{
template <>
void Gaussian_noise_generator_MKL<float>
::generate(float *noise, const unsigned length, const float sigma, const float mu)
{
	vsRngGaussian(VSL_RNG_METHOD_GAUSSIAN_BOXMULLER2,
	              stream_state,
	              length,
	              noise,
	              mu,
	              sigma);
	/*
	vsRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF,
	              stream_state,
	              length,
	              noise,
	              mu,
	              sigma);
	*/
}
}
}

namespace aff3ct
{
namespace tools
{
template <>
void Gaussian_noise_generator_MKL<double>
::generate(double *noise, const unsigned length, const double sigma, const double mu)
{
	vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_BOXMULLER2,
	              stream_state,
	              length,
	              noise,
	              mu,
	              sigma);
	/*
	vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF,
	              stream_state,
	              length,
	              noise,
	              mu,
	              sigma);
	*/
}
}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Gaussian_noise_generator_MKL<R_32>;
template class aff3ct::tools::Gaussian_noise_generator_MKL<R_64>;
#else
template class aff3ct::tools::Gaussian_noise_generator_MKL<R>;
#endif
// ==================================================================================== explicit template instantiation

#endif
