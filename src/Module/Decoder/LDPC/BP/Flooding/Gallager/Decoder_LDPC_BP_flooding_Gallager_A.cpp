#include <limits>
#include <stdexcept>

#include "Tools/Math/utils.h"

#include "Decoder_LDPC_BP_flooding_Gallager_A.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::Decoder_LDPC_BP_flooding_Gallager_A(const int &K, const int &N, const int& n_ite, const AList_reader &H,
                                      const mipp::vector<B> &info_bits_pos, const bool enable_syndrome,
                                      const int syndrome_depth, const int n_frames, const std::string name)

: Decoder_SISO<B,R>(K, N, n_frames, 1, name),
  n_ite            (n_ite                  ),
  H                (H                      ),
  enable_syndrome  (enable_syndrome        ),
  syndrome_depth   (syndrome_depth         ),
  info_bits_pos    (info_bits_pos          ),
  Y_N              (N                      ),
  C_to_V_messages  (H.get_n_branches(),   0),
  V_to_C_messages  (H.get_n_branches(),   0)
{
	if (n_ite <= 0)
		throw std::invalid_argument("aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_A: \"n_ite\" has to be "
		                            "greater than 0.");
	if (syndrome_depth <= 0)
		throw std::invalid_argument("aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_A: \"syndrome_depth\" has to "
		                            "be greater than 0.");
	if (N != (int)H.get_n_VN())
		throw std::invalid_argument("aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_A: \"N\" is not compatible "
		                            "with the alist file.");
}

template <typename B, typename R>
Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::~Decoder_LDPC_BP_flooding_Gallager_A()
{
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::_load(const mipp::vector<R>& Y_N_chn)
{
	for (auto i = 0; i < this->N; i++)
		Y_N[i] = Y_N_chn[i] < 0;
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::_hard_decode()
{
	auto cur_syndrome_depth = 0;

	for (auto ite = 0; ite < n_ite; ite++)
	{
		auto C_to_V_mess_ptr = C_to_V_messages.data();
		auto V_to_C_mess_ptr = V_to_C_messages.data();

		// V -> C (for each variable nodes)
		for (auto i = 0; i < (int)H.get_n_VN(); i++)
		{
			const auto node_degree = (int)H.get_VN_to_CN()[i].size();

			for (auto j = 0; j < node_degree; j++)
			{
				auto cur_state = Y_N[i];
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
		auto transpose_ptr = H.get_branches_transpose().data();
		for (auto i = 0; i < (int)H.get_n_CN(); i++)
		{
			const auto node_degree = (int)H.get_CN_to_VN()[i].size();

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
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::_store(mipp::vector<B>& V_K) const
{
	auto C_to_V_ptr = C_to_V_messages.data();

	// for the K variable nodes (make a majority vote with the entering messages)
	for (auto i = 0; i < this->K; i++)
	{
		const auto k = this->info_bits_pos[i];
		const auto node_degree = (int)H.get_VN_to_CN()[k].size();
		auto count = 0;

		for (auto j = 0; j < node_degree; j++)
			count += C_to_V_ptr[j] ? 1 : -1;

		if (node_degree % 2 == 0)
			count += Y_N[k] ? 1 : -1;

		// take the hard decision
		V_K[k] = count > 0 ? 1 : 0;

		C_to_V_ptr += node_degree; // jump to the next node
	}
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::soft_decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext)
{
	throw std::runtime_error("aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_A: this decoder does not support the "
	                         "\"soft_decode\" interface.");
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding_Gallager_A<B,R>
::_soft_decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2)
{
	throw std::runtime_error("aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_A: this decoder does not support the "
	                         "\"_soft_decode\" interface.");
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
