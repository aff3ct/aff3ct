#include "Encoder_LDPC_sys.hpp"

template <typename B>
Encoder_LDPC_sys<B>
::Encoder_LDPC_sys(const int n_frames)
: Encoder_sys<B>(n_frames)
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