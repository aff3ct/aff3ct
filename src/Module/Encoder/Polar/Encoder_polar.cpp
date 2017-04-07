#include <stdexcept>
#include <vector>
#include <cmath>

#include "Encoder_polar.hpp"

using namespace aff3ct::module;

template <typename B>
Encoder_polar<B>
::Encoder_polar(const int& K, const int& N, const mipp::vector<B>& frozen_bits, const int n_frames, 
                const std::string name)
: Encoder<B>(K, N, n_frames, name), m((int)std::log2(N)), frozen_bits(frozen_bits)
{
	if (this->N != (int)frozen_bits.size())
		throw std::length_error("aff3ct::module::Encoder_polar: \"frozen_bits.size()\" has to be equal to "
		                        "\"N\".");
}

template <typename B>
void Encoder_polar<B>
::_encode(const B *U_K, B *X_N)
{
	this->convert(U_K, X_N);
	this->light_encode(X_N);
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
::convert(const B *U_K, B *U_N)
{
	auto j = 0;
	for (unsigned i = 0; i < frozen_bits.size(); i++)
		U_N[i] = (frozen_bits[i]) ? (B)0 : U_K[j++];
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Encoder_polar<B_8>;
template class aff3ct::module::Encoder_polar<B_16>;
template class aff3ct::module::Encoder_polar<B_32>;
template class aff3ct::module::Encoder_polar<B_64>;
#else
template class aff3ct::module::Encoder_polar<B>;
#endif
// ==================================================================================== explicit template instantiation
