#include <limits>
#include <cmath>
#include <sstream>
#include <iostream>

#include "Tools/general_utils.h"
#include "Tools/Math/utils.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Perf/Reorderer/Reorderer.hpp"

#include "Decoder_LDPC_BP_horizontal_layered_inter.hpp"

namespace aff3ct
{
namespace module
{

template <typename R>
inline mipp::Reg<R> saturate(const mipp::Reg<R> v, const R s)
{
	return v;
}
template <>
inline mipp::Reg<int16_t> saturate(const mipp::Reg<int16_t> v, const int16_t s)
{
	return mipp::sat(v, (int16_t)-s, (int16_t)+s);
}
template <>
inline mipp::Reg<int8_t> saturate(const mipp::Reg<int8_t> v, const int8_t s)
{
	return mipp::sat(v, (int8_t)-s, (int8_t)+s);
}

template <typename B, typename R, class Update_rule>
Decoder_LDPC_BP_horizontal_layered_inter<B,R,Update_rule>
::Decoder_LDPC_BP_horizontal_layered_inter(const int K, const int N, const int n_ite,
                                           const tools::Sparse_matrix &_H,
                                           const std::vector<unsigned> &info_bits_pos,
                                           const Update_rule &up_rule,
                                           const bool enable_syndrome,
                                           const int syndrome_depth,
                                           const int n_frames)
: Decoder               (K, N, n_frames, mipp::N<R>()                                                       ),
  Decoder_SISO_SIHO<B,R>(K, N, n_frames, mipp::N<R>()                                                       ),
  Decoder_LDPC_BP       (K, N, n_ite, _H, enable_syndrome, syndrome_depth                                   ),
  info_bits_pos         (info_bits_pos                                                                      ),
  up_rule               (up_rule                                                                            ),
  sat_val               ((R)((1 << ((sizeof(R) * 8 -2) - (int)std::log2(this->H.get_rows_max_degree()))) -1)),
  var_nodes             (this->n_dec_waves, mipp::vector<mipp::Reg<R>>(N)                                   ),
  messages              (this->n_dec_waves, mipp::vector<mipp::Reg<R>>(this->H.get_n_connections())         ),
  contributions         (this->H.get_cols_max_degree()                                                      ),
  Y_N_reorderered       (N                                                                                  ),
  V_reorderered         (N                                                                                  ),
  init_flag             (true                                                                               )
{
	const std::string name = "Decoder_LDPC_BP_horizontal_layered_inter<" + this->up_rule.get_name() + ">";
	this->set_name(name);

	if (this->sat_val <= 0)
	{
		std::stringstream message;
		message << "'sat_val' has to be greater than 0 ('sat_val' = " << this->sat_val << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_horizontal_layered_inter<B,R,Update_rule>
::reset()
{
	this->init_flag = true;
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_horizontal_layered_inter<B,R,Update_rule>
::_load(const R *Y_N, const int frame_id)
{
	const auto cur_wave = frame_id / this->simd_inter_frame_level;

	// memory zones initialization
	if (this->init_flag)
	{
		const auto zero = mipp::Reg<R>((R)0);
		std::fill(this->messages [cur_wave].begin(), this->messages [cur_wave].end(), zero);
		std::fill(this->var_nodes[cur_wave].begin(), this->var_nodes[cur_wave].end(), zero);

		if (cur_wave == this->n_dec_waves -1) this->init_flag = false;
	}

	std::vector<const R*> frames(mipp::N<R>());
	for (auto f = 0; f < mipp::N<R>(); f++) frames[f] = Y_N + f * this->N;
	tools::Reorderer_static<R,mipp::N<R>()>::apply(frames, (R*)this->Y_N_reorderered.data(), this->N);

	for (auto i = 0; i < (int)var_nodes[cur_wave].size(); i++)
		this->var_nodes[cur_wave][i] += this->Y_N_reorderered[i]; // var_nodes contain previous extrinsic information
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_horizontal_layered_inter<B,R,Update_rule>
::_decode_siso(const R *Y_N1, R *Y_N2, const int frame_id)
{
	// memory zones initialization
	this->_load(Y_N1, frame_id);

	this->_decode(frame_id);

	// prepare for next round by processing extrinsic information
	const auto cur_wave = frame_id / this->simd_inter_frame_level;
	for (auto v = 0; v < this->N; v++)
		this->var_nodes[cur_wave][v] -= Y_N_reorderered[v];

	std::vector<R*> frames(mipp::N<R>());
	for (auto f = 0; f < mipp::N<R>(); f++) frames[f] = Y_N2 + f * this->N;
	tools::Reorderer_static<R,mipp::N<R>()>::apply_rev((R*)this->var_nodes[cur_wave].data(), frames, this->N);
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_horizontal_layered_inter<B,R,Update_rule>
::_decode_siho(const R *Y_N, B *V_K, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N, frame_id);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	this->_decode(frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	// take the hard decision
	const auto cur_wave = frame_id / this->simd_inter_frame_level;
	for (auto v = 0; v < this->K; v++)
	{
		const auto k = this->info_bits_pos[v];
		V_reorderered[v] = mipp::cast<R,B>(this->var_nodes[cur_wave][k]) >> (sizeof(B) * 8 - 1);
	}

	std::vector<B*> frames(mipp::N<R>());
	for (auto f = 0; f < mipp::N<R>(); f++) frames[f] = V_K + f * this->K;
	tools::Reorderer_static<B,mipp::N<R>()>::apply_rev((B*)V_reorderered.data(), frames, this->K);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_horizontal_layered_inter<B,R,Update_rule>
::_decode_siho_cw(const R *Y_N, B *V_N, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N, frame_id);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	this->_decode(frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	// take the hard decision
	const auto cur_wave = frame_id / this->simd_inter_frame_level;
	for (auto v = 0; v < this->N; v++)
		V_reorderered[v] = mipp::cast<R,B>(this->var_nodes[cur_wave][v]) >> (sizeof(B) * 8 - 1);

	std::vector<B*> frames(mipp::N<R>());
	for (auto f = 0; f < mipp::N<R>(); f++) frames[f] = V_N + f * this->N;
	tools::Reorderer_static<B,mipp::N<R>()>::apply_rev((B*)V_reorderered.data(), frames, this->N);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,  d_store);
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_horizontal_layered_inter<B,R,Update_rule>
::_decode(const int frame_id)
{
	const auto cur_wave = frame_id / this->simd_inter_frame_level;

	this->up_rule.begin_decoding(this->n_ite);

	for (auto ite = 0; ite < this->n_ite; ite++)
	{
		this->up_rule.begin_ite(ite);
		this->_decode_single_ite(this->var_nodes[cur_wave], this->messages[cur_wave]);
		this->up_rule.end_ite();

		if (this->_check_syndrome_soft(this->var_nodes[cur_wave]))
			break;
	}

	this->up_rule.end_decoding();
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_horizontal_layered_inter<B,R,Update_rule>
::_decode_single_ite(mipp::vector<mipp::Reg<R>> &var_nodes, mipp::vector<mipp::Reg<R>> &messages)
{
	auto kr = 0;
	auto kw = 0;

	// horizontal layered scheduling
	const auto n_chk_nodes = (int)this->H.get_n_cols();
	for (auto c = 0; c < n_chk_nodes; c++)
	{
		const auto chk_degree = (int)this->H[c].size();
		this->up_rule.begin_chk_node_in(c, chk_degree);
		for (auto v = 0; v < chk_degree; v++)
		{
			this->contributions[v] = var_nodes[this->H[c][v]] - messages[kr++];
			this->up_rule.compute_chk_node_in(v, this->contributions[v]);
		}
		this->up_rule.end_chk_node_in();

		this->up_rule.begin_chk_node_out(c, chk_degree);
		for (auto v = 0; v < chk_degree; v++)
		{
			messages[kw] = saturate<R>(this->up_rule.compute_chk_node_out(v, this->contributions[v]), this->sat_val);
			var_nodes[this->H[c][v]] = this->contributions[v] + messages[kw++];
		}
		this->up_rule.end_chk_node_out();
	}
}

template <typename B, typename R, class Update_rule>
bool Decoder_LDPC_BP_horizontal_layered_inter<B,R,Update_rule>
::_check_syndrome_soft(const mipp::vector<mipp::Reg<R>> &var_nodes)
{
	if (this->enable_syndrome)
	{
		const auto zero = mipp::Msk<mipp::N<B>()>(false);
		auto syndrome = zero;

		auto n_chk_nodes = (int)H.get_n_cols();
		auto c = 0;
		auto syndrome_scalar = true;
		while (c < n_chk_nodes && (syndrome_scalar = mipp::testz(syndrome)))
		{
			auto sign = zero;
			const auto chk_degree = (int)this->H[c].size();
			for (auto v = 0; v < chk_degree; v++)
			{
				const auto value = var_nodes[this->H[c][v]];
				sign ^= mipp::sign(value);
			}

			syndrome |= sign;
			c++;
		}

		this->cur_syndrome_depth = syndrome_scalar ? (this->cur_syndrome_depth +1) % this->syndrome_depth : 0;
		return syndrome_scalar && (this->cur_syndrome_depth == 0);
	}
	else
		return false;
}

}
}