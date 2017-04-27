#ifdef CHANNEL_MKL

#include <cmath>
#include <stdexcept>
#include <algorithm>

#include "Channel_AWGN_MKL_LLR.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename R>
Channel_AWGN_MKL_LLR<R>
::Channel_AWGN_MKL_LLR(const int N, const R& sigma, const int seed, const int n_frames, const std::string name)
: Channel<R>(N, sigma, n_frames, name)
{
	//vslNewStream(&stream_state, VSL_BRNG_MT2203, seed);
	vslNewStream(&stream_state, VSL_BRNG_SFMT19937, seed);

	if (stream_state == nullptr)
		throw std::runtime_error("aff3ct::module::Channel_AWGN_MKL_LLR: \"stream_state\" can't be null.");
}

template <typename R>
Channel_AWGN_MKL_LLR<R>
::~Channel_AWGN_MKL_LLR()
{
	vslDeleteStream(&stream_state);
}

template <typename R>
void Channel_AWGN_MKL_LLR<R>
::add_noise(const R *X_N, R *Y_N)
{
	throw std::runtime_error("aff3ct::module::Channel_AWGN_MKL_LLR: adding white Gaussian noise is impossible on this "
	                         "type of data.");
}

namespace aff3ct
{
namespace module
{
template <>
void Channel_AWGN_MKL_LLR<float>
::add_noise(const float *X_N, float *Y_N)
{
	vsRngGaussian(VSL_RNG_METHOD_GAUSSIAN_BOXMULLER2,
	              stream_state,
	              this->N * this->n_frames,
	              Y_N,
	              0.0,
	              this->sigma);
	/*
	vsRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF,
	              stream_state,
	              this->N * this->n_frames,
	              Y_N,
	              0.0,
	              this->sigma);
	*/

	for (auto i = 0; i < this->N * this->n_frames; i++)
		Y_N[i] = X_N[i] + Y_N[i];
}
}
}

namespace aff3ct
{
namespace module
{
template <>
void Channel_AWGN_MKL_LLR<double>
::add_noise(const double *X_N, double *Y_N)
{
	vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_BOXMULLER2,
	              stream_state,
	              this->N * this->n_frames,
	              Y_N,
	              0.0,
	              this->sigma);
	/*
	vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF,
	              stream_state,
	              this->N * this->n_frames,
	              Y_N,
	              0.0,
	              this->sigma);
	*/

	for (auto i = 0; i < this->N * this->n_frames; i++)
		Y_N[i] = X_N[i] + Y_N[i];
}
}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Channel_AWGN_MKL_LLR<R_32>;
template class aff3ct::module::Channel_AWGN_MKL_LLR<R_64>;
#else
template class aff3ct::module::Channel_AWGN_MKL_LLR<R>;
#endif
// ==================================================================================== explicit template instantiation

#endif
