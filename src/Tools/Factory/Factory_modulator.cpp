#include "../Math/max.h"

#include "../../Modulator/BPSK/Modulator_BPSK.hpp"
#include "../../Modulator/BPSK/Modulator_BPSK_fast.hpp"
#include "../../Modulator/PAM/Modulator_PAM.hpp"
#include "../../Modulator/QAM/Modulator_QAM.hpp"
#include "../../Modulator/PSK/Modulator_PSK.hpp"

#include "Factory_modulator.hpp"

template <typename B, typename R>
Modulator<B,R>* Factory_modulator<B,R>
::build(const t_mod_param &mod_params, const float sigma)
{
	Modulator<B,R> *modulator = nullptr;

	// build the modulator
	if (mod_params.type == "BPSK")
		modulator = new Modulator_BPSK<B,R>(sigma);
	else if (mod_params.type == "BPSK_FAST")
		modulator = new Modulator_BPSK_fast<B,R>(sigma);
	else if (mod_params.type == "PAM")
	{
		if (mod_params.demod_max == "MAX")
			modulator = new Modulator_PAM<B,R,max<R>>(mod_params.bits_per_symbol, sigma);
		else if (mod_params.demod_max == "MAXL")
			modulator = new Modulator_PAM<B,R,max_linear<R>>(mod_params.bits_per_symbol, sigma);
		else if (mod_params.demod_max == "MAXS")
			modulator = new Modulator_PAM<B,R,max_star<R>>(mod_params.bits_per_symbol, sigma);
		else if (mod_params.demod_max == "MAXSS")
			modulator = new Modulator_PAM<B,R,max_star_safe<R>>(mod_params.bits_per_symbol, sigma);
	}
	else if (mod_params.type == "QAM")
	{
		if (mod_params.demod_max == "MAX")
			modulator = new Modulator_QAM<B,R,max<R>>(mod_params.bits_per_symbol, sigma);
		else if (mod_params.demod_max == "MAXL")
			modulator = new Modulator_QAM<B,R,max_linear<R>>(mod_params.bits_per_symbol, sigma);
		else if (mod_params.demod_max == "MAXS")
			modulator = new Modulator_QAM<B,R,max_star<R>>(mod_params.bits_per_symbol, sigma);
		else if (mod_params.demod_max == "MAXSS")
			modulator = new Modulator_QAM<B,R,max_star_safe<R>>(mod_params.bits_per_symbol, sigma);
	}
	else if (mod_params.type == "PSK")
	{
		if (mod_params.demod_max == "MAX")
			modulator = new Modulator_PSK<B,R,max<R>>(mod_params.bits_per_symbol, sigma);
		else if (mod_params.demod_max == "MAXL")
			modulator = new Modulator_PSK<B,R,max_linear<R>>(mod_params.bits_per_symbol, sigma);
		else if (mod_params.demod_max == "MAXS")
			modulator = new Modulator_PSK<B,R,max_star<R>>(mod_params.bits_per_symbol, sigma);
		else if (mod_params.demod_max == "MAXSS")
			modulator = new Modulator_PSK<B,R,max_star_safe<R>>(mod_params.bits_per_symbol, sigma);
	}

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
