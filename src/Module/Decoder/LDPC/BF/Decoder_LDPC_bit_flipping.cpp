#include <algorithm>
#include <sstream>
#include <string>
#include <limits>
#include <cmath>

#include "Tools/general_utils.h"
#include "Tools/Perf/common/hard_decide.h"
#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/LDPC/BF/Decoder_LDPC_bit_flipping.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

// constexpr int C_to_V_max = 15; // saturation value for the LLRs/extrinsics

template <typename B, typename R>
Decoder_LDPC_bit_flipping<B,R>
::Decoder_LDPC_bit_flipping(const int &K, const int &N, const int& n_ite,
                            const tools::Sparse_matrix &H,
                            const std::vector<unsigned> &info_bits_pos,
                            const R mwbf_factor,
                            const bool enable_syndrome,
                            const int syndrome_depth)
: Decoder_SISO<B,R>(K, N                                            ),
  n_ite            (n_ite                                           ),
  n_V_nodes        (N                                               ), // same as N but more explicit
  n_C_nodes        ((int)H.get_n_cols()                             ),
  n_branches       ((int)H.get_n_connections()                      ),
  mwbf_factor      (mwbf_factor                                     ),
  enable_syndrome  (enable_syndrome                                 ),
  syndrome_depth   (syndrome_depth                                  ),
  H                (H                                               ),
  info_bits_pos    (info_bits_pos                                   ),
  Lp_N             (N,                                            -1), // -1 in order to fail when AZCW
  C_to_V           (this->n_frames, std::vector<R>(this->n_branches)),
  V_to_C           (this->n_frames, std::vector<R>(this->n_branches)),
  Y_min            (this->n_C_nodes                                 ),
  decis            (this->n_V_nodes                                 )
{
	const std::string name = "Decoder_LDPC_bit_flipping";
	this->set_name(name);

	tools::check_LUT(info_bits_pos, "info_bits_pos", (size_t)K);

	/*if (n_ite <= 0)
	{
		std::stringstream message;
		message << "'n_ite' has to be greater than 0 ('n_ite' = " << n_ite << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}*/

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

	this->reset();
}

template <typename B, typename R>
Decoder_LDPC_bit_flipping<B,R>* Decoder_LDPC_bit_flipping<B,R>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
void Decoder_LDPC_bit_flipping<B,R>
::_reset(const size_t frame_id)
{
	std::fill(this->C_to_V[frame_id].begin(), this->C_to_V[frame_id].end(), (R)0);
}

template <typename B, typename R>
int Decoder_LDPC_bit_flipping<B,R>
::_decode_siso(const R *Y_N1, int8_t *CWD, R *Y_N2, const size_t frame_id)
{
	// actual decoding
	auto synd = this->BF_decode(Y_N1,frame_id);

	// prepare for next round by processing extrinsic information
	for (auto i = 0; i < this->N; i++)
		Y_N2[i] = this->Lp_N[i] - Y_N1[i];

	// saturate<R>(Y_N2, (R)-C_to_V_max, (R)C_to_V_max);

	CWD[0] = (int8_t)synd;
	return !synd;
}

template <typename B, typename R>
int Decoder_LDPC_bit_flipping<B,R>
::_decode_siho(const R *Y_N, int8_t *CWD, B *V_K, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	// actual decoding
	auto synd = this->BF_decode(Y_N, frame_id);
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

	CWD[0] = (int8_t)synd;
	return !synd;
}

template <typename B, typename R>
int Decoder_LDPC_bit_flipping<B,R>
::_decode_siho_cw(const R *Y_N, int8_t *CWD, B *V_N, const size_t frame_id)
{
	//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	// actual decoding
	auto synd = this->BF_decode(Y_N, frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	tools::hard_decide(this->Lp_N.data(), V_N, this->N);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,  d_store);

	CWD[0] = (int8_t)synd;
	return !synd;
}

// BF algorithm
template <typename B, typename R>
bool Decoder_LDPC_bit_flipping<B,R>
::BF_decode(const R *Y_N, const size_t frame_id)
{
	//compute y_min,m for n in N(m)
	for (auto imin = 0; imin < this->n_C_nodes; imin++)
	{
		const auto length = this->n_variables_per_parity[imin];
		auto min_val = std::numeric_limits<R>::max();
		for (auto mmin = 0; mmin < length; ++mmin)
		{
			auto comp = (R)std::abs(Y_N[this->H.get_rows_from_col(imin)[mmin]]);
			min_val = (min_val > comp)?comp:min_val;
		}
		Y_min[imin] = min_val;
	}
	// compute init Zn
	for (int i = 0; i < this->n_V_nodes; ++i)
	{
		decis[i] = (Y_N[i]<0)?1:0;
	}

	// actual decoding
	auto syndrome = true;
	for (auto ite = 0; ite < this->n_ite; ite++)
	{
		// specific inner code depending on the selected implementation (WBF for example)
		syndrome = this->BF_process(Y_N, this->V_to_C[frame_id], this->C_to_V[frame_id]);

		// stop criterion
		if (this->enable_syndrome && syndrome)
			break;
	}

	//output
 	for (auto i = 0; i < this->n_V_nodes; i++)
	{
		// filling the output
		this->Lp_N[i] = (decis[i] == 0)?(R)1:(R)-1;
	}

	return syndrome;
}

template <typename B, typename R>
void Decoder_LDPC_bit_flipping<B,R>
::set_n_frames(const size_t n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		Decoder_SISO<B,R>::set_n_frames(n_frames);

		const auto old_C_to_V_size = this->C_to_V.size();
		const auto new_C_to_V_size = (old_C_to_V_size / old_n_frames) * n_frames;
		this->C_to_V.resize(new_C_to_V_size, std::vector<R>(this->n_branches));

		const auto old_V_to_C_size = this->V_to_C.size();
		const auto new_V_to_C_size = (old_V_to_C_size / old_n_frames) * n_frames;
		this->V_to_C.resize(new_V_to_C_size, std::vector<R>(this->n_branches));
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_LDPC_bit_flipping<B_8,Q_8>;
template class aff3ct::module::Decoder_LDPC_bit_flipping<B_16,Q_16>;
template class aff3ct::module::Decoder_LDPC_bit_flipping<B_32,Q_32>;
template class aff3ct::module::Decoder_LDPC_bit_flipping<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_LDPC_bit_flipping<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
