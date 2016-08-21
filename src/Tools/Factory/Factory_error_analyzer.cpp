#include "../../Error/Error_analyzer_std.hpp"

#include "Factory_error_analyzer.hpp"

template <typename B>
Error_analyzer<B>* Factory_error_analyzer<B>
::build(const t_simulation_param &simu_params, 
        const t_code_param &code_params, 
        const int n_frames)
{
	Error_analyzer<B> *analyzer = nullptr;

	// build the analyser
	analyzer = new Error_analyzer_std<B>(code_params.K, code_params.N, simu_params.max_fe, n_frames);

	return analyzer;
}

// ==================================================================================== explicit template instantiation 
#include "../types.h"
#ifdef MULTI_PREC
template struct Factory_error_analyzer<B_8>;
template struct Factory_error_analyzer<B_16>;
template struct Factory_error_analyzer<B_32>;
template struct Factory_error_analyzer<B_64>;
#else
template struct Factory_error_analyzer<B>;
#endif
// ==================================================================================== explicit template instantiation