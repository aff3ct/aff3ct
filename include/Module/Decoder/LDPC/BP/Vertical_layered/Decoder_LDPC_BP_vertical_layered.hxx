#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <algorithm>
#include <string>
#include <cmath>

#include "Tools/Perf/common/hard_decide.h"
#include "Module/Decoder/LDPC/BP/Vertical_layered/Decoder_LDPC_BP_vertical_layered.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, class Update_rule>
Decoder_LDPC_BP_vertical_layered<B,R,Update_rule>
::Decoder_LDPC_BP_vertical_layered(const int K, const int N, const int n_ite,
                                   const tools::Sparse_matrix &_H,
                                   const std::vector<unsigned> &info_bits_pos,
                                   const Update_rule &up_rule,
                                   const bool enable_syndrome,
                                   const int syndrome_depth,
                                   const int n_frames)
: Decoder               (K, N, n_frames, 1                                    ),
  Decoder_SISO_SIHO<B,R>(K, N, n_frames, 1                                    ),
  Decoder_LDPC_BP       (K, N, n_ite, _H, enable_syndrome, syndrome_depth     ),
  info_bits_pos         (info_bits_pos                                        ),
  up_rule               (up_rule                                              ),
  var_nodes             (n_frames, std::vector<R>(N                          )),
  messages              (n_frames, std::vector<R>(this->H.get_n_connections())),
  contributions         (this->H.get_cols_max_degree()                        ),
  messages_offsets      (this->H.get_n_cols()                                 ),
  init_flag             (true                                                 )
{
	const std::string name = "Decoder_LDPC_BP_vertical_layered<" + this->up_rule.get_name() + ">";
	this->set_name(name);

	size_t cur_off_msg = 0;
	const auto n_chk_nodes = (int)this->H.get_n_cols();
	for (auto c = 0; c < n_chk_nodes; c++)
	{
		messages_offsets[c] = (uint32_t)cur_off_msg;
		cur_off_msg += this->H[c].size();
	}
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_vertical_layered<B,R,Update_rule>
::reset()
{
	this->init_flag = true;
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_vertical_layered<B,R,Update_rule>
::_load(const R *Y_N, const int frame_id)
{
	// memory zones initialization
	if (this->init_flag)
	{
		std::fill(this->messages [frame_id].begin(), this->messages [frame_id].end(), (R)0);
		std::fill(this->var_nodes[frame_id].begin(), this->var_nodes[frame_id].end(), (R)0);

		if (frame_id == Decoder_SIHO<B,R>::n_frames -1)
			this->init_flag = false;
	}

	for (auto v = 0; v < (int)var_nodes[frame_id].size(); v++)
		this->var_nodes[frame_id][v] += Y_N[v]; // var_nodes contain previous extrinsic information
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_vertical_layered<B,R,Update_rule>
::_decode_siso(const R *Y_N1, R *Y_N2, const int frame_id)
{
	// memory zones initialization
	this->_load(Y_N1, frame_id);

	// actual decoding
	this->_decode(frame_id);

	// prepare for next round by processing extrinsic information
	for (auto v = 0; v < this->N; v++)
		Y_N2[v] = this->var_nodes[frame_id][v] - Y_N1[v];

	// copy extrinsic information into var_nodes for next TURBO iteration
	std::copy(Y_N2, Y_N2 + this->N, this->var_nodes[frame_id].begin());
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_vertical_layered<B,R,Update_rule>
::_decode_siho(const R *Y_N, B *V_K, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N, frame_id);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	// actual decoding
	this->_decode(frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	// take the hard decision
	for (auto i = 0; i < this->K; i++)
	{
		const auto k = this->info_bits_pos[i];
		V_K[i] = !(this->var_nodes[frame_id][k] >= 0);
	}
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_vertical_layered<B,R,Update_rule>
::_decode_siho_cw(const R *Y_N, B *V_N, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N, frame_id);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	// actual decoding
	this->_decode(frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	tools::hard_decide(this->var_nodes[frame_id].data(), V_N, this->N);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,  d_store);
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_vertical_layered<B,R,Update_rule>
::_decode(const int frame_id)
{
	this->up_rule.begin_decoding(this->n_ite);

	for (auto ite = 0; ite < this->n_ite; ite++)
	{
		this->up_rule.begin_ite(ite);
		this->_decode_single_ite(this->var_nodes[frame_id], this->messages[frame_id]);
		this->up_rule.end_ite();

		if (this->check_syndrome_soft(this->var_nodes[frame_id].data()))
			break;
	}

	this->up_rule.end_decoding();
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_vertical_layered<B,R,Update_rule>
::_decode_single_ite(std::vector<R> &var_nodes, std::vector<R> &messages)
{
	// vertical layered scheduling
	const auto n_var_nodes = (int)this->H.get_n_rows();
	for (auto vv = 0; vv < n_var_nodes; vv++)
	{
		auto msg_acc = (R)0;
		const auto var_degree = (int)this->H.get_cols_from_row(vv).size();
		for (auto c = 0; c < var_degree; c++)
		{
			auto v_out = -1;
			const auto cc = (int)this->H.get_cols_from_row(vv)[c];
			const auto off_msg = (int)this->messages_offsets[cc];
			const auto chk_degree = (int)this->H[cc].size();
			this->up_rule.begin_chk_node_in(cc, chk_degree);
			for (auto v = 0; v < chk_degree; v++)
			{
				const auto var_id = this->H[cc][v];
				v_out = (var_id == (unsigned)vv) ? v : v_out;
				this->contributions[v] = var_nodes[var_id] - messages[off_msg +v];
				this->up_rule.compute_chk_node_in(v, this->contributions[v]);
			}
			this->up_rule.end_chk_node_in();

			msg_acc -= messages[off_msg + v_out];
			this->up_rule.begin_chk_node_out(cc, chk_degree);
			messages[off_msg + v_out] = this->up_rule.compute_chk_node_out(v_out, this->contributions[v_out]);
			this->up_rule.end_chk_node_out();
			msg_acc += messages[off_msg + v_out];
		}
		var_nodes[vv] += msg_acc;
	}
}
}
}
