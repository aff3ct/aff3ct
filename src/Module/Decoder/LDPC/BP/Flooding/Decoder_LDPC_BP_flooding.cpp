#include <limits>

#include "Tools/Display/bash_tools.h"
#include "Tools/Math/utils.h"

#include "Decoder_LDPC_BP_flooding.hpp"

// constexpr int C_to_V_max = 15; // saturation value for the LLRs/extrinsics

template <typename B, typename R>
Decoder_LDPC_BP_flooding<B,R>
::Decoder_LDPC_BP_flooding(const int &K, const int &N, const int& n_ite,
                           const AList_reader &alist_data,
                           const int n_frames,
                           const std::string name)
: Decoder_SISO<B,R>      (K, N, n_frames, 1, name            ),
  n_ite                  (n_ite                              ),
  n_V_nodes              (N                                  ), // same as N but more explicit
  n_C_nodes              (N - K                              ),
  n_branches             ((int)alist_data.get_n_branches()   ),
  init_flag              (false                              ),

  n_variables_per_parity (alist_data.get_n_VN_per_CN()       ),
  n_parities_per_variable(alist_data.get_n_CN_per_VN()       ),
  transpose              (alist_data.get_branches_transpose()),

  Y_N                    (N                                  ),
  V_K                    (K                                  ),
  Lp_N                   (N,                               -1), // -1 in order to fail when AZCW
  C_to_V                 (this->n_branches,                 0),
  V_to_C                 (this->n_branches,                 0)
{
	assert(N == (int)alist_data.get_n_VN());
//	assert(K == N - (int)alist_data.get_n_CN());
	assert(n_ite > 0);
}

template <typename B, typename R>
Decoder_LDPC_BP_flooding<B,R>
::~Decoder_LDPC_BP_flooding()
{
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding<B,R>
::soft_decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext)
{
	std::cerr << bold_red("(EE) This decoder does not support this interface.") << std::endl;
	std::exit(-1);
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding<B,R>
::_soft_decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2)
{
	assert(Y_N1.size() == Y_N2.size());
	assert(Y_N1.size() == this->Y_N.size());

	// memory zones initialization
	if (this->init_flag)
	{
		std::fill(this->C_to_V.begin(), this->C_to_V.end(), (R)0);
		this->init_flag = false;
	}

	// actual decoding
	this->BP_decode(Y_N1);

	// prepare for next round by processing extrinsic information
	for (auto i = 0; i < (int)Y_N2.size(); i++)
		Y_N2[i] = this->Lp_N[i] - Y_N1[i];

	// saturate<R>(Y_N2, (R)-C_to_V_max, (R)C_to_V_max);
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding<B,R>
::load(const mipp::vector<R>& Y_N)
{
	assert(Y_N.size() >= this->Y_N.size());
	std::copy(Y_N.begin(), Y_N.begin() + this->Y_N.size(), this->Y_N.begin());
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding<B,R>
::hard_decode()
{
	// memory zones initialization
	if (this->init_flag)
	{
		std::fill(this->C_to_V.begin(), this->C_to_V.end(), (R)0);
		this->init_flag = false;
	}

	// actual decoding
	this->BP_decode(this->Y_N);

	// take the hard decision
	for (unsigned i = 0; i < this->V_K.size(); i++)
		this->V_K[i] = !(this->Lp_N[i] >= 0);

	// set the flag so C_to_V structure can be reset to 0 only at the beginning of the loop in iterative decoding
	this->init_flag = true;
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding<B,R>
::store(mipp::vector<B>& V_K) const
{
	assert(V_K.size() >= this->V_K.size());
	std::copy(this->V_K.begin(), this->V_K.end(), V_K.begin());
}

// BP algorithm
template <typename B, typename R>
bool Decoder_LDPC_BP_flooding<B,R>
::BP_decode(const mipp::vector<R> &Y_N)
{
	// actual decoding
	auto syndrome = false;
	for (auto ite = 0; ite < this->n_ite; ite++)
	{
		// beginning of the iteration upon all the matrix lines
		R *C_to_V_ptr = this->C_to_V.data();
		R *V_to_C_ptr = this->V_to_C.data();

		for (auto i = 0; i < this->n_V_nodes; i++)
		{
			// VN node accumulate all the incoming messages
			const auto length = this->n_parities_per_variable[i];

			auto sum_C_to_V = (R)0;
			for (auto j = 0; j < length; j++)
				sum_C_to_V += C_to_V_ptr[j];

			// update the intern values
			const auto temp = Y_N[i] + sum_C_to_V;

			// generate the outcoming messages to the CNs
			for (auto j = 0; j < length; j++)
				V_to_C_ptr[j] = temp - C_to_V_ptr[j];

			C_to_V_ptr += length; // jump to the next node
			V_to_C_ptr += length; // jump to the next node
		}

		// specific inner code depending on the selected implementation (min-sum or sum-product for example)
		syndrome = this->BP_process();
		
		// make a saturation
		// saturate<R>(this->C_to_V, (R)-C_to_V_max, (R)C_to_V_max);

		// stop criterion
		if (syndrome)
			break;
	}

	// begining of the iteration upon all the matrix lines
	R *C_to_V_ptr = this->C_to_V.data();
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

	return syndrome;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Decoder_LDPC_BP_flooding<B_8,Q_8>;
template class Decoder_LDPC_BP_flooding<B_16,Q_16>;
template class Decoder_LDPC_BP_flooding<B_32,Q_32>;
template class Decoder_LDPC_BP_flooding<B_64,Q_64>;
#else
template class Decoder_LDPC_BP_flooding<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
