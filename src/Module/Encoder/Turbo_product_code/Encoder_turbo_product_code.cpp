#include <vector>
#include <cmath>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Encoder_turbo_product_code.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_turbo_product_code<B>
::Encoder_turbo_product_code(const Interleaver<B> &pi, Encoder<B> &enc_r, Encoder<B> &enc_c)
: Encoder<B>(enc_r.get_K() * enc_c.get_K(), pi.get_core().get_size(), enc_r.get_n_frames()),
  pi   (pi   ),
  enc_r(enc_r),
  enc_c(enc_c),

  parity_extended(this->N == (enc_r.get_N() +1) * (enc_c.get_N() +1)),

  X_N_i (this->N)
{
	const std::string name = "Encoder_turbo_product_code";
	this->set_name(name);

	if ((parity_extended && this->N != (enc_r.get_N() +1) * (enc_c.get_N() +1)) || (!parity_extended && this->N != enc_r.get_N() * enc_c.get_N()))
	{
		std::stringstream message;
		message << "'N' has to be equal to ('enc_r.get_N()' +1) * ('enc_c.get_N()' +1) if parity code extension, "
				<< "else equal to 'enc_r.get_N()' * 'enc_c.get_N()'   ('N' = "
		        << this->N << ", 'enc_r.get_N()' = " << enc_r.get_N()
		        << ", 'enc_c.get_N()' = " << enc_c.get_N()
		        << " and 'parity_extended' = " << parity_extended << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->K != enc_r.get_K() * enc_c.get_K())
	{
		std::stringstream message;
		message << "'K' has to be equal to 'enc_r.get_K()' * 'enc_c.get_K()' ('K' = "
		        << this->K << ", 'enc_r.get_K()' = " << enc_r.get_K()
		        << ", 'enc_c.get_K()' = " << enc_c.get_K() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N != (int)pi.get_core().get_size())
	{
		std::stringstream message;
		message << "'pi.get_core().get_size()' has to be equal to 'N' ('pi.get_core().get_size()' = "
		        << pi.get_core().get_size() << ", 'N' = " << this->N << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B>
void Encoder_turbo_product_code<B>
::_encode(const B *U_K, B *X_N, const int frame_id)
{
	const int n_cols_K = this->enc_r.get_K();
	const int n_cols_N = this->enc_r.get_N() + (this->parity_extended ? 1 : 0);
	const int n_rows_K = this->enc_c.get_K();
	const int n_rows_N = this->enc_c.get_N() + (this->parity_extended ? 1 : 0);

	// encode each row
	for (int j = 0; j < n_rows_K; j++)
	{
		this->enc_r.encode(U_K + j*n_cols_K, this->X_N_i.data() + j*n_cols_N); // last parity bit ignored if any

		if (this->parity_extended)
			this->X_N_i[(j+1)*n_cols_N -1] = 0; // parity null for a BCH
	}

	pi.interleave(this->X_N_i.data(), X_N, 0);

	// encode each col
	for (int j = 0; j < n_cols_N; j++)
	{
		this->enc_c.encode(X_N + j*n_rows_N, this->X_N_i.data() + j*n_rows_N); // last parity bit ignored if any

		if (this->parity_extended)
			this->X_N_i[(j+1)*n_rows_N -1] = 0; // parity null for a BCH
	}

	pi.deinterleave(this->X_N_i.data(), X_N, 0);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Encoder_turbo_product_code<B_8>;
template class aff3ct::module::Encoder_turbo_product_code<B_16>;
template class aff3ct::module::Encoder_turbo_product_code<B_32>;
template class aff3ct::module::Encoder_turbo_product_code<B_64>;
#else
template class aff3ct::module::Encoder_turbo_product_code<B>;
#endif
// ==================================================================================== explicit template instantiation
