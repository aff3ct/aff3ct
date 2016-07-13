#include "../Math/max.h"

#include "../../Modulator/BPSK/Modulator_BPSK.hpp"
#include "../../Modulator/BPSK/Modulator_BPSK_fast.hpp"
#include "../../Modulator/PAM/Modulator_PAM.hpp"
#include "../../Modulator/QAM/Modulator_QAM.hpp"
#include "../../Modulator/PSK/Modulator_PSK.hpp"

#include "Factory_modulator.hpp"

template <typename B, typename R, typename Q>
Modulator<B,R,Q>* Factory_modulator<B,R,Q>
::build(const t_mod_param &mod_params, const float sigma)
{
	Modulator<B,R,Q> *modulator = nullptr;

	// build the modulator
	if (mod_params.type == "BPSK")
		modulator = new Modulator_BPSK<B,R,Q>(sigma);
	else if (mod_params.type == "BPSK_FAST")
		modulator = new Modulator_BPSK_fast<B,R,Q>(sigma);

	return modulator;
}

// ==================================================================================== explicit template instantiation 
#include "../types.h"
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
