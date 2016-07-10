#include "../../Modulator/BPSK/Modulator_BPSK.hpp"
#include "../../Modulator/BPSK/Modulator_BPSK_fast.hpp"

#include "Factory_modulator.hpp"

template <typename B, typename R>
Modulator<B,R>* Factory_modulator<B,R>
::build(const t_channel_param &chan_params)
{
	Modulator<B,R> *modulator = nullptr;

	// build the modulator
	if (chan_params.modulation_type == "BPSK")
		modulator = new Modulator_BPSK<B,R>();
	else if (chan_params.modulation_type == "BPSK_FAST")
		modulator = new Modulator_BPSK_fast<B,R>();

	return modulator;
}

// ==================================================================================== explicit template instantiation 
#include "../types.h"
#ifdef MULTI_PREC
template struct Factory_modulator<B_8,R_8>;
template struct Factory_modulator<B_16,R_16>;
template struct Factory_modulator<B_32,R_32>;
template struct Factory_modulator<B_64,R_64>;
#else
template struct Factory_modulator<B,R>;
#endif
// ==================================================================================== explicit template instantiation