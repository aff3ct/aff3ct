#include <algorithm>
#include <string>

#include "Tools/general_utils.h"
#include "Tools/Perf/common/hard_decide.h"
#include "Tools/Noise/noise_utils.h"
#include "Module/Decoder/LDPC/BP/Peeling/Decoder_LDPC_BP_peeling.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template<typename B, typename R>
Decoder_LDPC_BP_peeling<B,R>::Decoder_LDPC_BP_peeling(const int K, const int N, const int n_ite,
                                                      const tools::Sparse_matrix &_H,
                                                      const std::vector<unsigned> &info_bits_pos,
                                                      const bool enable_syndrome, const int syndrome_depth)
: Decoder_SIHO<B,R>(K, N),
  Decoder_LDPC_BP  (K, N, n_ite, _H, enable_syndrome, syndrome_depth),
  info_bits_pos    (info_bits_pos       ),
  var_nodes        (N                   ),
  check_nodes      (this->H.get_n_cols())
{
	const std::string name = "Decoder_LDPC_BP_peeling";
	this->set_name(name);

	tools::check_LUT(info_bits_pos, "info_bits_pos", (size_t)K);
}

template <typename B, typename R>
Decoder_LDPC_BP_peeling<B,R>* Decoder_LDPC_BP_peeling<B,R>
::clone() const
{
	auto m = new Decoder_LDPC_BP_peeling(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R>
bool Decoder_LDPC_BP_peeling<B,R>
::_decode(const size_t frame_id)
{
	this->cur_syndrome_depth = 0;
	auto links = this->H;

	auto& CN = this->check_nodes;
	auto& VN = this->var_nodes;

	std::fill(CN.begin(), CN.end(), (B)0);

	// first forward known values
	for (unsigned i = 0; i < links.get_n_rows(); i++)
	{
		auto cur_state = VN[i];
		if (!tools::is_unknown_symbol<B>(cur_state))
		{
			auto& cn_list = links.get_cols_from_row(i);
			while (cn_list.size())
			{
				auto& cn_pos = cn_list.front();

				CN[cn_pos] ^= cur_state;
				links.rm_connection(i, cn_pos);
			}
		}
	}

	bool all_check_nodes_done = false;

	for (auto ite = 0; ite < this->n_ite; ite++)
	{
		bool no_modification = true;
		all_check_nodes_done = true;

		// find degree-1 check nodes
		for (unsigned i = 0; i < links.get_n_cols(); i++)
		{
			auto& vn_list = links.get_rows_from_col(i);
			if (vn_list.size() == 1)
			{
				no_modification = false;

				// then forward the belief
				auto& vn_pos = vn_list.front();
				auto cur_state = CN[i];
				VN[vn_pos] = cur_state;
				CN[     i] = 0;
				links.rm_connection(vn_pos, i);

				// and propagate it
				auto& cn_list = links.get_cols_from_row(vn_pos);
				while (cn_list.size())
				{
					auto& cn_pos = cn_list.front();

					CN[cn_pos] ^= cur_state;
					links.rm_connection(vn_pos, cn_pos);
				}
			}
			else
				all_check_nodes_done &= vn_list.size() == 0;
		}

		if (this->enable_syndrome && (all_check_nodes_done || no_modification))
		{
			this->cur_syndrome_depth++;
			if (this->cur_syndrome_depth >= this->syndrome_depth)
				break;
		}
		else
			this->cur_syndrome_depth = 0;
	}

	return all_check_nodes_done;
};

template <typename B, typename R>
int Decoder_LDPC_BP_peeling<B,R>
::_decode_hiho(const B *Y_N, int8_t *CWD, B *V_K, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now();  // ---------------------------------------------------------- LOAD
	std::copy(Y_N, Y_N + this->N, var_nodes.data());
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	auto synd = this->_decode(frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	_store(V_K, frame_id);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_hiho].update_timer(dec::tm::decode_hiho::decode, d_decod);
//	(*this)[dec::tsk::decode_hiho].update_timer(dec::tm::decode_hiho::store,  d_store);

	CWD[0] = synd;
	return !synd;
}

template <typename B, typename R>
int Decoder_LDPC_BP_peeling<B,R>
::_decode_hiho_cw(const B *Y_N, int8_t *CWD, B *V_N, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now();  // ---------------------------------------------------------- LOAD
	std::copy(Y_N, Y_N + this->N, var_nodes.data());
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	auto synd = this->_decode(frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	_store_cw(V_N, frame_id);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_hiho_cw].update_timer(dec::tm::decode_hiho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_hiho_cw].update_timer(dec::tm::decode_hiho_cw::store,  d_store);

	CWD[0] = synd;
	return !synd;
}

template <typename B, typename R>
int Decoder_LDPC_BP_peeling<B,R>
::_decode_siho(const R *Y_N, int8_t *CWD, B *V_K, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now();  // ---------------------------------------------------------- LOAD
	tools::hard_decide_unk(Y_N, var_nodes.data(), this->N);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	auto synd = this->_decode(frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	_store(V_K, frame_id);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);

	CWD[0] = synd;
	return !synd;
}

template <typename B, typename R>
int Decoder_LDPC_BP_peeling<B,R>
::_decode_siho_cw(const R *Y_N, int8_t *CWD, B *V_N, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now();  // ---------------------------------------------------------- LOAD
	tools::hard_decide_unk(Y_N, var_nodes.data(), this->N);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	auto synd = this->_decode(frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	_store_cw(V_N, frame_id);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,  d_store);

	CWD[0] = synd;
	return !synd;
}

template<typename B, typename R>
void Decoder_LDPC_BP_peeling<B,R>::_store(B *V_K, const size_t frame_id)
{
	for (auto i = 0; i < this->K; i++)
		V_K[i] = this->var_nodes[this->info_bits_pos[i]];
}

template<typename B, typename R>
void Decoder_LDPC_BP_peeling<B,R>::_store_cw(B *V_N, const size_t frame_id)
{
	std::copy(this->var_nodes.begin(), this->var_nodes.end(), V_N);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_LDPC_BP_peeling<B_8,Q_8>;
template class aff3ct::module::Decoder_LDPC_BP_peeling<B_16,Q_16>;
template class aff3ct::module::Decoder_LDPC_BP_peeling<B_32,Q_32>;
template class aff3ct::module::Decoder_LDPC_BP_peeling<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_LDPC_BP_peeling<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
