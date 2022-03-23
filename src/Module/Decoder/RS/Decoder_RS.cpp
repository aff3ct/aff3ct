#include <string>
#include <sstream>

#include "Tools/Perf/common/hard_decide.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"
#include "Tools/Algo/Bit_packer.hpp"
#include "Module/Decoder/RS/Decoder_RS.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_RS<B, R>
::Decoder_RS(const int K, const int N, const tools::RS_polynomial_generator &GF)
: Decoder_SIHO<B,R>(K * GF.get_m(), N * GF.get_m()),
  K_rs        (K                              ),
  N_rs        (N                              ),
  m           (GF.get_m()                     ),
  n_rdncy_bits(GF.get_n_rdncy() * m           ),
  n_rdncy     (GF.get_n_rdncy()               ),
  alpha_to    (GF.get_alpha_to()              ),
  index_of    (GF.get_index_of()              ),
  t           (GF.get_t()                     ),
  N_p2_1      (tools::next_power_of_2(N_rs) -1),
  YH_N        (N_rs                           ),
  YH_Nb       (this->N                        )
{
	const std::string name = "Decoder_RS";
	this->set_name(name);

	if ((this->N_rs - this->K_rs) != this->n_rdncy)
	{
		std::stringstream message;
		message << "'N_rs - K_rs' is different than 'n_rdncy' ('K_rs' = " << this->K_rs << ", 'N_rs' = " << this->N_rs
		        << ", 'n_rdncy' = " << n_rdncy << ", 'N_rs - K_rs' = " << (this->N_rs - this->K_rs) << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R>
Decoder_RS<B,R>* Decoder_RS<B,R>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
int Decoder_RS<B, R>
::_decode_hiho(const B *Y_N, int8_t *CWD, B *V_K, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	tools::Bit_packer::pack(Y_N, YH_N.data(), this->N, 1, false, this->m);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	auto status = this->_decode(YH_N.data(), frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;
	CWD[0] = !status;
//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	tools::Bit_packer::unpack(YH_N.data() + this->n_rdncy, V_K, this->K, 1, false, this->m);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_hiho].update_timer(dec::tm::decode_hiho::load,   d_load);
//	(*this)[dec::tsk::decode_hiho].update_timer(dec::tm::decode_hiho::decode, d_decod);
//	(*this)[dec::tsk::decode_hiho].update_timer(dec::tm::decode_hiho::store,  d_store);

	return status;
}

template <typename B, typename R>
int Decoder_RS<B, R>
::_decode_hiho_cw(const B *Y_N, int8_t *CWD, B *V_N, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	tools::Bit_packer::pack(Y_N, YH_N.data(), this->N, 1, false, this->m);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	auto status = this->_decode(YH_N.data(), frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;
	CWD[0] = !status;
//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	tools::Bit_packer::unpack(YH_N.data(), V_N, this->N, 1, false, this->m);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_hiho_cw].update_timer(dec::tm::decode_hiho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_hiho_cw].update_timer(dec::tm::decode_hiho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_hiho_cw].update_timer(dec::tm::decode_hiho_cw::store,  d_store);

	return status;
}

template <typename B, typename R>
int Decoder_RS<B, R>
::_decode_siho(const R *Y_N, int8_t *CWD, B *V_K, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	tools::hard_decide(Y_N, YH_Nb.data(), this->N);
	tools::Bit_packer::pack(YH_Nb.data(), YH_N.data(), this->N, 1, false, this->m);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	auto status = this->_decode(YH_N.data(), frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;
	CWD[0] = !status;
//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	tools::Bit_packer::unpack(YH_N.data() + this->n_rdncy, V_K, this->K, 1, false, this->m);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);

	return status;
}

template <typename B, typename R>
int Decoder_RS<B, R>
::_decode_siho_cw(const R *Y_N, int8_t *CWD, B *V_N, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	tools::hard_decide(Y_N, YH_Nb.data(), this->N);
	tools::Bit_packer::pack(YH_Nb.data(), YH_N.data(), this->N, 1, false, this->m);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	auto status = this->_decode(YH_N.data(), frame_id);
	CWD[0] = !status;
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	tools::Bit_packer::unpack(YH_N.data(), V_N, this->N, 1, false, this->m);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,  d_store);

	return status;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_RS<B_8,Q_8>;
template class aff3ct::module::Decoder_RS<B_16,Q_16>;
template class aff3ct::module::Decoder_RS<B_32,Q_32>;
template class aff3ct::module::Decoder_RS<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_RS<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
