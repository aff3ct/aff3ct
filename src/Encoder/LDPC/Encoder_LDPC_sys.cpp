#include "Encoder_LDPC_sys.hpp"

template <typename B>
Encoder_LDPC_sys<B>
::Encoder_LDPC_sys(const int K, const int N, const int n_frames, const std::string name)
: Encoder_sys<B>(K, N, n_frames, name)
{
}

template <typename B>
Encoder_LDPC_sys<B>
::~Encoder_LDPC_sys()
{
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Encoder_LDPC_sys<B_8>;
template class Encoder_LDPC_sys<B_16>;
template class Encoder_LDPC_sys<B_32>;
template class Encoder_LDPC_sys<B_64>;
#else
template class Encoder_LDPC_sys<B>;
#endif
// ==================================================================================== explicit template instantiation