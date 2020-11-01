#include <algorithm>
#include <numeric>
#include <string>

#include "Tools/Math/utils.h"
#include "Module/Decoder/LDPC/BP/Flooding/Gallager/Decoder_LDPC_BP_flooding_Gallager_B.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_LDPC_BP_flooding_Gallager_B<B,R>
::Decoder_LDPC_BP_flooding_Gallager_B(const int K, const int N, const int n_ite, const tools::Sparse_matrix &_H,
                                      const std::vector<unsigned> &info_bits_pos, const bool enable_syndrome,
                                      const int syndrome_depth)
: Decoder_LDPC_BP_flooding_Gallager_A<B,R>(K, N, n_ite, _H, info_bits_pos, enable_syndrome, syndrome_depth)
{
	const std::string name = "Decoder_LDPC_BP_flooding_Gallager_B";
	this->set_name(name);
}

template <typename B, typename R>
Decoder_LDPC_BP_flooding_Gallager_B<B,R>* Decoder_LDPC_BP_flooding_Gallager_B<B,R>
::clone() const
{
	auto m = new Decoder_LDPC_BP_flooding_Gallager_B(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding_Gallager_B<B,R>
::_initialize_var_to_chk(const B *Y_N, const std::vector<int8_t> &chk_to_var, std::vector<int8_t> &var_to_chk,
                         const int ite)
{
	auto chk_to_var_ptr = chk_to_var.data();
	auto var_to_chk_ptr = var_to_chk.data();

	const bool first_ite = ite == 0;

	// for each variable nodes
	const auto n_var_nodes = (int)this->H.get_n_rows();
	for (auto v = 0; v < n_var_nodes; v++)
	{
		const auto var_degree = (int)this->H.get_row_to_cols()[v].size();
		const auto cur_state = (int8_t)Y_N[v];

		if (first_ite)
		{
			std::fill(var_to_chk_ptr, var_to_chk_ptr + var_degree, cur_state);
		}
		else
		{
			// algorithm from the Ryan & Lin book, "Channel codes: Classical and modern", sec. 5.7.3
			const auto sum = std::accumulate(chk_to_var_ptr, chk_to_var_ptr + var_degree, (int)0);
			const auto n_ones = sum + cur_state;
			const auto n_zero = var_degree - sum;
			const auto n_z_m_o = n_zero - n_ones;
			for (auto c = 0; c < var_degree; c++)
			{
				const auto diff = n_z_m_o + chk_to_var_ptr[c];
				var_to_chk_ptr[c] = diff == 0 ? cur_state : (int8_t)tools::signbit(diff);
			}

			// // naive version of the variable to check node messages computations
			// const auto n_ones = std::accumulate(chk_to_var_ptr, chk_to_var_ptr + var_degree, 0);
			// const auto n_zeros = var_degree - n_ones;
			// for (auto c = 0; c < var_degree; c++)
			// {
			// 	const auto n_ones_up  = !cur_state &&  chk_to_var_ptr[c] ? n_ones  -1 : n_ones;
			// 	const auto n_zeros_up =  cur_state && !chk_to_var_ptr[c] ? n_zeros -1 : n_zeros;
			// 	auto new_state = 0;
			// 	if (cur_state)
			// 		new_state = n_zeros_up > n_ones_up ? !cur_state : cur_state;
			// 	else
			// 		new_state = n_zeros_up < n_ones_up ? !cur_state : cur_state;
			// 	var_to_chk_ptr[c] = new_state;
			// }
		}

		chk_to_var_ptr += var_degree;
		var_to_chk_ptr += var_degree;
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_B<B_8,Q_8>;
template class aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_B<B_16,Q_16>;
template class aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_B<B_32,Q_32>;
template class aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_B<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_B<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
