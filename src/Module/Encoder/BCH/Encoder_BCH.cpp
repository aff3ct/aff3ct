#include <numeric>
#include <sstream>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Module/Encoder/BCH/Encoder_BCH.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_BCH<B>
::Encoder_BCH(const int& K,
              const int& N,
              const tools::BCH_polynomial_generator<B>& GF_poly)
 : Encoder<B>(K, N), n_rdncy(GF_poly.get_n_rdncy()), g(GF_poly.get_g()), bb(n_rdncy)
{
	const std::string name = "Encoder_BCH";
	this->set_name(name);

	if ((this->N - this->K) != n_rdncy)
	{
		std::stringstream message;
		message << "'N - K' is different than 'n_rdncy' ('K' = " << K << ", 'N' = " << N
		        << ", 'n_rdncy' = " << n_rdncy << ", 'N - K' = " << (this->N - this->K) << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	std::iota(this->info_bits_pos.begin(), this->info_bits_pos.end(), n_rdncy); // redundancy on the first 'n_rdncy' bits
}

template <typename B>
Encoder_BCH<B>* Encoder_BCH<B>
::clone() const
{
	auto m = new Encoder_BCH(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
void Encoder_BCH<B>
::__encode(const B *U_K, B *par)
{
	std::fill(par, par + n_rdncy, (B)0);

	for (auto i = this->K - 1; i >= 0; i--)
	{
		const auto feedback = U_K[i] ^ par[n_rdncy - 1];
		for (auto j = n_rdncy - 1; j > 0; j--)
			par[j] = par[j - 1] ^ (g[j] & feedback);
		par[0] = feedback ? g[0] && feedback : 0;
	}
}

template <typename B>
void Encoder_BCH<B>
::_encode(const B *U_K, B *X_N, const size_t frame_id)
{
	// generate the parity bits
	this->__encode(U_K, X_N);

	// copy the sys bits at the end of the codeword
	std::copy(U_K, U_K + this->K, X_N + n_rdncy);
}

template <typename B>
bool Encoder_BCH<B>
::is_codeword(const B *X_N)
{
	this->__encode(X_N + n_rdncy, this->bb.data());

	auto p = 0;
	while (p < n_rdncy && (X_N[p] == this->bb[p])) p++;

	return p == n_rdncy;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_BCH<B_8>;
template class aff3ct::module::Encoder_BCH<B_16>;
template class aff3ct::module::Encoder_BCH<B_32>;
template class aff3ct::module::Encoder_BCH<B_64>;
#else
template class aff3ct::module::Encoder_BCH<B>;
#endif
// ==================================================================================== explicit template instantiation
