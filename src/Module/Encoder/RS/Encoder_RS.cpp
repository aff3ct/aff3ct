#include <string>
#include <sstream>
#include <climits>
#include <numeric>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Bit_packer.hpp"
#include "Module/Encoder/RS/Encoder_RS.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_RS<B>
::Encoder_RS(const int& K, const int& N, const tools::RS_polynomial_generator& GF)
: Encoder<B>(K * GF.get_m(), N * GF.get_m()),
  K_rs        (K                   ),
  N_rs        (N                   ),
  m           (GF.get_m()          ),
  n_rdncy_bits(GF.get_n_rdncy() * m),
  n_rdncy     (GF.get_n_rdncy()    ),
  alpha_to    (GF.get_alpha_to()   ),
  index_of    (GF.get_index_of()   ),
  g           (GF.get_g()          ),
  bb          (n_rdncy             ),
  packed_U_K  (K_rs                ),
  packed_X_N  (N_rs                )
{
	const std::string name = "Encoder_RS";
	this->set_name(name);

	if ((this->N_rs - this->K_rs) != n_rdncy)
	{
		std::stringstream message;
		message << "'N_rs - K_rs' is different than 'n_rdncy' ('K_rs' = " << K_rs << ", 'N_rs' = " << N_rs
		        << ", 'n_rdncy' = " << n_rdncy << ", 'N_rs - K_rs' = " << (this->N_rs - this->K_rs) << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}


	if ((int)sizeof(S) * CHAR_BIT < GF.get_m())
	{
		std::stringstream message;
		message << "The given Galois Field is too big to be stored in a 'S' type ('S' = " << typeid(S).name()
		        << ", 'GF.get_m()' = " << GF.get_m() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	std::iota(this->info_bits_pos.begin(), this->info_bits_pos.end(), n_rdncy); // redundancy on the first 'n_rdncy' bits
}

template <typename B>
Encoder_RS<B>* Encoder_RS<B>
::clone() const
{
	auto m = new Encoder_RS(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
void Encoder_RS<B>
::__encode(const S *U_K, S *par)
{
	std::fill(par, par + this->n_rdncy, (S)0);

	for (auto i = this->K_rs - 1; i >= 0; i--)
	{
		const auto feedback = this->index_of[U_K[i] ^ par[this->n_rdncy - 1]];

		if (feedback != -1)
		{
			for (auto j = this->n_rdncy - 1; j > 0; j--)
				if (this->g[j] != -1)
					par[j] = par[j - 1] ^ this->alpha_to[(this->g[j] + feedback) % this->N_rs];
				else
					par[j] = par[j - 1];
			par[0] = this->alpha_to[(this->g[0] + feedback) % this->N_rs];
		}
		else
		{
			for (auto j = this->n_rdncy - 1; j > 0; j--)
				par[j] = par[j - 1];
			par[0] = 0;
		}
	}
}

template <typename B>
void Encoder_RS<B>
::_encode(const B *U_K, B *X_N, const size_t frame_id)
{
	tools::Bit_packer::pack(U_K, this->packed_U_K.data(), this->K, 1, false, this->m);

	// generate the parity bits
	this->__encode(this->packed_U_K.data(), this->bb.data());

	tools::Bit_packer::unpack(this->bb.data(), X_N, this->n_rdncy_bits, 1, false, this->m);

	// copy the sys bits at the end of the codeword
	std::copy(U_K, U_K + this->K, X_N + this->n_rdncy_bits);
}

template <typename B>
bool Encoder_RS<B>
::is_codeword(const B *X_N)
{
	tools::Bit_packer::pack(X_N, this->packed_X_N.data(), this->N, 1, false, this->m);

	this->__encode(this->packed_X_N.data() + this->n_rdncy, this->bb.data()); // encode the systematic bits

	auto p = 0;
	while (p < this->n_rdncy && (this->packed_X_N[p] == this->bb[p])) p++; // compare the parity parts

	return p == this->n_rdncy;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_RS<B_8>;
template class aff3ct::module::Encoder_RS<B_16>;
template class aff3ct::module::Encoder_RS<B_32>;
template class aff3ct::module::Encoder_RS<B_64>;
#else
template class aff3ct::module::Encoder_RS<B>;
#endif
// ==================================================================================== explicit template instantiation
