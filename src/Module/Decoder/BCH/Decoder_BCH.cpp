#include <chrono>
#include <sstream>

#include "Tools/Perf/common.h"
#include "Tools/Exception/exception.hpp"

#include "Decoder_BCH.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_BCH<B, R>
::Decoder_BCH(const int K, const int N, const int t, const int n_frames)
: Decoder               (K, N, n_frames, 1),
  Decoder_SIHO_HIHO<B,R>(K, N, n_frames, 1),
  t(t), YH_N(N)
{
	const std::string name = "Decoder_BCH";
	this->set_name(name);

	if (K <= 3)
	{
		std::stringstream message;
		message << "'K' has to be greater than 3 ('K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R>
Decoder_BCH<B, R>
::~Decoder_BCH()
{
}

template <typename B, typename R>
void Decoder_BCH<B, R>
::_decode_hiho(const B *Y_N, B *V_K, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	std::copy(Y_N, Y_N + this->N, YH_N.begin());
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	this->_decode(YH_N.data(), frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	std::copy(YH_N.data() + this->N - this->K, YH_N.data() + this->N, V_K);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_hiho].update_timer(dec::tm::decode_hiho::load,   d_load);
//	(*this)[dec::tsk::decode_hiho].update_timer(dec::tm::decode_hiho::decode, d_decod);
//	(*this)[dec::tsk::decode_hiho].update_timer(dec::tm::decode_hiho::store,  d_store);
}

template <typename B, typename R>
void Decoder_BCH<B, R>
::_decode_hiho_cw(const B *Y_N, B *V_N, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	std::copy(Y_N, Y_N + this->N, YH_N.begin());
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	this->_decode(YH_N.data(), frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	std::copy(YH_N.data(), YH_N.data() + this->N, V_N);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_hiho_cw].update_timer(dec::tm::decode_hiho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_hiho_cw].update_timer(dec::tm::decode_hiho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_hiho_cw].update_timer(dec::tm::decode_hiho_cw::store,  d_store);
}

template <typename B, typename R>
void Decoder_BCH<B, R>
::_decode_siho(const R *Y_N, B *V_K, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	tools::hard_decide(Y_N, YH_N.data(), this->N);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	this->_decode(YH_N.data(), frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	std::copy(YH_N.data() + this->N - this->K, YH_N.data() + this->N, V_K);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);
}

template <typename B, typename R>
void Decoder_BCH<B, R>
::_decode_siho_cw(const R *Y_N, B *V_N, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	tools::hard_decide(Y_N, YH_N.data(), this->N);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	this->_decode(YH_N.data(), frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	std::copy(YH_N.data(), YH_N.data() + this->N, V_N);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,  d_store);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_BCH<B_8,Q_8>;
template class aff3ct::module::Decoder_BCH<B_16,Q_16>;
template class aff3ct::module::Decoder_BCH<B_32,Q_32>;
template class aff3ct::module::Decoder_BCH<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_BCH<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
