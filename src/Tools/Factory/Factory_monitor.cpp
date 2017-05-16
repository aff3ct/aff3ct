#include "Tools/Factory/Factory_modulator.hpp"

#include "Module/Monitor/Standard/Monitor_std.hpp"

#include "Factory_monitor.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Monitor<B>* Factory_monitor<B>
::build(const parameters &params)
{
	Monitor<B> *monitor = nullptr;

	// build the monitor
	monitor = new Monitor_std<B>(params.code.K_info,
	                             params.monitor.n_frame_errors,
	                             params.simulation.inter_frame_level);

	return monitor;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_monitor<B_8>;
template struct aff3ct::tools::Factory_monitor<B_16>;
template struct aff3ct::tools::Factory_monitor<B_32>;
template struct aff3ct::tools::Factory_monitor<B_64>;
#else
template struct aff3ct::tools::Factory_monitor<B>;
#endif
// ==================================================================================== explicit template instantiation
