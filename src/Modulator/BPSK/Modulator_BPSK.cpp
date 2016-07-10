#include "Modulator_BPSK.hpp"

template <typename B, typename R>
Modulator_BPSK<B,R>
::Modulator_BPSK()
{
}

template <typename B, typename R>
Modulator_BPSK<B,R>
::~Modulator_BPSK()
{
}

template <typename B, typename R>
void Modulator_BPSK<B,R>
::modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2) const
{
	auto size = X_N1.size();
	for (unsigned i = 0; i < size; i++)
		X_N2[i] = 1 - (X_N1[i] + X_N1[i]); // (X_N[i] == 1) ? -1 : +1
}

template <typename B, typename R>
void Modulator_BPSK<B,R>
::demodulate(const mipp::vector<R>& Y_N1, mipp::vector<R>& Y_N2) const
{
	Y_N2 = Y_N1;
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Modulator_BPSK<B_8,R_8>;
template class Modulator_BPSK<B_16,R_16>;
template class Modulator_BPSK<B_32,R_32>;
template class Modulator_BPSK<B_64,R_64>;
#else
template class Modulator_BPSK<B,R>;
#endif
// ==================================================================================== explicit template instantiation