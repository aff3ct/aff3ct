#include <chrono>
#include <limits>
#include <cmath>
#include <stdexcept>

#include "Tools/Perf/hard_decision.h"
#include "Tools/Math/utils.h"

#include "Decoder_LDPC_BP_layered.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_LDPC_BP_layered<B,R>
::Decoder_LDPC_BP_layered(const int K, const int N, const int n_ite,
                          const tools::Sparse_matrix &H,
                          const std::vector<unsigned> &info_bits_pos,
                          const bool enable_syndrome,
                          const int syndrome_depth,
                          const int n_frames,
                          const std::string name)
: Decoder               (K, N,                                            n_frames, 1, name),
  Decoder_LDPC_BP<B,R>  (K, N, n_ite, H, enable_syndrome, syndrome_depth, n_frames, 1, name),
  n_C_nodes             ((int)H.get_n_cols()                                               ),
  init_flag             (true                                                              ),
  info_bits_pos         (info_bits_pos                                                     ),
  var_nodes             (n_frames, std::vector<R>(N                    )                   ),
  branches              (n_frames, std::vector<R>(H.get_n_connections())                   )
{
}

template <typename B, typename R>
Decoder_LDPC_BP_layered<B,R>
::~Decoder_LDPC_BP_layered()
{
}

template <typename B, typename R>
void Decoder_LDPC_BP_layered<B,R>
::reset()
{
	this->init_flag = true;
}

template <typename B, typename R>
void Decoder_LDPC_BP_layered<B,R>
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

template <typename B, typename R>
void Decoder_LDPC_BP_layered<B,R>
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

template <typename B, typename R>
void Decoder_LDPC_BP_layered<B,R>
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

template <typename B, typename R>
void Decoder_LDPC_BP_layered<B,R>
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
template <typename B, typename R>
void Decoder_LDPC_BP_layered<B,R>
::BP_decode(const int frame_id)
{
	for (auto ite = 0; ite < this->n_ite; ite++)
	{
		this->BP_process(this->var_nodes[frame_id], this->branches[frame_id]);

		if (this->check_syndrome_soft(this->var_nodes[frame_id].data()))
			break;
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_LDPC_BP_layered<B_8,Q_8>;
template class aff3ct::module::Decoder_LDPC_BP_layered<B_16,Q_16>;
template class aff3ct::module::Decoder_LDPC_BP_layered<B_32,Q_32>;
template class aff3ct::module::Decoder_LDPC_BP_layered<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_LDPC_BP_layered<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
