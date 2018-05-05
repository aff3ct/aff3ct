#include <chrono>
#include <limits>
#include <cmath>
#include <stdexcept>

#include "Tools/Perf/common.h"
#include "Tools/Math/utils.h"

#include "Decoder_LDPC_BP_layered_new.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, class Update_rule>
Decoder_LDPC_BP_layered_new<B,R,Update_rule>
::Decoder_LDPC_BP_layered_new(const int K, const int N, const int n_ite,
                              const tools::Sparse_matrix &H,
                              const std::vector<unsigned> &info_bits_pos,
                                    Update_rule &rule,
                              const bool enable_syndrome,
                              const int syndrome_depth,
                              const int n_frames)
: Decoder               (K, N,                                            n_frames, 1),
  Decoder_LDPC_BP<B,R>  (K, N, n_ite, H, enable_syndrome, syndrome_depth, n_frames, 1),
  n_C_nodes             ((int)H.get_n_cols()                                         ),
  init_flag             (true                                                        ),
  info_bits_pos         (info_bits_pos                                               ),
  var_nodes             (n_frames, std::vector<R>(N                    )             ),
  branches              (n_frames, std::vector<R>(H.get_n_connections())             ),
  contributions         (H.get_cols_max_degree()                                     ),
  rule                  (rule                                                        )
{
	const std::string name = "Decoder_LDPC_BP_layered_new";
	this->set_name(name);
}

template <typename B, typename R, class Update_rule>
Decoder_LDPC_BP_layered_new<B,R,Update_rule>
::~Decoder_LDPC_BP_layered_new()
{
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_layered_new<B,R,Update_rule>
::reset()
{
	this->init_flag = true;
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_layered_new<B,R,Update_rule>
::_load(const R *Y_N, const int frame_id)
{
	// memory zones initialization
	if (this->init_flag)
	{
		std::fill(this->branches [frame_id].begin(), this->branches [frame_id].end(), (R)0);
		std::fill(this->var_nodes[frame_id].begin(), this->var_nodes[frame_id].end(), (R)0);

		if (frame_id == Decoder_SIHO<B,R>::n_frames -1)
			this->init_flag = false;
	}

	for (auto i = 0; i < (int)var_nodes[frame_id].size(); i++)
		this->var_nodes[frame_id][i] += Y_N[i]; // var_nodes contain previous extrinsic information
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_layered_new<B,R,Update_rule>
::_decode_siso(const R *Y_N1, R *Y_N2, const int frame_id)
{
	// memory zones initialization
	this->_load(Y_N1, frame_id);

	// actual decoding
	this->BP_decode(frame_id);

	// prepare for next round by processing extrinsic information
	for (auto i = 0; i < this->N; i++)
		Y_N2[i] = this->var_nodes[frame_id][i] - Y_N1[i];

	// copy extrinsic information into var_nodes for next TURBO iteration
	std::copy(Y_N2, Y_N2 + this->N, this->var_nodes[frame_id].begin());
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_layered_new<B,R,Update_rule>
::_decode_siho(const R *Y_N, B *V_K, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N, frame_id);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	// actual decoding
	this->BP_decode(frame_id);
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
void Decoder_LDPC_BP_layered_new<B,R,Update_rule>
::_decode_siho_cw(const R *Y_N, B *V_N, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N, frame_id);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	// actual decoding
	this->BP_decode(frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	tools::hard_decide(this->var_nodes[frame_id].data(), V_N, this->N);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,  d_store);
}

// BP algorithm
template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_layered_new<B,R,Update_rule>
::BP_decode(const int frame_id)
{
	rule.begin_decoding(this->n_ite);

	for (auto ite = 0; ite < this->n_ite; ite++)
	{
		rule.begin_ite(ite);
		this->BP_process(this->var_nodes[frame_id], this->branches[frame_id]);
		rule.end_ite();

		if (this->check_syndrome_soft(this->var_nodes[frame_id].data()))
			break;
	}

	rule.end_decoding();
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_layered_new<B,R,Update_rule>
::BP_process(std::vector<R> &var_nodes, std::vector<R> &branches)
{
	auto kr = 0;
	auto kw = 0;
	for (auto i = 0; i < this->n_C_nodes; i++)
	{
		const auto n_VN = (int)this->H[i].size();
		rule.begin_check_node_in(i, n_VN);
		for (auto j = 0; j < n_VN; j++)
		{
			contributions[j] = var_nodes[this->H[i][j]] - branches[kr++];
			rule.compute_check_node_in(j, contributions[j]);
		}
		rule.end_check_node_in();

		rule.begin_check_node_out(i, n_VN);
		for (auto j = 0; j < n_VN; j++)
		{
			branches[kw] = rule.compute_check_node_out(j, contributions[j]);
			var_nodes[this->H[i][j]] = contributions[j] + branches[kw++];
		}
		rule.end_check_node_out();
	}
}
}
}