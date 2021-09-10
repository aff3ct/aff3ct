#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <algorithm>
#include <sstream>
#include <string>
#include <cmath>

#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Perf/Reorderer/Reorderer.hpp"
#include "Module/Decoder/LDPC/BP/Horizontal_layered/Decoder_LDPC_BP_horizontal_layered_inter.hpp"
#include "Module/Decoder/LDPC/BP/Flooding/Decoder_LDPC_BP_flooding_inter.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, class Update_rule>
Decoder_LDPC_BP_flooding_inter<B,R,Update_rule>
::Decoder_LDPC_BP_flooding_inter(const int K, const int N, const int n_ite,
                                 const tools::Sparse_matrix &_H,
                                 const std::vector<unsigned> &info_bits_pos,
                                 const Update_rule &up_rule,
                                 const bool enable_syndrome,
                                 const int syndrome_depth)
: Decoder_SISO<B,R>(K, N                                                                               ),
  Decoder_LDPC_BP  (K, N, n_ite, _H, enable_syndrome, syndrome_depth                                   ),
  info_bits_pos    (info_bits_pos                                                                      ),
  up_rule          (up_rule                                                                            ),
  sat_val          ((R)((1 << ((sizeof(R) * 8 -2) - (int)std::log2(this->H.get_rows_max_degree()))) -1)),
  transpose        (this->H.get_n_connections()                                                        ),
  post             (N, -1                                                                              ),
  msg_chk_to_var   (this->get_n_waves(), mipp::vector<mipp::Reg<R>>(this->H.get_n_connections())       ),
  msg_var_to_chk   (this->get_n_waves(), mipp::vector<mipp::Reg<R>>(this->H.get_n_connections())       ),
  Y_N_reorderered  (N                                                                                  ),
  V_reorderered    (N                                                                                  )
{
	const std::string name = "Decoder_LDPC_BP_flooding_inter<" + this->up_rule.get_name() + ">";
	this->set_name(name);
	this->set_n_frames_per_wave(mipp::N<R>());

	tools::check_LUT(info_bits_pos, "info_bits_pos", (size_t)K);

	if (this->sat_val <= 0)
	{
		std::stringstream message;
		message << "'sat_val' has to be greater than 0 ('sat_val' = " << this->sat_val << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

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
Decoder_LDPC_BP_flooding_inter<B,R,Update_rule>* Decoder_LDPC_BP_flooding_inter<B,R,Update_rule>
::clone() const
{
	auto m = new Decoder_LDPC_BP_flooding_inter(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_flooding_inter<B,R,Update_rule>
::_reset(const size_t frame_id)
{
	const auto cur_wave = frame_id / this->get_n_frames_per_wave();
	const auto zero = mipp::Reg<R>((R)0);
	std::fill(this->msg_chk_to_var[cur_wave].begin(), this->msg_chk_to_var[cur_wave].end(), zero);
}

template <typename B, typename R, class Update_rule>
int Decoder_LDPC_BP_flooding_inter<B,R,Update_rule>
::_decode_siso(const R *Y_N1, int8_t *CWD, R *Y_N2, const size_t frame_id)
{
	const auto cur_wave = frame_id / this->get_n_frames_per_wave();

	std::vector<const R*> frames_in(mipp::N<R>());
	for (auto f = 0; f < mipp::N<R>(); f++) frames_in[f] = Y_N1 + f * this->N;
	tools::Reorderer_static<R,mipp::N<R>()>::apply(frames_in, (R*)this->Y_N_reorderered.data(), this->N);

	auto status = this->_decode(this->Y_N_reorderered.data(), cur_wave);

	// prepare for next round by processing extrinsic information
	for (auto v = 0; v < this->N; v++)
	{
		auto ext = this->post[v] - mipp::Reg<R>(&Y_N1[v * mipp::N<R>()]);
		this->post[v] = ext;
	}

	std::vector<R*> frames_out(mipp::N<R>());
	for (auto f = 0; f < mipp::N<R>(); f++) frames_out[f] = Y_N2 + f * this->N;
	tools::Reorderer_static<R,mipp::N<R>()>::apply_rev((R*)this->post.data(), frames_out, this->N);

	for (auto f = 0; f < mipp::N<R>() ; f++)
		CWD[f] = !((status >> (mipp::N<R>() -1 -f)) & 1);
	return status;
}

template <typename B, typename R, class Update_rule>
int Decoder_LDPC_BP_flooding_inter<B,R,Update_rule>
::_decode_siho(const R *Y_N, int8_t *CWD, B *V_K, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	const auto cur_wave = frame_id / this->get_n_frames_per_wave();

	std::vector<const R*> frames_in(mipp::N<R>());
	for (auto f = 0; f < mipp::N<R>(); f++) frames_in[f] = Y_N + f * this->N;
	tools::Reorderer_static<R,mipp::N<R>()>::apply(frames_in, (R*)this->Y_N_reorderered.data(), this->N);

//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	// actual decoding
	auto status = this->_decode(this->Y_N_reorderered.data(), cur_wave);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	// take the hard decision
	for (auto v = 0; v < this->K; v++)
	{
		const auto k = this->info_bits_pos[v];
		V_reorderered[v] = mipp::cast<R,B>(this->post[k]) >> (sizeof(B) * 8 - 1);
	}

	std::vector<B*> frames_out(mipp::N<R>());
	for (auto f = 0; f < mipp::N<R>(); f++) frames_out[f] = V_K + f * this->K;
	tools::Reorderer_static<B,mipp::N<R>()>::apply_rev((B*)V_reorderered.data(), frames_out, this->K);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);

	for (auto f = 0; f < mipp::N<R>() ; f++)
		CWD[f] = !((status >> (mipp::N<R>() -1 -f)) & 1);
	return status;
}

template <typename B, typename R, class Update_rule>
int Decoder_LDPC_BP_flooding_inter<B,R,Update_rule>
::_decode_siho_cw(const R *Y_N, int8_t *CWD, B *V_N, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	const auto cur_wave = frame_id / this->get_n_frames_per_wave();

	std::vector<const R*> frames_in(mipp::N<R>());
	for (auto f = 0; f < mipp::N<R>(); f++) frames_in[f] = Y_N + f * this->N;
	tools::Reorderer_static<R,mipp::N<R>()>::apply(frames_in, (R*)this->Y_N_reorderered.data(), this->N);

//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	// actual decoding
	auto status = this->_decode(this->Y_N_reorderered.data(), cur_wave);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	for (auto v = 0; v < this->N; v++)
		V_reorderered[v] = mipp::cast<R,B>(this->post[v]) >> (sizeof(B) * 8 - 1);

	std::vector<B*> frames_out(mipp::N<R>());
	for (auto f = 0; f < mipp::N<R>(); f++) frames_out[f] = V_N + f * this->N;
	tools::Reorderer_static<B,mipp::N<R>()>::apply_rev((B*)V_reorderered.data(), frames_out, this->N);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,  d_store);

	for (auto f = 0; f < mipp::N<R>() ; f++)
		CWD[f] = !((status >> (mipp::N<R>() -1 -f)) & 1);
	return status;
}

template <typename B, typename R, class Update_rule>
int Decoder_LDPC_BP_flooding_inter<B,R,Update_rule>
::_decode(const mipp::Reg<R> *Y_N, const size_t cur_wave)
{
	this->up_rule.begin_decoding(this->n_ite);

	int packed_synd = 0;
	auto ite = 0;
	for (; ite < this->n_ite; ite++)
	{
		this->up_rule.begin_ite(ite);
		this->_initialize_var_to_chk(Y_N, this->msg_chk_to_var[cur_wave], this->msg_var_to_chk[cur_wave]);
		this->_decode_single_ite(this->msg_var_to_chk[cur_wave], this->msg_chk_to_var[cur_wave]);
		this->up_rule.end_ite();

		if (this->enable_syndrome && ite != this->n_ite -1)
		{
			this->_compute_post(Y_N, this->msg_chk_to_var[cur_wave], this->post);
			packed_synd = this->_check_syndrome_soft_status(this->post);
			if (packed_synd == status_t::SUCCESS) break;
		}
	}
	if (ite == this->n_ite)
		this->_compute_post(Y_N, this->msg_chk_to_var[cur_wave], this->post);

	this->up_rule.end_decoding();

	return packed_synd;
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_flooding_inter<B,R,Update_rule>
::_initialize_var_to_chk(const              mipp::Reg<R>  *Y_N,
                         const mipp::vector<mipp::Reg<R>> &msg_chk_to_var,
                               mipp::vector<mipp::Reg<R>> &msg_var_to_chk)
{
	auto *msg_chk_to_var_ptr = msg_chk_to_var.data();
	auto *msg_var_to_chk_ptr = msg_var_to_chk.data();

	const auto n_var_nodes = (int)this->H.get_n_rows();;
	for (auto v = 0; v < n_var_nodes; v++)
	{
		const auto var_degree = (int)this->H.get_row_to_cols()[v].size();

		auto sum_msg_chk_to_var = mipp::Reg<R>((R)0);
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
void Decoder_LDPC_BP_flooding_inter<B,R,Update_rule>
::_decode_single_ite(const mipp::vector<mipp::Reg<R>> &msg_var_to_chk, mipp::vector<mipp::Reg<R>> &msg_chk_to_var)
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
		{
			auto val = saturate<R>(this->up_rule.compute_chk_node_out(v, msg_var_to_chk[transpose_ptr[v]]), this->sat_val);
			msg_chk_to_var[transpose_ptr[v]] = val;
		}
		this->up_rule.end_chk_node_out();

		transpose_ptr += chk_degree;
	}
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_flooding_inter<B,R,Update_rule>
::_compute_post(const              mipp::Reg<R>  *Y_N,
                const mipp::vector<mipp::Reg<R>> &msg_chk_to_var,
                      mipp::vector<mipp::Reg<R>> &post)
{
	// compute the a posteriori info
	const auto *msg_chk_to_var_ptr = msg_chk_to_var.data();
	const auto n_var_nodes = (int)this->H.get_n_rows();;
	for (auto v = 0; v < n_var_nodes; v++)
	{
		const auto var_degree = (int)this->H.get_row_to_cols()[v].size();

		auto sum_msg_chk_to_var = mipp::Reg<R>((R)0);
		for (auto c = 0; c < var_degree; c++)
			sum_msg_chk_to_var += msg_chk_to_var_ptr[c];

		// filling the output
		post[v] = Y_N[v] + sum_msg_chk_to_var;

		msg_chk_to_var_ptr += var_degree;
	}
}

template <typename B, typename R, class Update_rule>
bool Decoder_LDPC_BP_flooding_inter<B,R,Update_rule>
::_check_syndrome_soft(const mipp::vector<mipp::Reg<R>> &var_nodes)
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

template <typename B, typename R, class Update_rule>
int Decoder_LDPC_BP_flooding_inter<B,R,Update_rule>
::_check_syndrome_soft_status(const mipp::vector<mipp::Reg<R>> &var_nodes)
{
	const auto zero = mipp::Msk<mipp::N<B>()>(false);
	auto syndrome = zero;

	auto n_chk_nodes = (int)H.get_n_cols();
	auto c = 0;
	auto syndrome_scalar = true;
	while (c < n_chk_nodes)
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

	syndrome_scalar = mipp::testz(syndrome);

	this->cur_syndrome_depth = syndrome_scalar ? (this->cur_syndrome_depth +1) % this->syndrome_depth : 0;
	if (this->cur_syndrome_depth == 0)
	{
		if (!syndrome_scalar)
		{
			int packed_synd = 0;
			for (auto n = 0; n < mipp::N<B>(); n++)
			{
				packed_synd <<= 1;
				packed_synd |= syndrome[n];
			}
			return packed_synd;
		}
		else
			return status_t::SUCCESS;
	}
	return status_t::FAILURE;
}

template <typename B, typename R, class Update_rule>
void Decoder_LDPC_BP_flooding_inter<B,R,Update_rule>
::set_n_frames(const size_t n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		Decoder_SISO<B,R>::set_n_frames(n_frames);

		const auto vec_size = this->msg_chk_to_var[0].size();
		this->msg_chk_to_var.resize(this->get_n_waves(), mipp::vector<mipp::Reg<R>>(vec_size));

		const auto vec_size2 = this->msg_var_to_chk[0].size();
		this->msg_var_to_chk.resize(this->get_n_waves(), mipp::vector<mipp::Reg<R>>(vec_size2));
	}
}

}
}
