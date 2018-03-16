#include <chrono>
#include <limits>
#include <sstream>

#include "Tools/Perf/common.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"

#include "Decoder_LDPC_BP_flooding.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

// constexpr int C_to_V_max = 15; // saturation value for the LLRs/extrinsics

template <typename B, typename R>
Decoder_LDPC_BP_flooding<B,R>
::Decoder_LDPC_BP_flooding(const int K, const int N, const int n_ite,
                           const tools::Sparse_matrix &H,
                           const std::vector<unsigned> &info_bits_pos,
                           const bool enable_syndrome,
                           const int syndrome_depth,
                           const int n_frames)
: Decoder               (K, N,                                            n_frames, 1),
  Decoder_LDPC_BP<B,R>  (K, N, n_ite, H, enable_syndrome, syndrome_depth, n_frames, 1),
  n_V_nodes             (N                                                           ), // same as N but more explicit
  n_C_nodes             ((int)H.get_n_cols()                                         ),
  n_branches            ((int)H.get_n_connections()                                  ),
  init_flag             (true                                                        ),
  info_bits_pos         (info_bits_pos                                               ),
  Lp_N                  (N, -1                                                       ), // -1 in order to fail when AZCW
  C_to_V                (n_frames, std::vector<R>(this->n_branches)                  ),
  V_to_C                (n_frames, std::vector<R>(this->n_branches)                  )
{
	const std::string name = "Decoder_LDPC_BP_flooding";
	this->set_name(name);

	transpose.resize(this->n_branches);
	mipp::vector<unsigned char> connections(H.get_n_rows(), 0);

	const auto &CN_to_VN = H.get_col_to_rows();
	const auto &VN_to_CN = H.get_row_to_cols();

	auto k = 0;
	for (auto i = 0; i < (int)CN_to_VN.size(); i++)
	{
		for (auto j = 0; j < (int)CN_to_VN[i].size(); j++)
		{
			auto id_V = CN_to_VN[i][j];

			auto branch_id = 0;
			for (auto ii = 0; ii < (int)id_V; ii++)
				branch_id += (int)VN_to_CN[ii].size();
			branch_id += connections[id_V];
			connections[id_V]++;

			if (connections[id_V] > (int)VN_to_CN[id_V].size())
			{
				std::stringstream message;
				message << "'connections[id_V]' has to be equal or smaller than 'VN_to_CN[id_V].size()' ('id_V' = "
				        << id_V << ", 'connections[id_V]' = " << connections[id_V] << ", 'VN_to_CN[id_V].size()' = "
				        << VN_to_CN[id_V].size() << ").";
				throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
			}

			transpose[k] = branch_id;
			k++;
		}
	}

	n_variables_per_parity.resize(H.get_n_cols());
	for (auto i = 0; i < (int)H.get_n_cols(); i++)
		n_variables_per_parity[i] = (unsigned char)CN_to_VN[i].size();

	n_parities_per_variable.resize(H.get_n_rows());
	for (auto i = 0; i < (int)H.get_n_rows(); i++)
		n_parities_per_variable[i] = (unsigned char)VN_to_CN[i].size();
}

template <typename B, typename R>
Decoder_LDPC_BP_flooding<B,R>
::~Decoder_LDPC_BP_flooding()
{
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding<B,R>
::reset()
{
	this->init_flag = true;
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding<B,R>
::_decode_siso(const R *Y_N1, R *Y_N2, const int frame_id)
{
	// memory zones initialization
	if (this->init_flag)
	{
		std::fill(this->C_to_V[frame_id].begin(), this->C_to_V[frame_id].end(), (R)0);

		if (frame_id == Decoder_SIHO<B,R>::n_frames -1)
			this->init_flag = false;
	}

	// actual decoding
	this->BP_decode(Y_N1,frame_id);

	// prepare for next round by processing extrinsic information
	for (auto i = 0; i < this->N; i++)
		Y_N2[i] = this->Lp_N[i] - Y_N1[i];

	// saturate<R>(Y_N2, (R)-C_to_V_max, (R)C_to_V_max);
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding<B,R>
::_decode_siho(const R *Y_N, B *V_K, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	// memory zones initialization
	if (this->init_flag)
	{
		std::fill(this->C_to_V[frame_id].begin(), this->C_to_V[frame_id].end(), (R)0);

		if (frame_id == Decoder_SIHO<B,R>::n_frames -1)
			this->init_flag = false;
	}
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	// actual decoding
	this->BP_decode(Y_N, frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	// take the hard decision
	for (auto i = 0; i < this->K; i++)
	{
		const auto k = this->info_bits_pos[i];
		V_K[i] = !(this->Lp_N[k] >= 0);
	}
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding<B,R>
::_decode_siho_cw(const R *Y_N, B *V_N, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	// memory zones initialization
	if (this->init_flag)
	{
		std::fill(this->C_to_V[frame_id].begin(), this->C_to_V[frame_id].end(), (R)0);

		if (frame_id == Decoder_SIHO<B,R>::n_frames -1)
			this->init_flag = false;
	}
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	// actual decoding
	this->BP_decode(Y_N, frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	tools::hard_decide(this->Lp_N.data(), V_N, this->N);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,  d_store);
}

// BP algorithm
template <typename B, typename R>
void Decoder_LDPC_BP_flooding<B,R>
::BP_decode(const R *Y_N, const int frame_id)
{
	// actual decoding
	for (auto ite = 0; ite < this->n_ite; ite++)
	{
		// specific inner code depending on the selected implementation (min-sum or sum-product for example)
		this->BP_process(Y_N, this->V_to_C[frame_id], this->C_to_V[frame_id]);

		// make a saturation
		// saturate<R>(this->C_to_V, (R)-C_to_V_max, (R)C_to_V_max);

		if (this->enable_syndrome && ite != this->n_ite -1)
		{
			R *C_to_V_ptr = this->C_to_V[frame_id].data();
			for (auto i = 0; i < this->n_V_nodes; i++)
			{
				const auto length = this->n_parities_per_variable[i];

				auto sum_C_to_V = (R)0;
				for (auto j = 0; j < length; j++)
					sum_C_to_V += C_to_V_ptr[j];

				// filling the output
				this->Lp_N[i] = Y_N[i] + sum_C_to_V;

				C_to_V_ptr += length;
			}

			if (this->check_syndrome_soft(this->Lp_N.data()))
				break;
		}
	}

	// begining of the iteration upon all the matrix lines
	R *C_to_V_ptr = this->C_to_V[frame_id].data();
	for (auto i = 0; i < this->n_V_nodes; i++)
	{
		const auto length = this->n_parities_per_variable[i];

		auto sum_C_to_V = (R)0;
		for (auto j = 0; j < length; j++)
			sum_C_to_V += C_to_V_ptr[j];

		// filling the output
		this->Lp_N[i] = Y_N[i] + sum_C_to_V;

		C_to_V_ptr += length;
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_LDPC_BP_flooding<B_8,Q_8>;
template class aff3ct::module::Decoder_LDPC_BP_flooding<B_16,Q_16>;
template class aff3ct::module::Decoder_LDPC_BP_flooding<B_32,Q_32>;
template class aff3ct::module::Decoder_LDPC_BP_flooding<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_LDPC_BP_flooding<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
