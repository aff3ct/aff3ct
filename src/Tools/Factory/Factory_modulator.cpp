#include "../../Modulator/Modulator_BPSK.hpp"
#include "../../Modulator/Modulator_PAM.hpp"

#include "Factory_modulator.hpp"


template <typename B, typename R>
Modulator<B,R>* Factory_modulator<B,R>
::build(const t_mod_param &mod_params, const R sigma)
{
	Modulator<B,R> *modulator = nullptr;

	if (mod_params.mod_type == "PAM")
		modulator = new Modulator_PAM<B,R>(mod_params.bits_per_symbol, sigma);
	else
		modulator = new Modulator_BPSK<B,R>();

	// build the modulator


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
