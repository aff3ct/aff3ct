#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <string>
#include <limits>
#include <cmath>
#include <mipp.h>

#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/LDPC/BP/Flooding/SPA/Decoder_LDPC_BP_flooding_SPA.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R>
Decoder_LDPC_BP_flooding_SPA<B,R>
::Decoder_LDPC_BP_flooding_SPA(const int K, const int N, const int n_ite,
                               const tools::Sparse_matrix &_H,
                               const std::vector<uint32_t> &info_bits_pos,
                               const bool enable_syndrome,
                               const int syndrome_depth)
: Decoder_LDPC_BP_flooding<B,R,tools::Update_rule_SPA<R>>(K, N, n_ite, _H, info_bits_pos,
                                                          tools::Update_rule_SPA<R>((unsigned int)_H.get_cols_max_degree()),
                                                          enable_syndrome, syndrome_depth),
  values(_H.get_cols_max_degree())
{
	const std::string name = "Decoder_LDPC_BP_flooding_SPA";
	this->set_name(name);
}

template <typename B, typename R>
Decoder_LDPC_BP_flooding_SPA<B,R>* Decoder_LDPC_BP_flooding_SPA<B,R>
::clone() const
{
	auto m = new Decoder_LDPC_BP_flooding_SPA(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding_SPA<B,R>
::_decode_single_ite(const std::vector<R> &msg_var_to_chk, std::vector<R> &msg_chk_to_var)
{
	const auto n_branches = (int)this->H.get_n_connections();
	auto transpose_ptr = this->transpose.data();

	mipp::Reg<R> r_tmp;
	const mipp::Reg<R> r_1 = (R)1.0;
	const auto vec_loop_size = (n_branches / mipp::N<R>()) * mipp::N<R>();
	for (auto b = 0; b < vec_loop_size; b += mipp::N<R>())
	{
		r_tmp = &msg_var_to_chk[b];
		// tanh(1/2 * t) = (exp(t) - 1) / (exp(t) + 1)
		r_tmp = mipp::exp(r_tmp);
		r_tmp = (r_tmp - r_1) / (r_tmp + r_1);
		r_tmp.storeu(&msg_chk_to_var[b]);
	}
	// tail loop to compute the remaining elements
	for (auto b = vec_loop_size; b < n_branches; b++)
		msg_chk_to_var[b] = (R) std::tanh((R)0.5 * msg_var_to_chk[b]);

	// flooding scheduling
	const auto n_chk_nodes = (int)this->H.get_n_cols();
	for (auto c = 0; c < n_chk_nodes; c++)
	{
		const auto chk_degree = (int)this->H.get_col_to_rows()[c].size();

		auto prod = (R)1;
		for (auto v = 0; v < chk_degree; v++)
		{
			this->values[v] = msg_chk_to_var[transpose_ptr[v]];
			prod *= this->values[v];
		}

		for (auto v = 0; v < chk_degree; v++)
		{
			auto val = prod / this->values[v];
			val = (std::abs(val) < (R)1.0) ? val : ((R)1.0 - std::numeric_limits<R>::epsilon()) * (val > 0 ? 1 : -1);
			msg_chk_to_var[transpose_ptr[v]] = (R)val;
		}

		transpose_ptr += chk_degree;
	}

	for (auto b = 0; b < vec_loop_size; b += mipp::N<R>())
	{
		r_tmp = &msg_chk_to_var[b];
		// 2 * atanh(t) = log((1 + t) / (1 - t)
		r_tmp = mipp::log((r_1 + r_tmp) / (r_1 - r_tmp));
		r_tmp.storeu(&msg_chk_to_var[b]);
	}
	// tail loop to compute the remaining elements
	for (auto b = vec_loop_size; b < n_branches; b++)
		msg_chk_to_var[b] = (R)2.0 * (R)std::atanh(msg_chk_to_var[b]);
}
}
}
