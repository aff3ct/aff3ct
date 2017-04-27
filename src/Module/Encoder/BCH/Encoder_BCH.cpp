#include <vector>
#include <cmath>

#include "Encoder_BCH.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder_BCH<B>
::Encoder_BCH(const int& K, const int& N, const int& m, const Galois &GF,
              const int n_frames, const std::string name)
 : Encoder<B>(K, N, n_frames, name), m(m), g(N - K + 1), bb(N - K)
{
	// set polynomial coefficients
	this->g = GF.g;
}

template <typename B>
void Encoder_BCH<B>
::_encode(const B *U_K, B *X_N)
{
	for (auto i = 0; i < this->N - this->K; i++)
		bb[i] = (B)0;
	for (auto i = this->K - 1; i >= 0; i--)
	{
		const auto feedback = U_K[i] ^ bb[this->N - this->K - 1];
		if (feedback != 0)
		{
			for (auto j = this->N - this->K - 1; j > 0; j--)
				if (g[j] != 0)
					bb[j] = bb[j - 1] ^ feedback;
				else
					bb[j] = bb[j - 1];
			bb[0] = g[0] && feedback;
		}
		else
		{
			for (auto j = this->N - this->K - 1; j > 0; j--)
				bb[j] = bb[j - 1];
			bb[0] = 0;
		}
	}

	for (auto i = 0; i < this->N - this->K; i++)
		X_N[i] = bb[i];
	for (auto i = 0; i < this->K; i++)
		X_N[i + this->N - this->K] = U_K[i];
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Encoder_BCH<B_8>;
template class aff3ct::module::Encoder_BCH<B_16>;
template class aff3ct::module::Encoder_BCH<B_32>;
template class aff3ct::module::Encoder_BCH<B_64>;
#else
template class aff3ct::module::Encoder_BCH<B>;
#endif
// ==================================================================================== explicit template instantiation
