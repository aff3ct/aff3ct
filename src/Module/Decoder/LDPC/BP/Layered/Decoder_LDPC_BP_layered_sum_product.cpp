#include <limits>
#include <cmath>

#include "Tools/Display/bash_tools.h"
#include "Tools/Math/utils.h"

#include "Decoder_LDPC_BP_layered_sum_product.hpp"

template <typename B, typename R>
Decoder_LDPC_BP_layered_sum_product<B,R>
::Decoder_LDPC_BP_layered_sum_product(const int &K, const int &N, const int& n_ite,
                                      const AList_reader &alist_data,
                                      const std::string name)
: Decoder_SISO<B,R>      (K, N, 1, name                   ),
  n_ite                  (n_ite                           ),
  n_V_nodes              (N                               ), // same as N but more explicit
  n_C_nodes              (N - K                           ),
  n_branches             ((int)alist_data.get_n_branches()),
  init_flag              (false                           ),

  CN_to_VN               (alist_data.get_CN_to_VN()       ),

  var_nodes              (N,                             0)
{
	assert(N == (int)alist_data.get_n_VN());
	assert(K == N - (int)alist_data.get_n_CN());
	assert(n_ite > 0);
}

template <typename B, typename R>
Decoder_LDPC_BP_layered_sum_product<B,R>
::~Decoder_LDPC_BP_layered_sum_product()
{
}

template <typename B, typename R>
void Decoder_LDPC_BP_layered_sum_product<B,R>
::decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext)
{
	std::cerr << bold_red("(EE) This decoder does not support this interface.") << std::endl;
	std::exit(-1);
}

template <typename B, typename R>
void Decoder_LDPC_BP_layered_sum_product<B,R>
::decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2)
{
	assert(Y_N1.size() == Y_N2.size());
	assert(Y_N1.size() == this->var_nodes.size());

	// memory zones initialization
	if (this->init_flag)
	{
		std::fill(this->var_nodes.begin(), this->var_nodes.end(), (R)0);
		this->init_flag = false;
	}

	auto loop_size = (int)this->var_nodes.size();
	for (auto i = 0; i < loop_size; i++)
		this->var_nodes[i] += Y_N1[i];

	// actual decoding
	this->BP_decode();

	// prepare for next round by processing extrinsic information
	for (auto i = 0; i < (int)Y_N2.size(); i++)
		Y_N2[i] = this->var_nodes[i] - Y_N1[i];
}

template <typename B, typename R>
void Decoder_LDPC_BP_layered_sum_product<B,R>
::load(const mipp::vector<R>& Y_N)
{
	assert(Y_N.size() == this->var_nodes.size());

	// memory zones initialization
	if (this->init_flag)
	{
		std::fill(this->var_nodes.begin(), this->var_nodes.end(), (R)0);
		this->init_flag = false;
	}

	auto loop_size = (int)this->var_nodes.size();
	for (auto i = 0; i < loop_size; i++)
		this->var_nodes[i] += Y_N[i];
}

template <typename B, typename R>
void Decoder_LDPC_BP_layered_sum_product<B,R>
::decode()
{
	// actual decoding
	this->BP_decode();

	// set the flag so C_to_V structure can be reset to 0 only at the beginning of the loop in iterative decoding
	this->init_flag = true;
}

template <typename B, typename R>
void Decoder_LDPC_BP_layered_sum_product<B,R>
::store(mipp::vector<B>& V_K) const
{
	assert(V_K.size() <= this->var_nodes.size());

	// take the hard decision
	auto loop_size = (int)V_K.size();
	for (auto i = 0; i < loop_size; i++)
		V_K[i] = this->var_nodes[i] < 0;
}

// BP algorithm
template <typename B, typename R>
void Decoder_LDPC_BP_layered_sum_product<B,R>
::BP_decode()
{
	R values[32]; // lets suppose that 32 >= length is always true...
	for (auto ite = 0; ite < this->n_ite; ite++)
	{
		for (auto i = 0; i < this->n_C_nodes; i++)
		{
			auto sign =    0;
			auto sum  = (R)0;

			for (int j = 0; j < (int)this->CN_to_VN[i].size(); j++)
			{
				const auto value  = this->var_nodes[this->CN_to_VN[i][j]];
				const auto v_abs  = (R)std::abs(value);
				const auto res    = (R)std::log(std::tanh(v_abs * (R)0.5));
				const auto c_sign = std::signbit((float)value) ? -1 : 0;

				sign ^= c_sign;
				sum  += res;
				values[j] = res;
			}

			for (int j = 0; j < (int)this->CN_to_VN[i].size(); j++)
			{
				const auto value   = this->var_nodes[this->CN_to_VN[i][j]];
				const auto v_sig   = sign ^ (std::signbit((float)value) ? -1 : 0);
				const auto v_res   = (R)2.0 * std::atanh(std::exp(sum - values[j]));
				const auto v_to_st = (R)std::copysign(v_res, v_sig);

				this->var_nodes[this->CN_to_VN[i][j]] += v_to_st;
			}
		}
	}
}

template <typename B, typename R>
void Decoder_LDPC_BP_layered_sum_product<B,R>
::set_n_frames(const int n_frames)
{
	std::clog << bold_yellow("(WW) Modifying the number of frames is not allowed in this decoder.") << std::endl;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Decoder_LDPC_BP_layered_sum_product<B_8,Q_8>;
template class Decoder_LDPC_BP_layered_sum_product<B_16,Q_16>;
template class Decoder_LDPC_BP_layered_sum_product<B_32,Q_32>;
template class Decoder_LDPC_BP_layered_sum_product<B_64,Q_64>;
#else
template class Decoder_LDPC_BP_layered_sum_product<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
