#include <limits>
#include <cmath>

#include "Tools/Display/bash_tools.h"
#include "Tools/Math/utils.h"

#include "Decoder_LDPC_BP_layered.hpp"

template <typename B, typename R>
Decoder_LDPC_BP_layered<B,R>
::Decoder_LDPC_BP_layered(const int &K, const int &N, const int& n_ite,
                          const AList_reader &alist_data,
                          const bool enable_syndrome,
                          const int n_frames,
                          const std::string name)
: Decoder_SISO<B,R>(K, N, n_frames, 1, name                                  ),
  cur_frame        (0                                                        ),
  n_ite            (n_ite                                                    ),
  n_C_nodes        ((int)alist_data.get_n_CN()                               ),
  enable_syndrome  (enable_syndrome                                          ),
  init_flag        (false                                                    ),
  CN_to_VN         (alist_data.get_CN_to_VN()                                ),
  var_nodes        (n_frames, mipp::vector<R>(N,                           0)),
  branches         (n_frames, mipp::vector<R>(alist_data.get_n_branches(), 0))
{
	assert(N == (int)alist_data.get_n_VN());
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
	assert(Y_N1.size() >= this->var_nodes[cur_frame].size());

	// memory zones initialization
	load(Y_N1);

	// actual decoding
	this->BP_decode();

	// prepare for next round by processing extrinsic information
	for (auto i = 0; i < this->N; i++)
		Y_N2[i] = this->var_nodes[cur_frame][i] - Y_N1[i];

	// copy extrinsic information into var_nodes for next TURBO iteration
	std::copy(Y_N2.begin(), Y_N2.begin() + this->N, this->var_nodes[cur_frame].begin());

	cur_frame = (cur_frame +1) % SISO<R>::n_frames;
}

template <typename B, typename R>
void Decoder_LDPC_BP_layered<B,R>
::load(const mipp::vector<R>& Y_N)
{
	assert(Y_N.size() >= this->var_nodes[cur_frame].size());

	// memory zones initialization
	if (this->init_flag)
	{
		std::fill(this->branches [cur_frame].begin(), this->branches [cur_frame].end(), (R)0);
		std::fill(this->var_nodes[cur_frame].begin(), this->var_nodes[cur_frame].end(), (R)0);

		if (cur_frame == Decoder<B,R>::n_frames -1)
			this->init_flag = false;
	}

	for (auto i = 0; i < (int)var_nodes[cur_frame].size(); i++)
		this->var_nodes[cur_frame][i] += Y_N[i]; // var_nodes contain previous extrinsic information
}

template <typename B, typename R>
void Decoder_LDPC_BP_layered<B,R>
::hard_decode()
{
	// actual decoding
	this->BP_decode();

	// set the flag so the branches can be reset to 0 only at the beginning of the loop in iterative decoding
	if (cur_frame == Decoder<B,R>::n_frames -1)
		this->init_flag = true;

	cur_frame = (cur_frame +1) % SISO<R>::n_frames;
}

template <typename B, typename R>
void Decoder_LDPC_BP_layered<B,R>
::store(mipp::vector<B>& V_K) const
{
	assert((int)V_K.size() >= this->K);

	const auto past_frame = (cur_frame -1) < 0 ? Decoder<B,R>::n_frames -1 : cur_frame -1;

	// take the hard decision
	for (auto i = 0; i < this->K; i++)
		V_K[i] = !(this->var_nodes[past_frame][i] >= 0);
}

// BP algorithm
template <typename B, typename R>
void Decoder_LDPC_BP_layered<B,R>
::BP_decode()
{
	for (auto ite = 0; ite < this->n_ite; ite++)
	{
		this->BP_process(this->var_nodes[cur_frame], this->branches[cur_frame]);

		// stop criterion
		if (this->enable_syndrome && this->check_syndrome()) break;
	}
}

template <typename B, typename R>
bool Decoder_LDPC_BP_layered<B,R>
::check_syndrome()
{
	auto syndrome = false;

	auto k = 0;
	for (auto i = 0; i < this->n_C_nodes; i++)
	{
		auto sign = 0;

		const auto n_VN = (int)this->CN_to_VN[i].size();
		for (auto j = 0; j < n_VN; j++)
		{
			const auto value = this->var_nodes[cur_frame][this->CN_to_VN[i][j]] - this->branches[cur_frame][k++];
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
template class Decoder_LDPC_BP_layered<B_8,Q_8>;
template class Decoder_LDPC_BP_layered<B_16,Q_16>;
template class Decoder_LDPC_BP_layered<B_32,Q_32>;
template class Decoder_LDPC_BP_layered<B_64,Q_64>;
#else
template class Decoder_LDPC_BP_layered<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
