#include <string>
#include <sstream>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/Turbo_product/Decoder_turbo_product.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_turbo_product<B,R>
::Decoder_turbo_product(const int& n_ite,
                        const std::vector<float>& alpha,
                        const Decoder_chase_pyndiah<B,R> &cp_r,
                        const Decoder_chase_pyndiah<B,R> &cp_c,
                        const Interleaver<R>& pi,
                        const std::vector<float>& beta)
: Decoder_SISO<B,R>(cp_r.get_K() * cp_c.get_K(), pi.get_core().get_size()),
  n_ite(n_ite),
  alpha(alpha),
  beta (beta ),
  pi   (pi  .clone()),
  cp_r (cp_r.clone()),
  cp_c (cp_c.clone()),

  Y_N_i (this->N),
  Y_N_pi(this->N),
  V_K_i (this->K),
  V_N_i (this->N),

  Y_N_cha_i(pi.get_core().get_size())
{
	const std::string name = "Decoder_turbo_product";
	this->set_name(name);

	if (n_ite <= 0)
	{
		std::stringstream message;
		message << "'n_ite' has to be greater than 0 ('n_ite' = " << n_ite << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (n_ite*2 != (int)alpha.size())
	{
		std::stringstream message;
		message << "'alpha.size()' has to be twice 'n_ite' ('n_ite' = " << n_ite << " and 'alpha.size()' = "
		        << alpha.size() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (n_ite*2 != (int)beta.size() && beta.size() != 0)
	{
		std::stringstream message;
		message << "'beta.size()' has to be twice 'n_ite' or null ('n_ite' = " << n_ite << " and 'beta.size()' = "
		        << alpha.size() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (cp_r.get_n_frames() != cp_c.get_n_frames())
	{
		std::stringstream message;
		message << "'cp_r.get_n_frames()' has to be equal to 'cp_c.get_n_frames()' ('cp_r.get_n_frames()' = "
		        << cp_r.get_n_frames() << ", 'cp_c.get_n_frames()' = " << cp_c.get_n_frames() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (cp_r.get_n_frames_per_wave() != cp_c.get_n_frames_per_wave())
	{
		std::stringstream message;
		message << "'cp_r.get_n_frames_per_wave()' has to be equal to 'cp_c.get_n_frames_per_wave()' "
		        << "('cp_r.get_n_frames_per_wave()' = " << cp_r.get_n_frames_per_wave()
		        << ", 'cp_c.get_n_frames_per_wave()' = " << cp_c.get_n_frames_per_wave() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N != cp_r.get_N() * cp_c.get_N())
	{
		std::stringstream message;
		message << "'N' has to be equal to 'cp_r.get_N()' * 'cp_c.get_N()' ('N' = "
		        << this->N << ", 'cp_r.get_N()' = " << cp_r.get_N()
		        << ", 'cp_c.get_N()' = " << cp_c.get_N() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}


	if (this->K != cp_r.get_K() * cp_c.get_K())
	{
		std::stringstream message;
		message << "'K' has to be equal to 'cp_r.get_K()' * 'cp_c.get_K()' ('K' = "
		        << this->K << ", 'cp_r.get_K()' = " << cp_r.get_K()
		        << ", 'cp_c.get_K()' = " << cp_c.get_K() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N != (int)pi.get_core().get_size())
	{
		std::stringstream message;
		message << "'pi.get_core().get_size()' has to be equal to 'N' ('pi.get_core().get_size()' = "
		        << pi.get_core().get_size() << ", 'N' = " << this->N << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (beta.size())
	{
		this->cp_r->clear_beta();
		this->cp_c->clear_beta();
	}

	this->set_n_frames(this->pi->get_n_frames());
}

template <typename B, typename R>
Decoder_turbo_product<B,R>* Decoder_turbo_product<B,R>
::clone() const
{
	auto m = new Decoder_turbo_product(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R>
void Decoder_turbo_product<B,R>
::deep_copy(const Decoder_turbo_product<B,R> &m)
{
	Module::deep_copy(m);
	if (m.cp_r != nullptr) this->cp_r.reset(m.cp_r->clone());
	if (m.cp_c != nullptr) this->cp_c.reset(m.cp_c->clone());
	if (m.pi   != nullptr) this->pi  .reset(m.pi  ->clone());
}

template <typename B, typename R>
int Decoder_turbo_product<B, R>
::_decode_siso(const R *Y_N1, R *Y_N2, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	std::copy(Y_N1, Y_N1 + this->N, Y_N_i.data());
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	auto status = this->_decode(Y_N1, frame_id, 2);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	std::copy(Y_N_i.data(), Y_N_i.data() + this->N, Y_N2);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);

	return status;
}

template <typename B, typename R>
int Decoder_turbo_product<B, R>
::_decode_siho(const R *Y_N, B *V_K, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	std::copy(Y_N, Y_N + this->N, Y_N_i.data());
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	auto status = this->_decode(Y_N, frame_id, 0);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	std::copy(V_K_i.data(), V_K_i.data() + this->K, V_K);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_hiho].update_timer(dec::tm::decode_hiho::load,   d_load);
//	(*this)[dec::tsk::decode_hiho].update_timer(dec::tm::decode_hiho::decode, d_decod);
//	(*this)[dec::tsk::decode_hiho].update_timer(dec::tm::decode_hiho::store,  d_store);

	return status;
}

template <typename B, typename R>
int Decoder_turbo_product<B, R>
::_decode_siho_cw(const R *Y_N, B *V_N, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	std::copy(Y_N, Y_N + this->N, Y_N_i.data());
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	auto status = this->_decode(Y_N, frame_id, 1);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	std::copy(V_N_i.data(), V_N_i.data() + this->N, V_N);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_cp_cw].update_timer(dec::tm::decode_cp_cw::load,   d_load);
//	(*this)[dec::tsk::decode_cp_cw].update_timer(dec::tm::decode_cp_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_cp_cw].update_timer(dec::tm::decode_cp_cw::store,  d_store);

	return status;
}

template <typename B, typename R>
int Decoder_turbo_product<B,R>
::_decode(const R *Y_N_cha, const size_t frame_id, int return_K_siso)
{
	const int n_cols = cp_r->get_N();
	const int n_rows = cp_c->get_N();

	pi->interleave(Y_N_cha, Y_N_cha_i.data(), frame_id, false); // interleave data from the channel

	for (int i = 0; i < n_ite; i++)
	{
		pi->interleave(Y_N_i.data(), Y_N_pi.data(), frame_id, false); // columns becomes rows

		if (beta.size())
		{
			cp_c->set_beta((R)beta[2*i+0]);
			cp_r->set_beta((R)beta[2*i+1]);
		}

		// decode each col
		for (int j = 0; j < n_cols; j++)
		{
			cp_c->decode_siso(Y_N_pi.data(), Y_N_pi.data(), j); // decode j-th column

			auto* cha_ptr = Y_N_cha_i.data() + j*n_rows;
			auto* last_it = Y_N_pi.data() + (j+1)*n_rows;

			for (auto it = Y_N_pi.data() + j*n_rows; it < last_it; it++, cha_ptr++)
			{
				*it *= (R)alpha[2*i];
				*it += *cha_ptr;
			}
		}

		pi->deinterleave(Y_N_pi.data(), Y_N_i.data(), frame_id, false); // rows go back as columns

		// decode each row
		if (i < (n_ite -1) || return_K_siso >= 2)
		{
			for (int j = 0; j < n_rows; j++)
			{
				cp_r->decode_siso(Y_N_i.data(), Y_N_i.data(), j); // decode j-th row

				auto* cha_ptr = Y_N_cha + j*n_cols;
				auto* last_it = Y_N_i.data() + (j+1)*n_cols;

				for (auto it = Y_N_i.data() + j*n_cols; it < last_it; it++, cha_ptr++)
				{
					*it *= (R)alpha[2*i+1];
					*it += *cha_ptr;
				}
			}
		}
		else if(return_K_siso == 0)
		{
			for (int j = 0; j < cp_c->get_K(); j++)
			{
				auto pos = (int)cp_c->get_info_bits_pos()[j];

				cp_r->decode_siho(Y_N_i.data(), V_K_i.data() + (j - pos)*cp_r->get_K(), pos); // decode pos-th row,
				                                                                // offset pos automatically added by decoder
			}
		}
		else if (return_K_siso == 1)
		{
			for (int j = 0; j < n_cols; j++)
				cp_r->decode_siho_cw(Y_N_i.data(), V_N_i.data(), j); // decode j-th row
		}
	}

	return 0;
}

template <typename B, typename R>
void Decoder_turbo_product<B,R>
::set_n_frames(const size_t n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		Decoder_SISO<B,R>::set_n_frames(n_frames);
		this->pi->set_n_frames(n_frames);
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_turbo_product<B_8,Q_8>;
template class aff3ct::module::Decoder_turbo_product<B_16,Q_16>;
template class aff3ct::module::Decoder_turbo_product<B_32,Q_32>;
template class aff3ct::module::Decoder_turbo_product<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_turbo_product<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
