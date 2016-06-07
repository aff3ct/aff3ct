#include "Modulator_BPSK.hpp"

template <typename B>
Modulator_BPSK<B>
::Modulator_BPSK()
{
}

template <typename B>
Modulator_BPSK<B>
::~Modulator_BPSK()
{
}

template <typename B>
void Modulator_BPSK<B>
::modulate(mipp::vector<B>& X_N) const
{
	auto size = X_N.size();
	for (unsigned i = 0; i < size; i++)
		X_N[i] = 1 - (X_N[i] + X_N[i]); // (X_N[i] == 1) ? -1 : +1
}

// ==================================================================================== explicit template instantiation 
#include "../Tools/types.h"
#ifdef MULTI_PREC
template class Modulator_BPSK<B_8>;
template class Modulator_BPSK<B_16>;
template class Modulator_BPSK<B_32>;
template class Modulator_BPSK<B_64>;
#else
template class Modulator_BPSK<B>;
#endif
// ==================================================================================== explicit template instantiation