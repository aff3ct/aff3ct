#include <string>
#include <sstream>
#include <numeric>

#include "Tools/general_utils.h"
#include "Tools/Perf/common/hard_decide.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"
#include "Module/Decoder/LDPC/BP/Flooding/Gallager/Decoder_LDPC_BP_flooding_Gallager_A.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::Decoder_LDPC_BP_flooding_Gallager_A(const int K, const int N, const int n_ite, const tools::Sparse_matrix &_H,
                                      const std::vector<unsigned> &info_bits_pos, const bool transform_HY_N,
                                      const bool enable_syndrome, const int syndrome_depth)
: Decoder_SIHO<B,R>(K, N                                            ),
  Decoder_LDPC_BP  (K, N, n_ite, _H, enable_syndrome, syndrome_depth),
  transform_HY_N   (transform_HY_N                                  ),
  info_bits_pos    (info_bits_pos                                   ),
  HY_N             (N                                               ),
  V_N              (N                                               ),
  chk_to_var       (this->H.get_n_connections(), 0                  ),
  var_to_chk       (this->H.get_n_connections(), 0                  ),
  transpose        (this->H.get_n_connections()                     )
{
	const std::string name = "Decoder_LDPC_BP_flooding_Gallager_A";
	this->set_name(name);

	tools::check_LUT(info_bits_pos, "info_bits_pos", (size_t)K);

	std::vector<unsigned char> connections(this->H.get_n_rows(), 0);

	const auto &chk_to_var_id = this->H.get_col_to_rows();
	const auto &var_to_chk_id = this->H.get_row_to_cols();

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
::Decoder_LDPC_BP_flooding_Gallager_A(const int K, const int N, const int n_ite, const tools::Sparse_matrix &_H,
                                      const std::vector<unsigned> &info_bits_pos, const bool enable_syndrome,
                                      const int syndrome_depth)
: Decoder_LDPC_BP_flooding_Gallager_A<B,R>(K, N, n_ite, _H, info_bits_pos, false, enable_syndrome, syndrome_depth)
{
}

template <typename B, typename R>
Decoder_LDPC_BP_flooding_Gallager_A<B,R>* Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::clone() const
{
	auto m = new Decoder_LDPC_BP_flooding_Gallager_A(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R>
int Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::_decode_hiho(const B *Y_N, int8_t *CWD, B *V_K, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now();  // ---------------------------------------------------------- LOAD
	if (this->transform_HY_N)
		// useful for the Gallager E
		std::transform(Y_N, Y_N + this->N, HY_N.begin(), [&](const B& in)
		{
			return B(1) - (in + in);
		});
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	int status = 0;
	if (this->transform_HY_N)
		status = this->_decode(HY_N.data());
	else
		status = this->_decode(Y_N);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	for (auto i = 0; i < this->K; i++)
		V_K[i] = (B)this->V_N[this->info_bits_pos[i]];
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_hiho].update_timer(dec::tm::decode_hiho::decode, d_decod);
//	(*this)[dec::tsk::decode_hiho].update_timer(dec::tm::decode_hiho::store,  d_store);

	CWD[0] = !status;
	return status;
}

template <typename B, typename R>
int Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::_decode_hiho_cw(const B *Y_N, int8_t *CWD, B *V_N, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now();  // ---------------------------------------------------------- LOAD
	if (this->transform_HY_N)
		// useful for the Gallager E
		std::transform(Y_N, Y_N + this->N, HY_N.begin(), [&](const B& in)
		{
			return B(1) - (in + in);
		});
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	int status = 0;
	if (this->transform_HY_N)
		status = this->_decode(HY_N.data());
	else
		status = this->_decode(Y_N);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	std::copy(this->V_N.begin(), this->V_N.begin() + this->N, V_N);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho   ].update_timer(dec::tm::decode_siho::load,      d_load);
//	(*this)[dec::tsk::decode_hiho_cw].update_timer(dec::tm::decode_hiho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_hiho_cw].update_timer(dec::tm::decode_hiho_cw::store,  d_store);

	CWD[0] = !status;
	return status;
}

template <typename B, typename R>
int Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::_decode_siho(const R *Y_N, int8_t *CWD, B *V_K, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now();  // ---------------------------------------------------------- LOAD
	tools::hard_decide(Y_N, HY_N.data(), this->N);
	if (this->transform_HY_N)
		// useful for the Gallager E
		std::transform(HY_N.begin(), HY_N.end(), HY_N.begin(), [&](const B& in)
		{
			return B(1) - (in + in);
		});
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	auto status = this->_decode(HY_N.data());
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	for (auto i = 0; i < this->K; i++)
		V_K[i] = (B)this->V_N[this->info_bits_pos[i]];
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);

	CWD[0] = !status;
	return status;
}

template <typename B, typename R>
int Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::_decode_siho_cw(const R *Y_N, int8_t *CWD, B *V_N, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now();  // ---------------------------------------------------------- LOAD
	tools::hard_decide(Y_N, HY_N.data(), this->N);
	if (this->transform_HY_N)
		// useful for the Gallager E
		std::transform(HY_N.begin(), HY_N.end(), HY_N.begin(), [&](const B& in)
		{
			return B(1) - (in + in);
		});
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	auto status = this->_decode(HY_N.data());
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	std::copy(this->V_N.begin(), this->V_N.begin() + this->N, V_N);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,  d_store);

	CWD[0] = !status;
	return status;
}

template <typename B, typename R>
int Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::_decode(const B *Y_N)
{
	auto ite = 0;
	auto synd = true;
	for (; ite < this->n_ite; ite++)
	{
		this->_initialize_var_to_chk(Y_N, chk_to_var, var_to_chk, ite);
		this->_decode_single_ite(this->var_to_chk, this->chk_to_var);

		if (this->enable_syndrome && ite != this->n_ite -1)
		{
			// for the K variable nodes (make a majority vote with the entering messages)
			this->_make_majority_vote(Y_N, this->V_N);
			synd = this->check_syndrome_hard(this->V_N.data());
			if (synd) break;
		}
	}
	if (ite == this->n_ite)
		this->_make_majority_vote(Y_N, this->V_N);

	return !synd;
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::_initialize_var_to_chk(const B *Y_N, const std::vector<int8_t> &chk_to_var, std::vector<int8_t> &var_to_chk,
                         const int ite)
{
	auto chk_to_var_ptr = chk_to_var.data();
	auto var_to_chk_ptr = var_to_chk.data();

	const bool first_ite = ite == 0;

	// for each variable nodes
	const auto n_var_nodes = (int)this->H.get_n_rows();
	for (auto v = 0; v < n_var_nodes; v++)
	{
		const auto var_degree = (int)this->H.get_row_to_cols()[v].size();
		const auto cur_state = (int8_t)Y_N[v];

		if (first_ite)
		{
			std::fill(var_to_chk_ptr, var_to_chk_ptr + var_degree, cur_state);
		}
		else
		{
			// algorithm from the Ryan & Lin book, "Channel codes: Classical and modern", sec. 5.7.3
			const auto n_ones = std::accumulate(chk_to_var_ptr, chk_to_var_ptr + var_degree, 0);
			const auto n_zeros = var_degree - n_ones;
			for (auto c = 0; c < var_degree; c++)
			{
				auto diff = cur_state ? n_zeros : n_ones;
				diff = cur_state != chk_to_var_ptr[c] ? diff -1 : diff;
				const auto new_state = diff == var_degree -1 && var_degree != 1 ? !cur_state : cur_state;
				var_to_chk_ptr[c] = new_state;
			}
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
		const auto cur_state = Y_N[v];
		const auto n_ones = std::accumulate(chk_to_var_ptr, chk_to_var_ptr + var_degree, (int)0);
		const auto n_zero = var_degree - n_ones;
		const auto n_z_m_o = n_zero - n_ones;
		V_N[v] = n_z_m_o == 0 ? (int8_t)cur_state : (int8_t)tools::signbit(n_z_m_o);
		chk_to_var_ptr += var_degree;

		// // naive version of the majority vote
		// const auto var_degree = (int)this->H.get_row_to_cols()[v].size();
		// auto count = 0;
		// for (auto c = 0; c < var_degree; c++)
		// 	count += chk_to_var_ptr[c] ? 1 : -1;
		// if (var_degree % 2 == 0)
		// 	count += Y_N[v] ? 1 : -1;
		// V_N[v] = count > 0 ? 1 : 0;
		// chk_to_var_ptr += var_degree;
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_A<B_8,Q_8>;
template class aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_A<B_16,Q_16>;
template class aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_A<B_32,Q_32>;
template class aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_A<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_A<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
