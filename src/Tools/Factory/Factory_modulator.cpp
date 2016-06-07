#include "../../Modulator/Modulator_BPSK.hpp"

#include "Factory_modulator.hpp"

template <typename B>
Modulator<B>* Factory_modulator<B>
::build()
{
	Modulator<B> *modulator = nullptr;

	// build the modulator
	modulator = new Modulator_BPSK<B>();

	return modulator;
}

// ==================================================================================== explicit template instantiation 
#include "../types.h"
#ifdef MULTI_PREC
template struct Factory_modulator<B_8>;
template struct Factory_modulator<B_16>;
template struct Factory_modulator<B_32>;
template struct Factory_modulator<B_64>;
#else
template struct Factory_modulator<B>;
#endif
// ==================================================================================== explicit template instantiation