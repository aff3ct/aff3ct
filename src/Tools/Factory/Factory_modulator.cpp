#include "Tools/Math/max.h"

#include "Module/Modulator/BPSK/Modulator_BPSK.hpp"
#include "Module/Modulator/BPSK/Modulator_BPSK_fast.hpp"
#include "Module/Modulator/PAM/Modulator_PAM.hpp"
#include "Module/Modulator/QAM/Modulator_QAM.hpp"
#include "Module/Modulator/PSK/Modulator_PSK.hpp"
#include "Module/Modulator/CPM/Modulator_CPM.hpp"
#include "Module/Modulator/User/Modulator_user.hpp"

#include "Factory_modulator.hpp"

template <typename B, typename R, typename Q>
Modulator<B,R,Q>* Factory_modulator<B,R,Q>
::build(const parameters &params, const float sigma)
{
	Modulator<B,R,Q> *modulator = nullptr;

	// build the modulator
	if (params.modulator.type == "BPSK")
		modulator = new Modulator_BPSK<B,R,Q>(params.code.N + params.code.tail_length, sigma, params.demodulator.no_sig2);
	else if (params.modulator.type == "BPSK_FAST")
		modulator = new Modulator_BPSK_fast<B,R,Q>(params.code.N + params.code.tail_length, sigma, params.demodulator.no_sig2);
	else if (params.modulator.type == "PAM")
	{
		if (params.demodulator.max == "MAX")
			modulator = new Modulator_PAM<B,R,Q,max<Q>>(params.code.N + params.code.tail_length, sigma, params.modulator.bits_per_symbol, params.demodulator.no_sig2);
		else if (params.demodulator.max == "MAXL")
			modulator = new Modulator_PAM<B,R,Q,max_linear<Q>>(params.code.N + params.code.tail_length, sigma, params.modulator.bits_per_symbol, params.demodulator.no_sig2);
		else if (params.demodulator.max == "MAXS")
			modulator = new Modulator_PAM<B,R,Q,max_star<Q>>(params.code.N + params.code.tail_length, sigma, params.modulator.bits_per_symbol, params.demodulator.no_sig2);
		else if (params.demodulator.max == "MAXSS")
			modulator = new Modulator_PAM<B,R,Q,max_star_safe<Q>>(params.code.N + params.code.tail_length, sigma, params.modulator.bits_per_symbol, params.demodulator.no_sig2);
	}
	else if (params.modulator.type == "QAM")
	{
		if (params.demodulator.max == "MAX")
			modulator = new Modulator_QAM<B,R,Q,max<Q>>(params.code.N + params.code.tail_length, sigma, params.modulator.bits_per_symbol, params.demodulator.no_sig2);
		else if (params.demodulator.max == "MAXL")
			modulator = new Modulator_QAM<B,R,Q,max_linear<Q>>(params.code.N + params.code.tail_length, sigma, params.modulator.bits_per_symbol, params.demodulator.no_sig2);
		else if (params.demodulator.max == "MAXS")
			modulator = new Modulator_QAM<B,R,Q,max_star<Q>>(params.code.N + params.code.tail_length, sigma, params.modulator.bits_per_symbol, params.demodulator.no_sig2);
		else if (params.demodulator.max == "MAXSS")
			modulator = new Modulator_QAM<B,R,Q,max_star_safe<Q>>(params.code.N + params.code.tail_length, sigma, params.modulator.bits_per_symbol, params.demodulator.no_sig2);
	}
	else if (params.modulator.type == "PSK")
	{
		if (params.demodulator.max == "MAX")
			modulator = new Modulator_PSK<B,R,Q,max<Q>>(params.code.N + params.code.tail_length, sigma, params.modulator.bits_per_symbol, params.demodulator.no_sig2);
		else if (params.demodulator.max == "MAXL")
			modulator = new Modulator_PSK<B,R,Q,max_linear<Q>>(params.code.N + params.code.tail_length, sigma, params.modulator.bits_per_symbol, params.demodulator.no_sig2);
		else if (params.demodulator.max == "MAXS")
			modulator = new Modulator_PSK<B,R,Q,max_star<Q>>(params.code.N + params.code.tail_length, sigma, params.modulator.bits_per_symbol, params.demodulator.no_sig2);
		else if (params.demodulator.max == "MAXSS")
			modulator = new Modulator_PSK<B,R,Q,max_star_safe<Q>>(params.code.N + params.code.tail_length, sigma, params.modulator.bits_per_symbol, params.demodulator.no_sig2);
	}
	else if (params.modulator.type == "CPM")
	{
		if (params.demodulator.max == "MAX")
			modulator = new Modulator_CPM<B,R,Q,max<Q>>(params.code.N + params.code.tail_length, params.modulator, sigma);
		else if (params.demodulator.max == "MAXL")
			modulator = new Modulator_CPM<B,R,Q,max_linear<Q>>(params.code.N + params.code.tail_length, params.modulator, sigma);
		else if (params.demodulator.max == "MAXS")
			modulator = new Modulator_CPM<B,R,Q,max_star<Q>>(params.code.N + params.code.tail_length, params.modulator, sigma);
		else if (params.demodulator.max == "MAXSS")
			modulator = new Modulator_CPM<B,R,Q,max_star_safe<Q>>(params.code.N + params.code.tail_length, params.modulator, sigma);
	}
	else if (params.modulator.type == "USER")
	{
		if (params.demodulator.max == "MAX")
			modulator = new Modulator_user<B,R,Q,max<Q>>(params.code.N + params.code.tail_length, sigma, params.modulator.bits_per_symbol, params.modulator.const_path, params.demodulator.no_sig2);
		else if (params.demodulator.max == "MAXL")
			modulator = new Modulator_user<B,R,Q,max_linear<Q>>(params.code.N + params.code.tail_length, sigma, params.modulator.bits_per_symbol, params.modulator.const_path, params.demodulator.no_sig2);
		else if (params.demodulator.max == "MAXS")
			modulator = new Modulator_user<B,R,Q,max_star<Q>>(params.code.N + params.code.tail_length, sigma, params.modulator.bits_per_symbol, params.modulator.const_path, params.demodulator.no_sig2);
		else if (params.demodulator.max == "MAXSS")
			modulator = new Modulator_user<B,R,Q,max_star_safe<Q>>(params.code.N + params.code.tail_length, sigma, params.modulator.bits_per_symbol, params.modulator.const_path, params.demodulator.no_sig2);
	}

	return modulator;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct Factory_modulator<B_8,R_8,R_8>;
template struct Factory_modulator<B_8,R_8,Q_8>;
template struct Factory_modulator<B_16,R_16,R_16>;
template struct Factory_modulator<B_16,R_16,Q_16>;
template struct Factory_modulator<B_32,R_32,R_32>;
template struct Factory_modulator<B_64,R_64,R_64>;
#else
template struct Factory_modulator<B,R,Q>;
#if !defined(PREC_32_BIT) && !defined(PREC_64_BIT)
template struct Factory_modulator<B,R,R>;
#endif
#endif
// ==================================================================================== explicit template instantiation
