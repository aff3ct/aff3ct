#include <limits>
#include <cmath>

#include "Tools/Display/bash_tools.h"
#include "Tools/Math/utils.h"

#include "Decoder_LDPC_BP_layered.hpp"

template <typename B, typename R>
Decoder_LDPC_BP_layered<B,R>
::Decoder_LDPC_BP_layered(const int &K, const int &N, const int& n_ite,
                          const AList_reader &alist_data,
                          const int n_frames,
                          const std::string name)
: Decoder_SISO<B,R>(K, N, n_frames, 1, name       ),
  n_ite            (n_ite                         ),
  n_C_nodes        (N - K                         ),
  init_flag        (false                         ),
  CN_to_VN         (alist_data.get_CN_to_VN()     ),
  var_nodes        (N,                           0),
  branches         (alist_data.get_n_branches(), 0)
{
	assert(N == (int)alist_data.get_n_VN());
//	assert(K == N - (int)alist_data.get_n_CN());
	assert(n_ite > 0);
}

template <typename B, typename R>
Decoder_LDPC_BP_layered<B,R>
::~Decoder_LDPC_BP_layered()
{
}

template <typename B, typename R>
void Decoder_LDPC_BP_layered<B,R>
::soft_decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext)
{
	std::cerr << bold_red("(EE) This decoder does not support this interface.") << std::endl;
	std::exit(-1);
}

template <typename B, typename R>
void Decoder_LDPC_BP_layered<B,R>
::_soft_decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2)
{
	assert(Y_N1.size() == Y_N2.size());
	assert(Y_N1.size() == this->var_nodes.size());

	// memory zones initialization
	if (this->init_flag)
	{
		std::fill(this->branches.begin(), this->branches.end(), (R)0);
		this->init_flag = false;
	}

	std::copy(Y_N1.begin(), Y_N1.end(), this->var_nodes.begin());

	// actual decoding
	this->BP_decode();

	// prepare for next round by processing extrinsic information
	for (auto i = 0; i < (int)Y_N2.size(); i++)
		Y_N2[i] = this->var_nodes[i] - Y_N1[i];
}

template <typename B, typename R>
void Decoder_LDPC_BP_layered<B,R>
::load(const mipp::vector<R>& Y_N)
{
	assert(Y_N.size() == this->var_nodes.size());

	// memory zones initialization
	if (this->init_flag)
	{
		std::fill(this->branches.begin(), this->branches.end(), (R)0);
		this->init_flag = false;
	}

	std::copy(Y_N.begin(), Y_N.end(), this->var_nodes.begin());
}

template <typename B, typename R>
void Decoder_LDPC_BP_layered<B,R>
::hard_decode()
{
	// actual decoding
	this->BP_decode();

	// set the flag so the branches can be reset to 0 only at the beginning of the loop in iterative decoding
	this->init_flag = true;
}

template <typename B, typename R>
void Decoder_LDPC_BP_layered<B,R>
::store(mipp::vector<B>& V_K) const
{
	assert(V_K.size() <= this->var_nodes.size());

	// take the hard decision
	auto loop_size = (int)V_K.size();
	for (auto i = 0; i < loop_size; i++)
		V_K[i] = !(this->var_nodes[i] >= 0);
}

// BP algorithm
template <typename B, typename R>
void Decoder_LDPC_BP_layered<B,R>
::BP_decode()
{
	for (auto ite = 0; ite < this->n_ite; ite++)
	{
		this->BP_process();

//		auto syndrome = this->BP_process();
//		// stop criterion
//		if (syndrome && ite > 20)
//			break;
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Decoder_LDPC_BP_layered<B_8,Q_8>;
template class Decoder_LDPC_BP_layered<B_16,Q_16>;
template class Decoder_LDPC_BP_layered<B_32,Q_32>;
template class Decoder_LDPC_BP_layered<B_64,Q_64>;
#else
template class Decoder_LDPC_BP_layered<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
