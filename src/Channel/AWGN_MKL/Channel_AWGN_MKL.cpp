#ifdef CHANNEL_MKL

#include <cmath>
#include <algorithm>
#include <cassert>

#include "Channel_AWGN_MKL.hpp"

#include "../../Tools/bash_tools.h"

#ifndef VSL_METHOD_SGAUSSIAN_BOXMULLER2
	#define VSL_METHOD_SGAUSSIAN_BOXMULLER2 1
#endif

template <typename B, typename R>
Channel_AWGN_MKL<B,R>
::Channel_AWGN_MKL(const R& sigma, const int seed, const R& scaling_factor)
: sigma(sigma),
  scaling_factor(scaling_factor)
{
	//vslNewStream(&stream_state, VSL_BRNG_MT2203, seed);
	vslNewStream(&stream_state, VSL_BRNG_SFMT19937, seed);	
}

template <typename B, typename R>
Channel_AWGN_MKL<B,R>
::~Channel_AWGN_MKL()
{
	vslDeleteStream(&stream_state);
}

template <typename B, typename R>
struct MKL_gaussian_noise
{
	static void generate(VSLStreamStatePtr &stream_state, mipp::vector<R>& Y_N, const R &sigma)
	{
		std::cerr << bold_red("(EE) Adding white Gaussian noise is impossible on this type of data.") << std::endl;
		exit(-1);
	}
};

template <typename B>
struct MKL_gaussian_noise <B, float>
{
	static void generate(VSLStreamStatePtr &stream_state, mipp::vector<float>& Y_N, const float &sigma)
	{
		/*
		vsRngGaussian(VSL_METHOD_SGAUSSIAN_BOXMULLER2,
		              stream_state,
		              Y_N.size(),
		              Y_N.data(),
		              0.0,
		              sigma);
		*/

		vsRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF,
		              stream_state,
		              Y_N.size(),
		              Y_N.data(),
		              0.0,
		              sigma);
	}
};

template <typename B>
struct MKL_gaussian_noise <B, double>
{
	static void generate(VSLStreamStatePtr &stream_state, mipp::vector<double>& Y_N, const double &sigma)
	{
		/*
		vdRngGaussian(VSL_METHOD_SGAUSSIAN_BOXMULLER2,
		              stream_state,
		              Y_N.size(),
		              Y_N.data(),
		              0.0,
		              sigma);
		*/

		vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF,
		              stream_state,
		              Y_N.size(),
		              Y_N.data(),
		              0.0,
		              sigma);
	}
};

template <typename B, typename R>
void Channel_AWGN_MKL<B,R>
::add_gaussian_noise(const mipp::vector<B>& X_N, mipp::vector<R>& Y_N)
{
	assert(X_N.size()   == Y_N.size());
	assert(sigma        != 0         );
	assert(stream_state != nullptr   );

	MKL_gaussian_noise<B,R>::generate(stream_state, Y_N, sigma);

	auto size = Y_N.size();
	for (unsigned i = 0; i < size; i++)
		Y_N[i] = ((R)X_N[i] + Y_N[i]) * scaling_factor;
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Channel_AWGN_MKL<B_8,R_8>;
template class Channel_AWGN_MKL<B_16,R_16>;
template class Channel_AWGN_MKL<B_32,R_32>;
template class Channel_AWGN_MKL<B_64,R_64>;
#else
template class Channel_AWGN_MKL<B,R>;
#endif
// ==================================================================================== explicit template instantiation

#endif