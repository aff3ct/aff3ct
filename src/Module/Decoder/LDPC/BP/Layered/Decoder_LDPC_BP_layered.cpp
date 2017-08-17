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
::Decoder_LDPC_BP_layered(const int &K, const int &N, const int& n_ite,
                          const tools::Sparse_matrix &H,
                          const std::vector<unsigned> &info_bits_pos,
                          const bool enable_syndrome,
                          const int syndrome_depth,
                          const int n_frames,
                          const std::string name)
: Decoder_SISO_SIHO<B,R>(K, N, n_frames, 1, name                        ),
  n_ite                 (n_ite                                          ),
  n_C_nodes             ((int)H.get_n_cols()                            ),
  enable_syndrome       (enable_syndrome                                ),
  syndrome_depth        (syndrome_depth                                 ),
  init_flag             (true                                           ),
  info_bits_pos         (info_bits_pos                                  ),
  H                     (H                                              ),
  var_nodes             (n_frames, std::vector<R>(N                    )),
  branches              (n_frames, std::vector<R>(H.get_n_connections()))
{
	if (n_ite <= 0)
	{
		std::stringstream message;
		message << "'n_ite' has to be greater than 0 ('n_ite' = " << n_ite << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (syndrome_depth <= 0)
	{
		std::stringstream message;
		message << "'syndrome_depth' has to be greater than 0 ('syndrome_depth' = " << syndrome_depth << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (N != (int)H.get_n_rows())
	{
		std::stringstream message;
		message << "'N' is not compatible with the H matrix ('N' = " << N << ", 'H.get_n_rows()' = "
		        << H.get_n_rows() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R>
Decoder_LDPC_BP_layered<B,R>
::~Decoder_LDPC_BP_layered()
{
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
::__decode_siho(const R *Y_N, const int frame_id)
{
	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N, frame_id);
	auto d_load = std::chrono::steady_clock::now() - t_load;

	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	// actual decoding
	this->BP_decode(frame_id);
	auto d_decod = std::chrono::steady_clock::now() - t_decod;

	// set the flag so the branches can be reset to 0 only at the beginning of the loop in iterative decoding
	if (frame_id == Decoder_SIHO<B,R>::n_frames -1)
		this->init_flag = true;

	this->d_load_total  += d_load;
	this->d_decod_total += d_decod;
}

template <typename B, typename R>
void Decoder_LDPC_BP_layered<B,R>
::_decode_siho(const R *Y_N, B *V_K, const int frame_id)
{
	this->__decode_siho(Y_N, frame_id);

	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	// take the hard decision
	for (auto i = 0; i < this->K; i++)
	{
		const auto k = this->info_bits_pos[i];
		V_K[i] = !(this->var_nodes[frame_id][k] >= 0);
	}
	auto d_store = std::chrono::steady_clock::now() - t_store;

	this->d_store_total += d_store;
}

template <typename B, typename R>
void Decoder_LDPC_BP_layered<B,R>
::_decode_siho_coded(const R *Y_N, B *V_N, const int frame_id)
{
	this->__decode_siho(Y_N, frame_id);

	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	tools::hard_decide(this->var_nodes[frame_id].data(), V_N, this->N);
	auto d_store = std::chrono::steady_clock::now() - t_store;

	this->d_store_total += d_store;
}

// BP algorithm
template <typename B, typename R>
void Decoder_LDPC_BP_layered<B,R>
::BP_decode(const int frame_id)
{
	auto cur_syndrome_depth = 0;

	for (auto ite = 0; ite < this->n_ite; ite++)
	{
		this->BP_process(this->var_nodes[frame_id], this->branches[frame_id]);

		// stop criterion
		if (this->enable_syndrome && this->check_syndrome(frame_id))
		{
			cur_syndrome_depth++;
			if (cur_syndrome_depth == this->syndrome_depth)
				break;
		}
		else
			cur_syndrome_depth = 0;
	}
}

template <typename B, typename R>
bool Decoder_LDPC_BP_layered<B,R>
::check_syndrome(const int frame_id)
{
	auto syndrome = false;

	auto k = 0;
	for (auto i = 0; i < this->n_C_nodes; i++)
	{
		auto sign = 0;

		const auto n_VN = (int)this->H[i].size();
		for (auto j = 0; j < n_VN; j++)
		{
			const auto value = this->var_nodes[frame_id][this->H[i][j]] - this->branches[frame_id][k++];
			const auto tmp_sign  = std::signbit((float)value) ? -1 : 0;

			sign ^= tmp_sign;
		}

		syndrome = syndrome || sign;
	}

	return (syndrome == 0);
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
