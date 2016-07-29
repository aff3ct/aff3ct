#include "../../../Tools/bash_tools.h"

#include "Decoder_LDPC_BP_naive.hpp"

template <typename B, typename R>
Decoder_LDPC_BP_naive<B,R>
::Decoder_LDPC_BP_naive()
: Decoder<B,R>()
{
	// get K and N to pre-allocate Y_N and V_K inner buffers
}

template <typename B, typename R>
Decoder_LDPC_BP_naive<B,R>
::~Decoder_LDPC_BP_naive()
{
}

template <typename B, typename R>
void Decoder_LDPC_BP_naive<B,R>
::decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext)
{
	std::cerr << bold_red("(EE) This decoder does not support this interface.") << std::endl;
	std::exit(-1);
}

template <typename B, typename R>
void Decoder_LDPC_BP_naive<B,R>
::decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2)
{
	assert(Y_N1.size() == Y_N2.size());

	// do something here => BP algo
}

template <typename B, typename R>
void Decoder_LDPC_BP_naive<B,R>
::load(const mipp::vector<R>& Y_N)
{
	this->Y_N = Y_N;
}

template <typename B, typename R>
void Decoder_LDPC_BP_naive<B,R>
::decode()
{
	// read LLR from this->Y_N

	// do something here => BP algo

	// store LLR to this->V_K
}

template <typename B, typename R>
void Decoder_LDPC_BP_naive<B,R>
::store(mipp::vector<B>& V_K) const
{
	assert(V_K.size() == this->V_K.size());
	V_K = this->V_K;
}

// ==================================================================================== explicit template instantiation 
#include "../../../Tools/types.h"
#ifdef MULTI_PREC
template class Decoder_LDPC_BP_naive<B_8,Q_8>;
template class Decoder_LDPC_BP_naive<B_16,Q_16>;
template class Decoder_LDPC_BP_naive<B_32,Q_32>;
template class Decoder_LDPC_BP_naive<B_64,Q_64>;
#else
template class Decoder_LDPC_BP_naive<B,Q>;
#endif
// ==================================================================================== explicit template instantiation