#include <cassert>
#include <vector>
#include <cmath>
#include <iostream>

#include "Encoder_polar_sys.hpp"

template <typename B>
Encoder_polar_sys<B>
::Encoder_polar_sys(const int& K, const int& N, const mipp::vector<B>& frozen_bits, const int& n_frames,
                    const std::string name)
: Encoder_polar<B>(K, N, frozen_bits, n_frames, name)
{
}

template <typename B>
void Encoder_polar_sys<B>
::encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N)
{
	assert(U_K.size() == (unsigned) (this->K * this->n_frames));
	assert(X_N.size() == (unsigned) (this->N * this->n_frames));

	this->convert(U_K, this->U_N);

	for (auto i_frame = 0; i_frame < this->n_frames; i_frame++)
	{
		// first time encode
		this->frame_encode(this->U_N, X_N, i_frame);

		const auto offset_X_N = i_frame * this->N;

		for (auto i = 0; i < this->N; i++)
			X_N[offset_X_N +i] = !this->frozen_bits[i] && X_N[offset_X_N +i];

		// second time encode because of systematic encoder
		this->frame_encode(X_N, X_N, i_frame);
	}
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Encoder_polar_sys<B_8>;
template class Encoder_polar_sys<B_16>;
template class Encoder_polar_sys<B_32>;
template class Encoder_polar_sys<B_64>;
#else
template class Encoder_polar_sys<B>;
#endif
// ==================================================================================== explicit template instantiation