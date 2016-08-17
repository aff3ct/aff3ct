#include <cmath>

#include "../../Decoder/decoder_functions.h"

#include "Puncturer_polar_wangliu.hpp"

template <typename B, typename Q>
Puncturer_polar_wangliu<B,Q>
::Puncturer_polar_wangliu(const int &N,
                          const int &K,
                          const Frozenbits_generator<B> &fb_generator,
                          const int n_frames,
                          const std::string name)
: Puncturer<B,Q>(n_frames, name),
  N(N), 
  N_code(std::exp2(std::ceil(std::log2(N)))), 
  K(K), 
  fb_generator(fb_generator)
{
}

template <typename B, typename Q>
Puncturer_polar_wangliu<B,Q>
::~Puncturer_polar_wangliu()
{
}

template <typename B, typename Q>
void Puncturer_polar_wangliu<B,Q>
::gen_frozen_bits(mipp::vector<B> &frozen_bits)
{
	const std::vector<int>& best_channels = fb_generator.get_best_channels();

	int info_bits_placed = 0;

	// initialize all bits to frozen
	std::fill(frozen_bits.begin(), frozen_bits.end(), (B)1);

	auto i = 0;
	while (info_bits_placed < K)
	{
 		if (best_channels[i] < N) // choose best channels in interval [0 ; N]
		{                         // interval [0 ; N] are frozen
			frozen_bits[best_channels[i]] = (B)0;
			info_bits_placed++;
		}
		i++;
	}
}

template <typename B, typename Q>
void Puncturer_polar_wangliu<B,Q>
::puncture(const mipp::vector<B>& X_N1, mipp::vector<B>& X_N2) const
{
	assert(X_N1.size() == (unsigned) (N_code * this->n_frames));
	assert(X_N2.size() == (unsigned) (N      * this->n_frames));

	for (auto f = 0; f < this->n_frames; f++)
		std::copy(X_N1.begin() + f * N_code, X_N1.begin() + f * N_code + N, X_N2.begin() + f * N);
}

template <typename B, typename Q>
void Puncturer_polar_wangliu<B,Q>
::depuncture(const mipp::vector<Q>& Y_N1, mipp::vector<Q>& Y_N2) const
{
	assert(Y_N1.size() == (unsigned) (N      * this->n_frames));
	assert(Y_N2.size() == (unsigned) (N_code * this->n_frames));

	for (auto f = 0; f < this->n_frames; f++)
		std::copy(Y_N1.begin() + f * N, Y_N1.begin() + f * N + N, Y_N2.begin() + f * N_code);

	// +inf (bit = 0)
	for (auto f = 0; f < this->n_frames; f++)
		std::fill(Y_N2.begin() + f * N_code + N, Y_N2.begin() + f * N_code + N_code, sat_vals<Q>().second);
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Puncturer_polar_wangliu<B_8,Q_8>;
template class Puncturer_polar_wangliu<B_16,Q_16>;
template class Puncturer_polar_wangliu<B_32,Q_32>;
template class Puncturer_polar_wangliu<B_64,Q_64>;
#else
template class Puncturer_polar_wangliu<B,Q>;
#endif
// ==================================================================================== explicit template instantiation