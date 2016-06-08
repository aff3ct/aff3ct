#include <cmath>

#include "../../../Decoder/decoder_functions.h"

#include "Puncturer_polar_wangliu.hpp"

template <typename B, typename R>
Puncturer_polar_wangliu<B,R>
::Puncturer_polar_wangliu(const int & N, const int & K, const Frozenbits_generator<B> &fb_generator)
: N(N), 
  N_2(std::exp2(std::ceil(std::log2(N)))), 
  Np(N_2 - N), 
  K(K), 
  puncturing_pattern(N_2),
  fb_generator(fb_generator)
{
	std::fill(puncturing_pattern.begin()           , puncturing_pattern.begin() + N_2 - Np, 0);
	std::fill(puncturing_pattern.begin() + N_2 - Np, puncturing_pattern.end()             , 1);
}

template <typename B, typename R>
Puncturer_polar_wangliu<B,R>
::~Puncturer_polar_wangliu()
{
}

template <typename B, typename R>
void Puncturer_polar_wangliu<B,R>
::gen_frozen_bits(mipp::vector<B>& frozen_bits)
{
	const std::vector<int>& best_channels = fb_generator.get_best_channels();

	int info_bits_placed = 0;

	// initialize all bits to frozen
	for (auto i = 0 ; i < N_2 ; i++)
		frozen_bits[i] = (B)1;

	auto i = 0;
	while (info_bits_placed < K)
	{
 		if (best_channels[i] < N_2 - Np) // choose best channels in interval [0 ; N_2 - Np]
		{                                // interval [0 ; N_2 - Np] are frozen
			frozen_bits[best_channels[i]] = (B)0;
			info_bits_placed++;
		}
		i++;
	}
}

template <typename B, typename R>
void Puncturer_polar_wangliu<B,R>
::puncture(mipp::vector<R>& Y_N) const
{
	for (int i = 0 ; i < N_2 ; i++)
		if (puncturing_pattern[i])
			Y_N[i] = sat_vals<R>().second;
}

// ==================================================================================== explicit template instantiation 
#include "../../types.h"
#ifdef MULTI_PREC
template class Puncturer_polar_wangliu<B_8,Q_8>;
template class Puncturer_polar_wangliu<B_16,Q_16>;
template class Puncturer_polar_wangliu<B_32,Q_32>;
template class Puncturer_polar_wangliu<B_64,Q_64>;
#else
template class Puncturer_polar_wangliu<B,Q>;
#endif
// ==================================================================================== explicit template instantiation