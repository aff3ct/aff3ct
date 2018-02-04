#include <vector>
#include <cmath>
#include <iostream>

#include "Encoder_BCH.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_BCH<B>
::Encoder_BCH(const int& K, const int& N, const tools::BCH_polynomial_generator& GF_poly, const int n_frames)
 : Encoder<B>(K, N, n_frames), g(GF_poly.get_g()), bb(N - K)
{
	const std::string name = "Encoder_BCH";
	this->set_name(name);
	
	if ((this->N - this->K) != GF_poly.get_n_rdncy())
	{
		std::stringstream message;
		message << "'N - K' is different than 'n_rdncy' ('K' = " << K << ", 'N' = " << N << ", 'n_rdncy' = "
		        << GF_poly.get_n_rdncy() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	std::iota(this->info_bits_pos.begin(), this->info_bits_pos.end(), this->N - this->K);
}

template <typename B>
Encoder_BCH<B>
::~Encoder_BCH()
{
}

template <typename B>
void Encoder_BCH<B>
::__encode(const B *U_K, B *bb)
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
}

template <typename B>
void Encoder_BCH<B>
::_encode(const B *U_K, B *X_N, const int frame_id)
{
	// generate the parity bits
	this->__encode(U_K, X_N);

	// copy the sys bits at the end of the codeword
	std::copy(U_K, U_K + this->K, X_N + this->N - this->K);
}

template <typename B>
bool Encoder_BCH<B>
::is_codeword(const B *X_N)
{
	this->__encode(X_N + this->N - this->K, this->bb.data());

	auto par_size = this->N - this->K;
	auto p = 0;
	while (p < par_size && (X_N[p] == this->bb[p])) p++;

	return p == par_size;
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
