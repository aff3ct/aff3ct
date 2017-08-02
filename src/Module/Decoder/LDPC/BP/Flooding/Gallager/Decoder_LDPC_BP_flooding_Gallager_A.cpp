#include <chrono>
#include <limits>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"

#include "Decoder_LDPC_BP_flooding_Gallager_A.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::Decoder_LDPC_BP_flooding_Gallager_A(const int &K, const int &N, const int& n_ite, const Sparse_matrix &H,
                                      const std::vector<unsigned> &info_bits_pos, const bool enable_syndrome,
                                      const int syndrome_depth, const int n_frames, const std::string name)

: Decoder_SISO_SIHO<B,R>(K, N, n_frames, 1, name ),
  n_ite                 (n_ite                   ),
  H                     (H                       ),
  enable_syndrome       (enable_syndrome         ),
  syndrome_depth        (syndrome_depth          ),
  info_bits_pos         (info_bits_pos           ),
  HY_N                  (N                       ),
  V_N                   (N                       ),
  C_to_V_messages       (H.get_n_connections(), 0),
  V_to_C_messages       (H.get_n_connections(), 0)
{
	if (n_ite <= 0)
	{
		std::stringstream message;
		message << "'n_ite' has to be greater than 0 ('n_ite' = " << n_ite << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (syndrome_depth <= 0)
	{
		std::stringstream message;
		message << "'syndrome_depth' has to be greater than 0 ('syndrome_depth' = " << syndrome_depth << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (N != (int)H.get_n_rows())
	{
		std::stringstream message;
		message << "'N' is not compatible with the H matrix ('N' = " << N << ", 'H.get_n_rows()' = "
		        << H.get_n_rows() << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	transpose.resize(H.get_n_connections());
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
				message << "'connections[id_V]' has to be equal or smaller than 'VN_to_CN[id_V].size()' "
				        << "('id_V' = " << id_V << ", 'connections[id_V]' = " << connections[id_V]
				        << ", 'VN_to_CN[id_V].size()' = " << VN_to_CN[id_V].size() << ")'.";
				throw runtime_error(__FILE__, __LINE__, __func__, message.str());
			}

			transpose[k] = branch_id;
			k++;
		}
	}
}

template <typename B, typename R>
Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::~Decoder_LDPC_BP_flooding_Gallager_A()
{
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::_decode_siho(const R *Y_N, B *V_K, const int frame_id)
{
	auto t_load = std::chrono::steady_clock::now();  // ---------------------------------------------------------- LOAD
	for (auto i = 0; i < this->N; i++)
		HY_N[i] = Y_N[i] < 0;
	auto d_load = std::chrono::steady_clock::now() - t_load;

	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	auto cur_syndrome_depth = 0;
	for (auto ite = 0; ite < n_ite; ite++)
	{
		auto C_to_V_mess_ptr = C_to_V_messages.data();
		auto V_to_C_mess_ptr = V_to_C_messages.data();

		// V -> C (for each variable nodes)
		for (auto i = 0; i < (int)H.get_n_rows(); i++)
		{
			const auto node_degree = (int)H.get_row_to_cols()[i].size();

			for (auto j = 0; j < node_degree; j++)
			{
				auto cur_state = HY_N[i];
				if (ite > 0)
				{
					auto count = 0;
					for (auto k = 0; k < node_degree; k++)
						if (k != j && C_to_V_mess_ptr[k] != cur_state)
							count++;

					cur_state = count == (node_degree -1) ? !cur_state : cur_state;
				}

				V_to_C_mess_ptr[j] = cur_state;
			}

			C_to_V_mess_ptr += node_degree; // jump to the next node
			V_to_C_mess_ptr += node_degree; // jump to the next node
		}

		// C -> V (for each check nodes)
		auto syndrome = 0;
		auto transpose_ptr = this->transpose.data();
		for (auto i = 0; i < (int)H.get_n_cols(); i++)
		{
			const auto node_degree = (int)H.get_col_to_rows()[i].size();

			// accumulate the incoming information in CN
			auto acc = 0;
			for (auto j = 0; j < node_degree; j++)
				acc ^= V_to_C_messages[transpose_ptr[j]];

			// regenerate the CN outcoming values
			for (auto j = 0; j < node_degree; j++)
				C_to_V_messages[transpose_ptr[j]] = acc ^ V_to_C_messages[transpose_ptr[j]];

			syndrome = syndrome || acc; // compute the syndrome

			transpose_ptr += node_degree; // jump to the next node
		}

		// stop criterion
		if (this->enable_syndrome && (syndrome == 0))
		{
			cur_syndrome_depth++;
			if (cur_syndrome_depth == this->syndrome_depth)
				break;
		}
		else
			cur_syndrome_depth = 0;
	}
	auto d_decod = std::chrono::steady_clock::now() - t_decod;

	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE

	auto C_to_V_ptr = C_to_V_messages.data();
	// for the K variable nodes (make a majority vote with the entering messages)
	for (auto i = 0; i < this->N; i++)
	{
		const auto node_degree = (int)H.get_row_to_cols()[i].size();
		auto count = 0;

		for (auto j = 0; j < node_degree; j++)
			count += C_to_V_ptr[j] ? 1 : -1;

		if (node_degree % 2 == 0)
			count += HY_N[i] ? 1 : -1;

		// take the hard decision
		this->V_N[i] = count > 0 ? 1 : 0;

		C_to_V_ptr += node_degree; // jump to the next node
	}

	for (auto i = 0; i < this->K; i++)
		V_K[i] = this->V_N[this->info_bits_pos[i]];
	auto d_store = std::chrono::steady_clock::now() - t_store;

	this->d_load_total  += d_load;
	this->d_decod_total += d_decod;
	this->d_store_total += d_store;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_A<B_8,Q_8>;
template class aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_A<B_16,Q_16>;
template class aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_A<B_32,Q_32>;
template class aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_A<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_A<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
