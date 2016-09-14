#include "Module/Error/Standard/Error_analyzer_std.hpp"

#include "Factory_error_analyzer.hpp"

template <typename B>
Error_analyzer<B>* Factory_error_analyzer<B>
::build(const parameters &params, const int n_frames)
{
	Error_analyzer<B> *analyzer = nullptr;

	// build the analyser
	analyzer = new Error_analyzer_std<B>(params.code.K, params.code.N, params.simulation.max_fe, n_frames);

	return analyzer;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct Factory_error_analyzer<B_8>;
template struct Factory_error_analyzer<B_16>;
template struct Factory_error_analyzer<B_32>;
template struct Factory_error_analyzer<B_64>;
#else
template struct Factory_error_analyzer<B>;
#endif
// ==================================================================================== explicit template instantiation
