#include <algorithm>
#include <sstream>
#include <string>

#include "Tools/general_utils.h"
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
                           const int syndrome_depth)
: Decoder_SISO<B,R>(K, N                                                       ),
  Decoder_LDPC_BP  (K, N, n_ite, _H, enable_syndrome, syndrome_depth           ),
  info_bits_pos    (info_bits_pos                                              ),
  up_rule          (up_rule                                                    ),
  transpose        (this->H.get_n_connections()                                ),
  post             (N, -1                                                      ),
  msg_chk_to_var   (this->n_frames, std::vector<R>(this->H.get_n_connections())),
  msg_var_to_chk   (this->n_frames, std::vector<R>(this->H.get_n_connections()))
{
	const std::string name = "Decoder_LDPC_BP_flooding<" + this->up_rule.get_name() + ">";
	this->set_name(name);

	tools::check_LUT(info_bits_pos, "info_bits_pos", (size_t)K);

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

	this->reset();
}

template <typename B, typename R, class Update_rule>
Decoder_LDPC_BP_flooding<B,R,Update_rule>* Decoder_LDPC_BP_flooding<B,R,Update_rule>
::clone() const
{
	auto m = new Decoder_LDPC_BP_flooding(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_flooding<B,R,Update_rule>
::_reset(const size_t frame_id)
{
	std::fill(this->msg_chk_to_var[frame_id].begin(), this->msg_chk_to_var[frame_id].end(), (R)0);
}

template <typename B, typename R, class Update_rule>
int Decoder_LDPC_BP_flooding<B,R,Update_rule>
::_decode_siso(const R *Y_N1, int8_t *CWD, R *Y_N2, const size_t frame_id)
{
	auto status = this->_decode(Y_N1, frame_id);

	// prepare for next round by processing extrinsic information
	for (auto v = 0; v < this->N; v++)
		Y_N2[v] = this->post[v] - Y_N1[v];

	CWD[0] = !status;
	return status;
}

template <typename B, typename R, class Update_rule>
int Decoder_LDPC_BP_flooding<B,R,Update_rule>
::_decode_siho(const R *Y_N, int8_t *CWD, B *V_K, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	// actual decoding
	auto status = this->_decode(Y_N, frame_id);
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

	CWD[0] = !status;
	return status;
}

template <typename B, typename R, class Update_rule>
int Decoder_LDPC_BP_flooding<B,R,Update_rule>
::_decode_siho_cw(const R *Y_N, int8_t *CWD, B *V_N, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	// memory zones initialization
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	// actual decoding
	auto status = this->_decode(Y_N, frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	tools::hard_decide(this->post.data(), V_N, this->N);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,  d_store);

	CWD[0] = !status;
	return status;
}

template <typename B, typename R, class Update_rule>
int Decoder_LDPC_BP_flooding<B,R,Update_rule>
::_decode(const R *Y_N, const size_t frame_id)
{
	this->up_rule.begin_decoding(this->n_ite);

	bool valid_synd = true;
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
			valid_synd = this->check_syndrome_soft(this->post.data());
			if (valid_synd) break;
		}
	}
	if (ite == this->n_ite)
		this->_compute_post(Y_N, this->msg_chk_to_var[frame_id], this->post);

	this->up_rule.end_decoding();

	return !valid_synd;
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

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_flooding<B,R,Update_rule>
::set_n_frames(const size_t n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		Decoder_SISO<B,R>::set_n_frames(n_frames);

		const auto vec_size = this->msg_chk_to_var[0].size();
		const auto old_msg_chk_to_var_size = this->msg_chk_to_var.size();
		const auto new_msg_chk_to_var_size = (old_msg_chk_to_var_size / old_n_frames) * n_frames;
		this->msg_chk_to_var.resize(new_msg_chk_to_var_size, std::vector<R>(vec_size));

		const auto vec_size2 = this->msg_var_to_chk[0].size();
		const auto old_msg_var_to_chk_size = this->msg_var_to_chk.size();
		const auto new_msg_var_to_chk_size = (old_msg_var_to_chk_size / old_n_frames) * n_frames;
		this->msg_var_to_chk.resize(new_msg_var_to_chk_size, std::vector<R>(vec_size2));
	}
}

}
}
