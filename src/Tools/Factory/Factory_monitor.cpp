#include "Module/Monitor/Standard/Monitor_std.hpp"

#include "Factory_monitor.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Monitor<B,R>* Factory_monitor<B,R>
::build(const parameters &params, const int n_frames)
{
	Monitor<B,R> *monitor = nullptr;

	// build the monitor
	monitor = new Monitor_std<B,R>(params.code.K, params.code.N + params.code.tail_length, params.monitor.n_frame_errors, n_frames);

	return monitor;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct Factory_monitor<B_8, R_8>;
template struct Factory_monitor<B_16,R_16>;
template struct Factory_monitor<B_32,R_32>;
template struct Factory_monitor<B_64,R_64>;
#else
template struct Factory_monitor<B,R>;
#endif
// ==================================================================================== explicit template instantiation
