#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Perf/Reorderer/Reorderer.hpp"

#include "Decoder_turbo_product_code.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_turbo_product_code<B,R>
::Decoder_turbo_product_code(const int& n_ite,
                             const Interleaver<R> &pi,
                             Decoder_HIHO<B> &hiho_n,
                             Decoder_HIHO<B> &hiho_i)
: Decoder               (hiho_n.get_K() * hiho_i.get_K(), pi.get_core().get_size(), hiho_n.get_n_frames(), hiho_n.get_simd_inter_frame_level()),
  Decoder_SISO_SIHO<B,R>(hiho_n.get_K() * hiho_i.get_K(), pi.get_core().get_size(), hiho_n.get_n_frames(), hiho_n.get_simd_inter_frame_level()),
  n_ite (n_ite ),
  pi    (pi    ),
  hiho_n(hiho_n),
  hiho_i(hiho_i),

  parity_extended(this->N == (hiho_n.get_N() +1) * (hiho_i.get_N() +1)),

  Y_N_i (this->N),
  Y_N_pi(this->N),
  V_K_i (this->K),
  V_N_i (this->N)
{
	const std::string name = "Decoder_turbo_product_code";
	this->set_name(name);

	if ((parity_extended && this->N != (hiho_n.get_N() +1) * (hiho_i.get_N() +1)) || this->N != hiho_n.get_N() * hiho_i.get_N())
	{
		std::stringstream message;
		message << "'N' has to be equal to ('hiho_n.get_N()' +1) * ('hiho_i.get_N()' +1) if parity code extension, "
				<< "else equal to 'hiho_n.get_N()' * 'hiho_i.get_N()'   ('N' = "
		        << this->N << ", 'hiho_n.get_N()' = " << hiho_n.get_N()
		        << ", 'hiho_i.get_N()' = " << hiho_i.get_N() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (n_ite <= 0)
	{
		std::stringstream message;
		message << "'n_ite' has to be greater than 0 ('n_ite' = " << n_ite << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (hiho_n.get_n_frames() != hiho_i.get_n_frames())
	{
		std::stringstream message;
		message << "'hiho_n.get_n_frames()' has to be equal to 'hiho_i.get_n_frames()' ('hiho_n.get_n_frames()' = "
		        << hiho_n.get_n_frames() << ", 'hiho_i.get_n_frames()' = " << hiho_i.get_n_frames() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (hiho_n.get_simd_inter_frame_level() != hiho_i.get_simd_inter_frame_level())
	{
		std::stringstream message;
		message << "'hiho_n.get_simd_inter_frame_level()' has to be equal to 'hiho_i.get_simd_inter_frame_level()' "
		        << "('hiho_n.get_simd_inter_frame_level()' = " << hiho_n.get_simd_inter_frame_level()
		        << ", 'hiho_i.get_simd_inter_frame_level()' = " << hiho_i.get_simd_inter_frame_level() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}



	if (this->K != hiho_n.get_K() * hiho_i.get_K())
	{
		std::stringstream message;
		message << "'K' has to be equal to 'hiho_K.get_K()' * 'hiho_i.get_K()' ('K' = "
		        << this->K << ", 'hiho_K.get_K()' = " << hiho_n.get_K()
		        << ", 'hiho_i.get_K()' = " << hiho_i.get_K() << ").";
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

template <typename B, typename R>
Decoder_turbo_product_code<B,R>
::~Decoder_turbo_product_code()
{
}

template <typename B, typename R>
void Decoder_turbo_product_code<B, R>
::_decode_siso(const R *Y_N1, R *Y_N2, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	std::copy(Y_N1, Y_N1 + this->N, Y_N_i.begin());
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	this->_decode(Y_N1, false);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	std::copy(Y_N_i.data(), Y_N_i.data() + this->N, Y_N2);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);
}

template <typename B, typename R>
void Decoder_turbo_product_code<B, R>
::_decode_siho(const R *Y_N, B *V_K, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	std::copy(Y_N, Y_N + this->N, Y_N_i.begin());
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	this->_decode(Y_N, true);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	std::copy(V_K_i.data(), V_K_i.data() + this->K, V_K);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_hiho].update_timer(dec::tm::decode_hiho::load,   d_load);
//	(*this)[dec::tsk::decode_hiho].update_timer(dec::tm::decode_hiho::decode, d_decod);
//	(*this)[dec::tsk::decode_hiho].update_timer(dec::tm::decode_hiho::store,  d_store);
}

template <typename B, typename R>
void Decoder_turbo_product_code<B, R>
::_decode_siho_cw(const R *Y_N, B *V_N, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	std::copy(Y_N, Y_N + this->N, Y_N_i.begin());
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	this->_decode(Y_N, false);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	std::copy(V_N_i.data(), V_N_i.data() + this->N, V_N);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_hiho_cw].update_timer(dec::tm::decode_hiho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_hiho_cw].update_timer(dec::tm::decode_hiho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_hiho_cw].update_timer(dec::tm::decode_hiho_cw::store,  d_store);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_turbo_product_code<B_8,Q_8>;
template class aff3ct::module::Decoder_turbo_product_code<B_16,Q_16>;
template class aff3ct::module::Decoder_turbo_product_code<B_32,Q_32>;
template class aff3ct::module::Decoder_turbo_product_code<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_turbo_product_code<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
