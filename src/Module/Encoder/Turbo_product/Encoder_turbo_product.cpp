#include <string>
#include <cmath>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Perf/compute_parity.h"
#include "Module/Encoder/Turbo_product/Encoder_turbo_product.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_turbo_product<B>
::Encoder_turbo_product(const Encoder<B> &enc_r, const Encoder<B> &enc_c, const Interleaver<B> &pi)
: Encoder<B>(enc_r.get_K() * enc_c.get_K(), pi.get_core().get_size()),
  pi   (pi   .clone()),
  enc_r(enc_r.clone()),
  enc_c(enc_c.clone()),

  parity_extended(this->N == (enc_r.get_N() +1) * (enc_c.get_N() +1)),

  X_N_i (this->N)
{
	const std::string name = "Encoder_turbo_product";
	this->set_name(name);
	this->set_n_frames(pi.get_n_frames());

	if ((parity_extended && this->N != (enc_r.get_N() +1) * (enc_c.get_N() +1)) ||
	    (!parity_extended && this->N != enc_r.get_N() * enc_c.get_N()))
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

	int par = this->parity_extended ? 1 : 0;

	if ((size_t)(enc_r.get_N() + par) != enc_c.get_n_frames())
	{
		std::stringstream message;
		message << "'enc_r.get_K() + par' has to be equal to 'enc_c.get_n_frames()' ('enc_r.get_K()' = "
		        << enc_r.get_K() << ", 'enc_c.get_n_frames()' = " << enc_c.get_n_frames()
		        << ", 'parity_extended' = " << this->parity_extended << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((size_t)(enc_c.get_N() + par) != enc_r.get_n_frames())
	{
		std::stringstream message;
		message << "'enc_c.get_K() + par' has to be equal to 'enc_r.get_n_frames()' ('enc_c.get_K()' = "
		        << enc_c.get_K() << ", 'enc_r.get_n_frames()' = " << enc_r.get_n_frames()
		        << ", 'parity_extended' = " << this->parity_extended << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B>
Encoder_turbo_product<B>* Encoder_turbo_product<B>
::clone() const
{
	auto m = new Encoder_turbo_product(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
void Encoder_turbo_product<B>
::deep_copy(const Encoder_turbo_product<B> &m)
{
	Module::deep_copy(m);
	if (m.enc_r != nullptr) this->enc_r.reset(m.enc_r->clone());
	if (m.enc_c != nullptr) this->enc_c.reset(m.enc_c->clone());
	if (m.pi    != nullptr) this->pi   .reset(m.pi   ->clone());
}

template <typename B>
void Encoder_turbo_product<B>
::_encode(const B *U_K, B *X_N, const size_t frame_id)
{
	// const int n_cols_K = this->enc_r->get_K();
	const int n_cols_N = this->enc_r->get_N() + (this->parity_extended ? 1 : 0);
	const int n_rows_K = this->enc_c->get_K();
	const int n_rows_N = this->enc_c->get_N() + (this->parity_extended ? 1 : 0);

	// encode each row
	int X_row_par_off = 0; // hack because the encoder enc_r jumps along X_N N_cw bits by N_cw bits
	                       // but when there is a parity bit, X_N_i is 1 bit longer so it is not taken into account
	                       // by the encoder. X_row_par_off shifts X_N_i start pointer parity bit by parity bit.
	for (int j = 0; j < n_rows_K; j++)
	{
		this->enc_r->encode(U_K, this->X_N_i.data() + X_row_par_off, j); // encode the j-th row

		if (this->parity_extended)
		{
			this->X_N_i[(j+1)*n_cols_N -1] = tools::compute_parity(this->X_N_i.data() + j*n_cols_N, n_cols_N -1);
			X_row_par_off ++;
		}
	}

	pi->interleave(this->X_N_i.data(), X_N, frame_id, false);

	// encode each col
	int M = n_rows_N - n_rows_K;
	int U_col_M_off = 0;   // hack because the encoder enc_c jumps along U_K K bits by K bits
	                       // but memory of a column (X_N) is N bits long.
	                       // U_col_M_off shifts X_N start pointer M bits by M bits.
	int X_col_par_off = 0; // hack because the encoder enc_c jumps along X_N N_cw bits by N_cw bits
	                       // but when there is a parity bit, X_N_i is 1 bit longer so it is not taken into account
	                       // by the encoder. X_col_par_off shifts X_N_i start pointer parity bit by parity bit.

	for (int j = 0; j < n_cols_N; j++)
	{
		this->enc_c->encode(X_N + U_col_M_off, this->X_N_i.data() + X_col_par_off, j); // encode the j-th col

		if (this->parity_extended)
		{
			this->X_N_i[(j+1)*n_rows_N -1] = tools::compute_parity(this->X_N_i.data() + j*n_rows_N, n_rows_N -1);
			X_col_par_off++;
		}

		U_col_M_off += M;
	}

	pi->deinterleave(this->X_N_i.data(), X_N, frame_id, false);

	/*
	if (this->enc_r->is_memorizing())
	{
		// TODO : hack to access to a private member not really really nice
		auto& X_N_mem = (this->enc_r->*result<Enc_X_N_mem>::ptr);

		for (int j = n_rows_K; j < n_rows_N; j++)
			std::copy(X_N + j * n_cols_N,
			          X_N + j * n_cols_N + this->enc_r->get_N(), // ignore last parity bit
			          X_N_mem[j].begin());
	}
	*/
}

template <typename B>
void Encoder_turbo_product<B>
::set_n_frames(const size_t n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		Encoder<B>::set_n_frames(n_frames);
		this->pi->set_n_frames(n_frames);
	}
}

/*
template <typename B>
template <typename Tag>
typename Encoder_turbo_product<B>::template result<Tag>::type Encoder_turbo_product<B>::result<Tag>::ptr;

template <typename B>
template <typename Tag, typename Tag::type p>
typename Encoder_turbo_product<B>::template rob<Tag, p>::filler Encoder_turbo_product<B>::rob<Tag, p>::filler_obj;
*/

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
/*template class Encoder_turbo_product<B_8 >::rob<Encoder_turbo_product<B_8 >::Enc_X_N_mem, &Encoder<B_8 >::X_N_mem>;
template class Encoder_turbo_product<B_16>::rob<Encoder_turbo_product<B_16>::Enc_X_N_mem, &Encoder<B_16>::X_N_mem>;
template class Encoder_turbo_product<B_32>::rob<Encoder_turbo_product<B_32>::Enc_X_N_mem, &Encoder<B_32>::X_N_mem>;
template class Encoder_turbo_product<B_64>::rob<Encoder_turbo_product<B_64>::Enc_X_N_mem, &Encoder<B_64>::X_N_mem>;
*/
template class aff3ct::module::Encoder_turbo_product<B_8>;
template class aff3ct::module::Encoder_turbo_product<B_16>;
template class aff3ct::module::Encoder_turbo_product<B_32>;
template class aff3ct::module::Encoder_turbo_product<B_64>;
#else
template class aff3ct::module::Encoder_turbo_product<B>;
#endif
// ==================================================================================== explicit template instantiation
