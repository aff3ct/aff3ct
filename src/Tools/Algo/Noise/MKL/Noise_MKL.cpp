#ifdef CHANNEL_MKL

#include <stdexcept>

#include "Noise_MKL.hpp"

using namespace aff3ct::tools;

template <typename R>
Noise_MKL<R>
::Noise_MKL(const int seed)
: Noise<R>(), stream_state(nullptr), is_stream_alloc(false)
{
	this->set_seed(seed);
}

template <typename R>
Noise_MKL<R>
::~Noise_MKL()
{
	if (is_stream_alloc)
		vslDeleteStream(&stream_state);
}

template <typename R>
void Noise_MKL<R>
::set_seed(const int seed)
{
	if (is_stream_alloc) vslDeleteStream(&stream_state);

	//vslNewStream(&stream_state, VSL_BRNG_MT2203, seed);
	vslNewStream(&stream_state, VSL_BRNG_SFMT19937, seed);

	is_stream_alloc = true;
}

template <typename R>
void Noise_MKL<R>
::generate(R *noise, const unsigned length, const R sigma)
{
	throw std::runtime_error("aff3ct::module::Noise_MKL: adding white Gaussian noise is impossible on this "
	                         "type of data.");
}

namespace aff3ct
{
namespace tools
{
template <>
void Noise_MKL<float>
::generate(float *noise, const unsigned length, const float sigma)
{
	vsRngGaussian(VSL_RNG_METHOD_GAUSSIAN_BOXMULLER2,
	              stream_state,
	              length,
	              noise,
	              0.0,
	              sigma);
	/*
	vsRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF,
	              stream_state,
	              length,
	              noise,
	              0.0,
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
void Noise_MKL<double>
::generate(double *noise, const unsigned length, const double sigma)
{
	vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_BOXMULLER2,
	              stream_state,
	              length,
	              noise,
	              0.0,
	              sigma);
	/*
	vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF,
	              stream_state,
	              length,
	              noise,
	              0.0,
	              sigma);
	*/
}
}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Noise_MKL<R_32>;
template class aff3ct::tools::Noise_MKL<R_64>;
#else
template class aff3ct::tools::Noise_MKL<R>;
#endif
// ==================================================================================== explicit template instantiation

#endif
