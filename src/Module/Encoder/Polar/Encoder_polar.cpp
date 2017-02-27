#include <cassert>
#include <vector>
#include <cmath>

#include "Encoder_polar.hpp"

using namespace aff3ct::module;

template <typename B>
Encoder_polar<B>
::Encoder_polar(const int& K, const int& N, const mipp::vector<B>& frozen_bits, const int n_frames, 
                const std::string name)
: Encoder<B>(K, N, n_frames, name), m((int)std::log2(N)), frozen_bits(frozen_bits), U_N(N * n_frames)
{
	assert(frozen_bits.size() == (unsigned) N);
	assert(this->n_frames > 0);
}

template <typename B>
void Encoder_polar<B>
::encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N)
{
	assert(U_K.size() == (unsigned) (this->K * this->n_frames));
	assert(X_N.size() == (unsigned) (this->N * this->n_frames));

	this->convert(U_K, U_N);
	for (auto i_frame = 0; i_frame < this->n_frames; i_frame++)
		frame_encode(U_N, X_N, i_frame); // frame encode
}

template <typename B>
void Encoder_polar<B>
::frame_encode(const mipp::vector<B>& U_N, mipp::vector<B>& X_N, const int &i_frame)
{
	const auto offset = i_frame * this->N;

	for (auto i = 0; i < this->N; i++) X_N[offset +i] = U_N[offset +i];

	light_encode(X_N.data() + offset);
}

template <typename B>
void Encoder_polar<B>
::light_encode(B *bits)
{
	for (auto k = (this->N >> 1); k > 0; k >>= 1)
		for (auto j = 0; j < this->N; j += 2 * k)
			for (auto i = 0; i < k; i++)
				bits[j + i] = bits[j + i] ^ bits[k + j + i];
}

template <typename B>
void Encoder_polar<B>
::convert(const mipp::vector<B>& U_K, mipp::vector<B>& U_N)
{
	assert(U_K.size() == (unsigned) (this->K * this->n_frames));
	assert(U_N.size() == (unsigned) (this->N * this->n_frames));

	for (auto f = 0; f < this->n_frames; f++)
	{
		const auto offset_U_k = f * this->K;
		const auto offset_U_n = f * this->N;

		auto j = 0;
		for (unsigned i = 0; i < frozen_bits.size(); i++)
			U_N[offset_U_n +i] = (frozen_bits[i]) ? (B)0 : U_K[offset_U_k + j++];
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Encoder_polar<B_8>;
template class Encoder_polar<B_16>;
template class Encoder_polar<B_32>;
template class Encoder_polar<B_64>;
#else
template class Encoder_polar<B>;
#endif
// ==================================================================================== explicit template instantiation
