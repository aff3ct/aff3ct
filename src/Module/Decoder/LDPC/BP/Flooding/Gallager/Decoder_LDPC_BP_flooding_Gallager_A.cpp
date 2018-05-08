#include <chrono>
#include <limits>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Perf/common.h"
#include "Tools/Math/utils.h"

#include "Decoder_LDPC_BP_flooding_Gallager_A.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::Decoder_LDPC_BP_flooding_Gallager_A(const int K, const int N, const int n_ite, const tools::Sparse_matrix &H,
                                      const std::vector<unsigned> &info_bits_pos, const bool enable_syndrome,
                                      const int syndrome_depth, const int n_frames)
: Decoder             (K, N,                                            n_frames, 1),
  Decoder_LDPC_BP<B,R>(K, N, n_ite, H, enable_syndrome, syndrome_depth, n_frames, 1),
  info_bits_pos       (info_bits_pos                                               ),
  HY_N                (N                                                           ),
  V_N                 (N                                                           ),
  chk_to_var          (H.get_n_connections(), 0                                    ),
  var_to_chk          (H.get_n_connections(), 0                                    )
{
	const std::string name = "Decoder_LDPC_BP_flooding_Gallager_A";
	this->set_name(name);

	transpose.resize(H.get_n_connections());
	std::vector<unsigned char> connections(H.get_n_rows(), 0);

	const auto &chk_to_var_id = H.get_col_to_rows();
	const auto &var_to_chk_id = H.get_row_to_cols();

	auto k = 0;
	for (auto i = 0; i < (int)chk_to_var_id.size(); i++)
	{
		for (auto j = 0; j < (int)chk_to_var_id[i].size(); j++)
		{
			auto var_id = chk_to_var_id[i][j];

			auto branch_id = 0;
			for (auto ii = 0; ii < (int)var_id; ii++)
				branch_id += (int)var_to_chk_id[ii].size();
			branch_id += connections[var_id];
			connections[var_id]++;

			if (connections[var_id] > (int)var_to_chk_id[var_id].size())
			{
				std::stringstream message;
				message << "'connections[var_id]' has to be equal or smaller than 'var_to_chk_id[var_id].size()' "
				        << "('var_id' = " << var_id << ", 'connections[var_id]' = " << connections[var_id]
				        << ", 'var_to_chk_id[var_id].size()' = " << var_to_chk_id[var_id].size() << ").";
				throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
			}

			transpose[k] = branch_id;
			k++;
		}
	}
}

template <typename B, typename R>
Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::~Decoder_LDPC_BP_flooding_Gallager_A()
{
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::_decode_hiho(const B *Y_N, B *V_K, const int frame_id)
{
//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	this->_decode(Y_N);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	for (auto i = 0; i < this->K; i++)
		V_K[i] = (B)this->V_N[this->info_bits_pos[i]];
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_hiho].update_timer(dec::tm::decode_hiho::decode, d_decod);
//	(*this)[dec::tsk::decode_hiho].update_timer(dec::tm::decode_hiho::store,  d_store);
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::_decode_hiho_cw(const B *Y_N, B *V_N, const int frame_id)
{
//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	this->_decode(Y_N);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	std::copy(this->V_N.begin(), this->V_N.begin() + this->N, V_N);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_hiho_cw].update_timer(dec::tm::decode_hiho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_hiho_cw].update_timer(dec::tm::decode_hiho_cw::store,  d_store);
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::_decode_siho(const R *Y_N, B *V_K, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now();  // ---------------------------------------------------------- LOAD
	tools::hard_decide(Y_N, HY_N.data(), this->N);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	this->_decode(HY_N.data());
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	for (auto i = 0; i < this->K; i++)
		V_K[i] = (B)this->V_N[this->info_bits_pos[i]];
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::_decode_siho_cw(const R *Y_N, B *V_N, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now();  // ---------------------------------------------------------- LOAD
	tools::hard_decide(Y_N, HY_N.data(), this->N);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	this->_decode(HY_N.data());
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	std::copy(this->V_N.begin(), this->V_N.begin() + this->N, V_N);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,  d_store);
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::_decode(const B *Y_N)
{
	auto ite = 0;
	for (; ite < this->n_ite; ite++)
	{
		this->_initialize_var_to_chk(Y_N, chk_to_var, var_to_chk, ite == 0);
		this->_decode_single_ite(this->var_to_chk, this->chk_to_var);

		if (this->enable_syndrome && ite != this->n_ite -1)
		{
			// for the K variable nodes (make a majority vote with the entering messages)
			this->_make_majority_vote(Y_N, this->V_N);
			if (this->check_syndrome_hard(this->V_N.data()))
				break;
		}
	}
	if (ite == this->n_ite)
		this->_make_majority_vote(Y_N, this->V_N);
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::_initialize_var_to_chk(const B *Y_N, const std::vector<int8_t> &chk_to_var, std::vector<int8_t> &var_to_chk,
                         const bool first_ite)
{
	auto chk_to_var_ptr = chk_to_var.data();
	auto var_to_chk_ptr = var_to_chk.data();

	// for each variable nodes
	const auto n_var_nodes = (int)this->H.get_n_rows();
	for (auto v = 0; v < n_var_nodes; v++)
	{
		const auto var_degree = (int)this->H.get_row_to_cols()[v].size();

		for (auto c = 0; c < var_degree; c++)
		{
			auto cur_state = Y_N[v];
			if (!first_ite)
			{
				auto count = 0;
				for (auto cc = 0; cc < var_degree; cc++)
					if (cc != c && chk_to_var_ptr[cc] != cur_state)
						count++;

				cur_state = count == (var_degree -1) ? !cur_state : cur_state;
			}

			var_to_chk_ptr[c] = (int8_t)cur_state;
		}

		chk_to_var_ptr += var_degree;
		var_to_chk_ptr += var_degree;
	}
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::_decode_single_ite(const std::vector<int8_t> &var_to_chk, std::vector<int8_t> &chk_to_var)
{
	auto transpose_ptr = this->transpose.data();

	// for each check nodes
	const auto n_chk_nodes = (int)this->H.get_n_cols();
	for (auto c = 0; c < n_chk_nodes; c++)
	{
		const auto chk_degree = (int)this->H.get_col_to_rows()[c].size();

		auto acc = 0;
		for (auto v = 0; v < chk_degree; v++)
			acc ^= var_to_chk[transpose_ptr[v]];

		for (auto v = 0; v < chk_degree; v++)
			chk_to_var[transpose_ptr[v]] = acc ^ var_to_chk[transpose_ptr[v]];

		transpose_ptr += chk_degree;
	}
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::_make_majority_vote(const B *Y_N, std::vector<int8_t> &V_N)
{
	auto chk_to_var_ptr = this->chk_to_var.data();

	// for the K variable nodes (make a majority vote with the entering messages)
	const auto n_var_nodes = (int)this->H.get_n_rows();
	for (auto v = 0; v < n_var_nodes; v++)
	{
		const auto var_degree = (int)this->H.get_row_to_cols()[v].size();
		auto count = 0;

		for (auto c = 0; c < var_degree; c++)
			count += chk_to_var_ptr[c] ? 1 : -1;

		if (var_degree % 2 == 0)
			count += Y_N[v] ? 1 : -1;

		// take the hard decision
		V_N[v] = count > 0 ? 1 : 0;

		chk_to_var_ptr += var_degree;
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_A<B_8,Q_8>;
template class aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_A<B_16,Q_16>;
template class aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_A<B_32,Q_32>;
template class aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_A<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_A<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
