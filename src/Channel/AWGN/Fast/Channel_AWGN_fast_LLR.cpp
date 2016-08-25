#include <algorithm>
#include <cassert>

#include "../../../Tools/bash_tools.h"

#include "Channel_AWGN_fast_LLR.hpp"

template <typename R>
Channel_AWGN_fast_LLR<R>
::Channel_AWGN_fast_LLR(const int N, const R& sigma, const int seed, const int n_frames, const std::string name)
: Channel<R>(N, n_frames, name),
  sigma(sigma),
  mt19937(seed),
  mt19937_simd()
{
	assert(sigma != 0);

	mipp::vector<int> seeds(mipp::nElReg<int>());
	for (auto i = 0; i < mipp::nElReg<int>(); i++)
		seeds[i] = mt19937.rand();
	mt19937_simd.seed(seeds.data());
}

template <typename R>
Channel_AWGN_fast_LLR<R>
::~Channel_AWGN_fast_LLR()
{
}

template <typename R>
mipp::Reg<R> Channel_AWGN_fast_LLR<R>
::get_random_simd()
{
	std::cerr << bold_red("(EE) The MT19937 random generator does not support this type.") << std::endl;
	std::exit(-1);
}

template <typename R>
R Channel_AWGN_fast_LLR<R>
::get_random()
{
	std::cerr << bold_red("(EE) The MT19937 random generator does not support this type.") << std::endl;
	std::exit(-1);
}

template <>
mipp::Reg<float> Channel_AWGN_fast_LLR<float>
::get_random_simd()
{
	// return a vector of numbers between ]0,1[
	return mt19937_simd.randf_oo();
}

template <>
float Channel_AWGN_fast_LLR<float>
::get_random()
{
	// return a number between ]0,1[
	return mt19937.randf_oo();
}

template <typename R>
void Channel_AWGN_fast_LLR<R>
::add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N)
{
	assert(X_N.size() == Y_N.size());

	const auto twopi = (R)(2.0 * 3.14159265358979323846);

	// SIMD version of the Box Muller method in the polar form
	const auto loop_size = (int)Y_N.size();
	const auto vec_loop_size = (int)((loop_size / (mipp::nElReg<R>() * 2)) * mipp::nElReg<R>() * 2);
	for (auto i = 0; i < vec_loop_size; i += mipp::nElReg<R>() * 2) 
	{
		const auto u1 = get_random_simd();
		const auto u2 = get_random_simd();

		const auto radius = mipp::sqrt(mipp::log(u1) * (R)-2.0) * sigma;
		const auto theta  = u2 * twopi;

		mipp::Reg<R> sintheta, costheta;
		mipp::sincos(theta, sintheta, costheta);

		// fmadd(a, b, c) = a * b + c
		auto awgn1 = mipp::fmadd(radius, costheta, mipp::Reg<R>(&X_N[i                    ]));
		auto awgn2 = mipp::fmadd(radius, sintheta, mipp::Reg<R>(&X_N[i + mipp::nElReg<R>()]));

		awgn1.store(&Y_N[i                    ]);
		awgn2.store(&Y_N[i + mipp::nElReg<R>()]);
	}

	// seq version of the Box Muller method in the polar form
	const auto seq_loop_size = (int)(loop_size / 2) * 2;
	for (auto i = vec_loop_size; i < seq_loop_size; i += 2)
	{
		const auto u1 = get_random();
		const auto u2 = get_random();

		const auto radius = (R)std::sqrt(std::log(u1) * (R)-2.0) * sigma;
		const auto theta  = u2 * twopi;

		const auto sintheta = std::sin(theta);
		const auto costheta = std::cos(theta);

		Y_N[i +0] = radius * sintheta + X_N[i +0];
		Y_N[i +1] = radius * costheta + X_N[i +1];
	}

	// distribute the last odd element
	if (loop_size != seq_loop_size)
	{
		const auto u1 = get_random();
		const auto u2 = get_random();

		const auto radius = (R)std::sqrt(std::log(u1) * (R)-2.0) * sigma;
		const auto theta  = twopi * u2;

		const auto sintheta = std::sin(theta);

		Y_N[loop_size -1] = radius * sintheta + X_N[loop_size -1];
	}	

}

// ==================================================================================== explicit template instantiation 
#include "../../../Tools/types.h"
#ifdef MULTI_PREC
template class Channel_AWGN_fast_LLR<R_32>;
template class Channel_AWGN_fast_LLR<R_64>;
#else
template class Channel_AWGN_fast_LLR<R>;
#endif
// ==================================================================================== explicit template instantiation