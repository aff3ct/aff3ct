#include "Factory_monitor.hpp"

#include "Module/Monitor/Standard/Monitor_std.hpp"

template <typename B>
Monitor<B>* Factory_monitor<B>
::build(const parameters &params, const int n_frames)
{
	Monitor<B> *monitor = nullptr;

	// build the monitor
	monitor = new Monitor_std<B>(params.code.K, params.code.N, params.monitor.n_frame_errors, n_frames);

	return monitor;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct Factory_monitor<B_8>;
template struct Factory_monitor<B_16>;
template struct Factory_monitor<B_32>;
template struct Factory_monitor<B_64>;
#else
template struct Factory_monitor<B>;
#endif
// ==================================================================================== explicit template instantiation
