#include <algorithm>
#include <numeric>
#include <sstream>
#include <string>

#include "Tools/general_utils.h"
#include "Tools/Perf/common/hard_decide.h"
#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/LDPC/BF/Decoder_LDPC_bit_flipping_hard.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

// constexpr int C_to_V_max = 15; // saturation value for the LLRs/extrinsics

template <typename B, typename R>
Decoder_LDPC_bit_flipping_hard<B,R>
::Decoder_LDPC_bit_flipping_hard(const int &K, const int &N, const int& n_ite,
                           const tools::Sparse_matrix &_H,
                           const std::vector<unsigned> &info_bits_pos,
                           const bool enable_syndrome,
                           const int syndrome_depth)
: Decoder_SIHO<B,R>(K, N                ),
  n_ite            (n_ite               ),
  enable_syndrome  (enable_syndrome     ),
  syndrome_depth   (syndrome_depth      ),
  H                (_H                  ),
  var_nodes        (N                   ),
  check_nodes      (this->H.get_n_cols()),
  YH_N             (N                   ),
  info_bits_pos    (info_bits_pos       )
{
	const std::string name = "Decoder_LDPC_bit_flipping_hard";
	this->set_name(name);

	tools::check_LUT<unsigned>(info_bits_pos, "info_bits_pos", (size_t)K);

	if (n_ite <= 0)
	{
		std::stringstream message;
		message << "'n_ite' has to be greater than 0 ('n_ite' = " << n_ite << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (syndrome_depth <= 0)
	{
		std::stringstream message;
		message << "'syndrome_depth' has to be greater than 0 ('syndrome_depth' = " << syndrome_depth << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (N != (int)H.get_n_rows())
	{
		std::stringstream message;
		message << "'N' is not compatible with the H matrix ('N' = " << N << ", 'H.get_n_rows()' = "
		        << H.get_n_rows() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R>
Decoder_LDPC_bit_flipping_hard<B,R>* Decoder_LDPC_bit_flipping_hard<B,R>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
int Decoder_LDPC_bit_flipping_hard<B,R>
::_decode_hiho(const B *Y_N, int8_t *CWD, B *V_K, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now();  // ---------------------------------------------------------- LOAD
	std::copy(Y_N, Y_N + this->N, var_nodes.data());
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	auto synd = this->decode(Y_N, frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	_store(V_K, frame_id);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_hiho].update_timer(dec::tm::decode_hiho::decode, d_decod);
//	(*this)[dec::tsk::decode_hiho].update_timer(dec::tm::decode_hiho::store,  d_store);

	CWD[0] = (int8_t)synd;
	return !synd;
}

template <typename B, typename R>
int Decoder_LDPC_bit_flipping_hard<B,R>
::_decode_hiho_cw(const B *Y_N, int8_t *CWD, B *V_N, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now();  // ---------------------------------------------------------- LOAD
	std::copy(Y_N, Y_N + this->N, var_nodes.data());
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	auto synd = this->decode(Y_N, frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	_store_cw(V_N, frame_id);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_hiho_cw].update_timer(dec::tm::decode_hiho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_hiho_cw].update_timer(dec::tm::decode_hiho_cw::store,  d_store);

	CWD[0] = (int8_t)synd;
	return !synd;
}

template <typename B, typename R>
int Decoder_LDPC_bit_flipping_hard<B,R>
::_decode_siho(const R *Y_N, int8_t *CWD, B *V_K, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now();  // ---------------------------------------------------------- LOAD
	tools::hard_decide(Y_N, YH_N.data(), this->N);
	std::copy(YH_N.begin(), YH_N.end(), var_nodes.begin());
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	auto synd = this->decode(YH_N.data(), frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	_store(V_K, frame_id);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);

	CWD[0] = (int8_t)synd;
	return !synd;
}

template <typename B, typename R>
int Decoder_LDPC_bit_flipping_hard<B,R>
::_decode_siho_cw(const R *Y_N, int8_t *CWD, B *V_N, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now();  // ---------------------------------------------------------- LOAD
	tools::hard_decide(Y_N, YH_N.data(), this->N);
	std::copy(YH_N.begin(), YH_N.end(), var_nodes.begin());
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	auto synd = this->decode(YH_N.data(), frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	_store_cw(V_N, frame_id);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,  d_store);

	CWD[0] = (int8_t)synd;
	return !synd;
}

template<typename B, typename R>
void Decoder_LDPC_bit_flipping_hard<B,R>
::_store(B *V_K, const size_t frame_id)
{
	for (auto i = 0; i < this->K; i++)
		V_K[i] = this->var_nodes[this->info_bits_pos[i]];
}

template<typename B, typename R>
void Decoder_LDPC_bit_flipping_hard<B,R>
::_store_cw(B *V_N, const size_t frame_id)
{
	std::copy(this->var_nodes.begin(), this->var_nodes.end(), V_N);
}

template <typename B, typename R>
bool Decoder_LDPC_bit_flipping_hard<B,R>
::decode(const B *Y_N, const size_t frame_id)
{
	this->cur_syndrome_depth = 0;
	bool synd = false;

	auto* CN = this->check_nodes.data();
	auto* VN = this->var_nodes  .data();

	for (auto ite = 0; ite < this->n_ite; ite++)
	{
		this->cn_process(VN, CN, frame_id);

		if (this->enable_syndrome && (synd = check_syndrome()))
		{
			this->cur_syndrome_depth++;
			if (this->cur_syndrome_depth >= this->syndrome_depth)
				break;
		}
		else
			this->cur_syndrome_depth = 0;

		this->vn_process(Y_N, VN, CN, frame_id);
	}

	if (!this->enable_syndrome || !synd)
		synd = check_syndrome();

	return synd;
}

template <typename B, typename R>
bool Decoder_LDPC_bit_flipping_hard<B,R>
::check_syndrome() const
{
	auto res = std::accumulate(check_nodes.begin(), check_nodes.end(), (B)0,
	                           [](const B a, B b){return a || b;});

	return !res;
}


// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_LDPC_bit_flipping_hard<B_8,Q_8>;
template class aff3ct::module::Decoder_LDPC_bit_flipping_hard<B_16,Q_16>;
template class aff3ct::module::Decoder_LDPC_bit_flipping_hard<B_32,Q_32>;
template class aff3ct::module::Decoder_LDPC_bit_flipping_hard<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_LDPC_bit_flipping_hard<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
