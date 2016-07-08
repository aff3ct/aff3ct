#include <algorithm>
#include <cassert>

#include "../../../Tools/bash_tools.h"

#include "Channel_AWGN_fast_LLR.hpp"

template <typename R>
Channel_AWGN_fast_LLR<R>
::Channel_AWGN_fast_LLR(const R& sigma, const int seed, const R scaling_factor)
: sigma(sigma),
  scaling_factor(scaling_factor),
  mt19937(seed),
  mt19937_simd()
{
	assert(scaling_factor != 0);
	assert(sigma          != 0);

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
::get_random_fast()
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
::get_random_fast()
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
	const mipp::Reg<R> r_sf       = scaling_factor;
	const mipp::Reg<R> r_sigma    = sigma;
	const mipp::Reg<R> r_twopi    = twopi;
	const mipp::Reg<R> r_minustwo = -2.0;

	const auto loop_size = (int)Y_N.size();
	const auto vec_loop_size = (int)((loop_size / mipp::nElReg<R>()) * mipp::nElReg<R>());
	for (auto i = 0; i < vec_loop_size; i += mipp::nElReg<R>() * 2) 
	{
		const auto r_u1 = get_random_fast();
		const auto r_u2 = get_random_fast();

		const auto r_radius = mipp::sqrt(r_minustwo * mipp::log(r_u1)) * r_sigma;
		const auto r_theta  = r_twopi * r_u2;

		mipp::Reg<R> r_sintheta, r_costheta;
		mipp::sincos(r_theta, r_sintheta, r_costheta);

		// fmadd(a, b, c) = a * b + c
		auto r_awgn1 = mipp::fmadd(r_radius, r_costheta, mipp::Reg<R>(&X_N[i                    ])) * r_sf;
		auto r_awgn2 = mipp::fmadd(r_radius, r_sintheta, mipp::Reg<R>(&X_N[i + mipp::nElReg<R>()])) * r_sf;

		r_awgn1.store(&Y_N[i                    ]);
		r_awgn2.store(&Y_N[i + mipp::nElReg<R>()]);
	}

	// seq version of the Box Muller method in the polar form
	const auto seq_loop_size = (int)(loop_size / 2) * 2;
	for (auto i = vec_loop_size; i < seq_loop_size; i += 2)
	{
		const auto u1 = get_random();
		const auto u2 = get_random();

		const auto radius = (R)std::sqrt((R)-2.0 * std::log(u1)) * sigma;
		const auto theta  = twopi * u2;

		const auto sintheta = std::sin(theta);
		const auto costheta = std::cos(theta);

		Y_N[i +0] = (radius * sintheta + X_N[i +0]) * scaling_factor;
		Y_N[i +1] = (radius * costheta + X_N[i +1]) * scaling_factor;
	}

	// distribute the last odd element
	if (loop_size != seq_loop_size)
	{
		const auto u1 = get_random();
		const auto u2 = get_random();

		const auto radius = (R)std::sqrt((R)-2.0 * std::log(u1)) * sigma;
		const auto theta  = twopi * u2;

		const auto sintheta = std::sin(theta);

		Y_N[loop_size -1] = (radius * sintheta + X_N[loop_size -1]) * scaling_factor;
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