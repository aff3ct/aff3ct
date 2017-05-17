#include "Tools/Display/Terminal/BFER/Terminal_BFER.hpp"
#include "Tools/Display/Terminal/BFER/Terminal_BFER_legacy.hpp"

#include "Factory_terminal.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Terminal* Factory_terminal<B>
::build(const parameters &params,
        const float &snr_s,
        const float &snr_b,
        const Monitor<B> *monitor,
        const std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> &t_snr,
        const std::chrono::nanoseconds *d_decod_total)
{
	Terminal *terminal = nullptr;

	// build a terminal to display the BER/FER
	if (params.terminal.type == "LEGACY")
		terminal = new Terminal_BFER_legacy<B>(params.code.K_info, params.code.N_code, snr_b, *monitor, t_snr);
	else if (params.terminal.type == "STD")
		terminal = new Terminal_BFER<B>(params.code.K_info, params.code.N_code, snr_s, snr_b, *monitor, t_snr,
		                                d_decod_total);

	return terminal;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_terminal<B_8>;
template struct aff3ct::tools::Factory_terminal<B_16>;
template struct aff3ct::tools::Factory_terminal<B_32>;
template struct aff3ct::tools::Factory_terminal<B_64>;
#else
template struct aff3ct::tools::Factory_terminal<B>;
#endif
// ==================================================================================== explicit template instantiation
