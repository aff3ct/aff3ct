#include <algorithm>
#include <sstream>
#include <string>

#include "Tools/Exception/exception.hpp"
#include "Tools/Perf/common/hard_decide.h"
#include "Module/Decoder/LDPC/BP/Flooding/Decoder_LDPC_BP_flooding.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, class Update_rule>
Decoder_LDPC_BP_flooding<B,R,Update_rule>
::Decoder_LDPC_BP_flooding(const int K, const int N, const int n_ite,
                           const tools::Sparse_matrix &_H,
                           const std::vector<uint32_t> &info_bits_pos,
                           const Update_rule &up_rule,
                           const bool enable_syndrome,
                           const int syndrome_depth,
                           const int n_frames)
: Decoder               (K, N, n_frames, 1                                    ),
  Decoder_SISO_SIHO<B,R>(K, N, n_frames, 1                                    ),
  Decoder_LDPC_BP       (K, N, n_ite, _H, enable_syndrome, syndrome_depth     ),
  info_bits_pos         (info_bits_pos                                        ),
  up_rule               (up_rule                                              ),
  transpose             (this->H.get_n_connections()                          ),
  post                  (N, -1                                                ),
  msg_chk_to_var        (n_frames, std::vector<R>(this->H.get_n_connections())),
  msg_var_to_chk        (n_frames, std::vector<R>(this->H.get_n_connections())),
  init_flag             (true                                                 )
{
	const std::string name = "Decoder_LDPC_BP_flooding<" + this->up_rule.get_name() + ">";
	this->set_name(name);

	mipp::vector<unsigned char> connections(this->H.get_n_rows(), 0);

	const auto &msg_chk_to_var_id = this->H.get_col_to_rows();
	const auto &msg_var_to_chk_id = this->H.get_row_to_cols();

	auto k = 0;
	for (auto i = 0; i < (int)msg_chk_to_var_id.size(); i++)
	{
		for (auto j = 0; j < (int)msg_chk_to_var_id[i].size(); j++)
		{
			auto var_id = msg_chk_to_var_id[i][j];

			auto branch_id = 0;
			for (auto ii = 0; ii < (int)var_id; ii++)
				branch_id += (int)msg_var_to_chk_id[ii].size();
			branch_id += connections[var_id];
			connections[var_id]++;

			if (connections[var_id] > (int)msg_var_to_chk_id[var_id].size())
			{
				std::stringstream message;
				message << "'connections[var_id]' has to be equal or smaller than 'msg_var_to_chk_id[var_id].size()' "
				        << "('var_id' = " << var_id << ", 'connections[var_id]' = " << connections[var_id]
				        << ", 'msg_var_to_chk_id[var_id].size()' = " << msg_var_to_chk_id[var_id].size() << ").";
				throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
			}

			transpose[k] = branch_id;
			k++;
		}
	}
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_flooding<B,R,Update_rule>
::reset()
{
	this->init_flag = true;
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_flooding<B,R,Update_rule>
::_decode_siso(const R *Y_N1, R *Y_N2, const int frame_id)
{
	// memory zones initialization
	if (this->init_flag)
	{
		std::fill(this->msg_chk_to_var[frame_id].begin(), this->msg_chk_to_var[frame_id].end(), (R)0);

		if (frame_id == Decoder_SIHO<B,R>::n_frames -1)
			this->init_flag = false;
	}

	this->_decode(Y_N1, frame_id);

	// prepare for next round by processing extrinsic information
	for (auto v = 0; v < this->N; v++)
		Y_N2[v] = this->post[v] - Y_N1[v];
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_flooding<B,R,Update_rule>
::_decode_siho(const R *Y_N, B *V_K, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	// memory zones initialization
	if (this->init_flag)
	{
		std::fill(this->msg_chk_to_var[frame_id].begin(), this->msg_chk_to_var[frame_id].end(), (R)0);

		if (frame_id == Decoder_SIHO<B,R>::n_frames -1)
			this->init_flag = false;
	}
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	// actual decoding
	this->_decode(Y_N, frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	// take the hard decision
	for (auto v = 0; v < this->K; v++)
	{
		const auto k = this->info_bits_pos[v];
		V_K[v] = !(this->post[k] >= 0);
	}
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_flooding<B,R,Update_rule>
::_decode_siho_cw(const R *Y_N, B *V_N, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	// memory zones initialization
	if (this->init_flag)
	{
		std::fill(this->msg_chk_to_var[frame_id].begin(), this->msg_chk_to_var[frame_id].end(), (R)0);

		if (frame_id == Decoder_SIHO<B,R>::n_frames -1)
			this->init_flag = false;
	}
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	// actual decoding
	this->_decode(Y_N, frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	tools::hard_decide(this->post.data(), V_N, this->N);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,  d_store);
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_flooding<B,R,Update_rule>
::_decode(const R *Y_N, const int frame_id)
{
	this->up_rule.begin_decoding(this->n_ite);

	auto ite = 0;
	for (; ite < this->n_ite; ite++)
	{
		this->up_rule.begin_ite(ite);
		this->_initialize_var_to_chk(Y_N, this->msg_chk_to_var[frame_id], this->msg_var_to_chk[frame_id]);
		this->_decode_single_ite(this->msg_var_to_chk[frame_id], this->msg_chk_to_var[frame_id]);
		this->up_rule.end_ite();

		if (this->enable_syndrome && ite != this->n_ite -1)
		{
			this->_compute_post(Y_N, this->msg_chk_to_var[frame_id], this->post);
			if (this->check_syndrome_soft(this->post.data()))
				break;
		}
	}
	if (ite == this->n_ite)
		this->_compute_post(Y_N, this->msg_chk_to_var[frame_id], this->post);

	this->up_rule.end_decoding();
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_flooding<B,R,Update_rule>
::_initialize_var_to_chk(const R *Y_N, const std::vector<R> &msg_chk_to_var, std::vector<R> &msg_var_to_chk)
{
	auto *msg_chk_to_var_ptr = msg_chk_to_var.data();
	auto *msg_var_to_chk_ptr = msg_var_to_chk.data();

	const auto n_var_nodes = (int)this->H.get_n_rows();;
	for (auto v = 0; v < n_var_nodes; v++)
	{
		const auto var_degree = (int)this->H.get_row_to_cols()[v].size();

		auto sum_msg_chk_to_var = (R)0;
		for (auto c = 0; c < var_degree; c++)
			sum_msg_chk_to_var += msg_chk_to_var_ptr[c];

		const auto tmp = Y_N[v] + sum_msg_chk_to_var;
		for (auto c = 0; c < var_degree; c++)
			msg_var_to_chk_ptr[c] = tmp - msg_chk_to_var_ptr[c];

		msg_chk_to_var_ptr += var_degree;
		msg_var_to_chk_ptr += var_degree;
	}
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_flooding<B,R,Update_rule>
::_decode_single_ite(const std::vector<R> &msg_var_to_chk, std::vector<R> &msg_chk_to_var)
{
	auto transpose_ptr = this->transpose.data();

	// flooding scheduling
	const auto n_chk_nodes = (int)this->H.get_n_cols();
	for (auto c = 0; c < n_chk_nodes; c++)
	{
		const auto chk_degree = (int)this->H.get_col_to_rows()[c].size();

		this->up_rule.begin_chk_node_in(c, chk_degree);
		for (auto v = 0; v < chk_degree; v++)
			this->up_rule.compute_chk_node_in(v, msg_var_to_chk[transpose_ptr[v]]);
		this->up_rule.end_chk_node_in();

		this->up_rule.begin_chk_node_out(c, chk_degree);
		for (auto v = 0; v < chk_degree; v++)
			msg_chk_to_var[transpose_ptr[v]] = this->up_rule.compute_chk_node_out(v, msg_var_to_chk[transpose_ptr[v]]);
		this->up_rule.end_chk_node_out();

		transpose_ptr += chk_degree;
	}
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_flooding<B,R,Update_rule>
::_compute_post(const R *Y_N, const std::vector<R> &msg_chk_to_var, std::vector<R> &post)
{
	// compute the a posteriori info
	const auto *msg_chk_to_var_ptr = msg_chk_to_var.data();
	const auto n_var_nodes = (int)this->H.get_n_rows();;
	for (auto v = 0; v < n_var_nodes; v++)
	{
		const auto var_degree = (int)this->H.get_row_to_cols()[v].size();

		auto sum_msg_chk_to_var = (R)0;
		for (auto c = 0; c < var_degree; c++)
			sum_msg_chk_to_var += msg_chk_to_var_ptr[c];

		// filling the output
		post[v] = Y_N[v] + sum_msg_chk_to_var;

		msg_chk_to_var_ptr += var_degree;
	}
}
}
}