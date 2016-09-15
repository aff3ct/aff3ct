#include "Tools/Display/Terminal/BFER/Terminal_BFER.hpp"
#include "Tools/Display/Terminal/BFER/Terminal_BFER_legacy.hpp"

#include "Factory_terminal.hpp"

template <typename B, typename R>
Terminal* Factory_terminal<B,R>
::build(const parameters &params,
        const R snr,
        const Monitor<B> *monitor,
        const std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> &t_snr,
        const std::chrono::nanoseconds &d_decod_total)
{
	Terminal *terminal = nullptr;

	// build a terminal to display the BER/FER
	if(params.simulation.enable_leg_term)
		terminal = new Terminal_BFER_legacy<B,R>(snr, *monitor, t_snr);
	else
		terminal = new Terminal_BFER<B,R>(snr, *monitor, t_snr, d_decod_total, params.simulation.enable_dec_thr);

	return terminal;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct Factory_terminal<B_8,R_8>;
template struct Factory_terminal<B_16,R_16>;
template struct Factory_terminal<B_32,R_32>;
template struct Factory_terminal<B_64,R_64>;
#else
template struct Factory_terminal<B,R>;
#endif
// ==================================================================================== explicit template instantiation
