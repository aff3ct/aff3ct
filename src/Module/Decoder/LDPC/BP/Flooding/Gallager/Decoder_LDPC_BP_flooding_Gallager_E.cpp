#include <type_traits>
#include <algorithm>
#include <numeric>
#include <string>

#include "Tools/Math/utils.h"
#include "Module/Decoder/LDPC/BP/Flooding/Gallager/Decoder_LDPC_BP_flooding_Gallager_E.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_LDPC_BP_flooding_Gallager_E<B,R>
::Decoder_LDPC_BP_flooding_Gallager_E(const int K, const int N, const int n_ite, const tools::Sparse_matrix &_H,
                                      const std::vector<unsigned> &info_bits_pos, const bool enable_syndrome,
                                      const int syndrome_depth)
: Decoder_LDPC_BP_flooding_Gallager_A<B,R>(K, N, n_ite, _H, info_bits_pos, true, enable_syndrome, syndrome_depth)
{
	const std::string name = "Decoder_LDPC_BP_flooding_Gallager_E";
	this->set_name(name);
}

template <typename B, typename R>
Decoder_LDPC_BP_flooding_Gallager_E<B,R>* Decoder_LDPC_BP_flooding_Gallager_E<B,R>
::clone() const
{
	auto m = new Decoder_LDPC_BP_flooding_Gallager_E(*this);
	m->deep_copy(*this);
	return m;
}

template <typename T> inline constexpr
T signum(T x, std::false_type is_signed) {
	return T(0) < x;
}

template <typename T> inline constexpr
T signum(T x, std::true_type is_signed) {
	return (T(0) < x) - (x < T(0));
}

template <typename T> inline constexpr
T signum(T x) {
	return signum(x, std::is_signed<T>());
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding_Gallager_E<B,R>
::_initialize_var_to_chk(const B *Y_N, const std::vector<int8_t> &chk_to_var, std::vector<int8_t> &var_to_chk,
                         const int ite)
{
	const auto scaling = ite < 2 ? 2 : 1;
	const bool first_ite = ite == 0;

	auto chk_to_var_ptr = chk_to_var.data();
	auto var_to_chk_ptr = var_to_chk.data();

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
			auto sum = std::accumulate(chk_to_var_ptr, chk_to_var_ptr + var_degree, (int)0);
			sum += scaling * cur_state;

			// majority vote on each node
			for (auto c = 0; c < var_degree; c++)
			{
				auto diff = sum - chk_to_var_ptr[c];
				var_to_chk_ptr[c] = signum(diff);
			}
		}

		chk_to_var_ptr += var_degree;
		var_to_chk_ptr += var_degree;
	}
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding_Gallager_E<B,R>
::_decode_single_ite(const std::vector<int8_t> &var_to_chk, std::vector<int8_t> &chk_to_var)
{
	auto transpose_ptr = this->transpose.data();

	// for each check nodes
	const auto n_chk_nodes = (int)this->H.get_n_cols();
	for (auto c = 0; c < n_chk_nodes; c++)
	{
		const auto chk_degree = (int)this->H.get_col_to_rows()[c].size();

		for (auto v = 0; v < chk_degree; v++)
		{
			auto acc = (int8_t)1;
			for (auto vv = 0; vv < chk_degree; vv++)
				acc = (v != vv) ? acc * var_to_chk[transpose_ptr[vv]] : acc;

			chk_to_var[transpose_ptr[v]] = acc;
		}

		transpose_ptr += chk_degree;
	}
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding_Gallager_E<B,R>
::_make_majority_vote(const B *Y_N, std::vector<int8_t> &V_N)
{
	auto chk_to_var_ptr = this->chk_to_var.data();

	// for the K variable nodes (make a majority vote with the entering messages)
	const auto n_var_nodes = (int)this->H.get_n_rows();
	for (auto v = 0; v < n_var_nodes; v++)
	{
		const auto var_degree = (int)this->H.get_row_to_cols()[v].size();
		const auto cur_state = Y_N[v];

		auto sum = std::accumulate(chk_to_var_ptr, chk_to_var_ptr + var_degree, (int)0);
		sum += (int)cur_state;

		// take the hard decision
		V_N[v] = (int8_t)tools::div2(B(1) -(sum == 0 ? cur_state : signum(sum)));

		chk_to_var_ptr += var_degree;
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_E<B_8,Q_8>;
template class aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_E<B_16,Q_16>;
template class aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_E<B_32,Q_32>;
template class aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_E<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_LDPC_BP_flooding_Gallager_E<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
