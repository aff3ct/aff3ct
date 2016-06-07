#include "../../Error/Error_analyzer.hpp"

#include "Factory_error_analyzer.hpp"

template <typename B, typename R>
Error_analyzer<B,R>* Factory_error_analyzer<B,R>
::build(const t_simulation_param &simu_params, 
        const t_code_param &code_params, 
        const int n_frames)
{
	Error_analyzer<B,R> *analyzer = nullptr;

	// build the analyser
	analyzer = new Error_analyzer<B,R>(code_params.K, code_params.N, simu_params.max_fe, n_frames);

	return analyzer;
}

// ==================================================================================== explicit template instantiation 
#include "../types.h"
#ifdef MULTI_PREC
template struct Factory_error_analyzer<B_8,R_8>;
template struct Factory_error_analyzer<B_16,R_16>;
template struct Factory_error_analyzer<B_32,R_32>;
template struct Factory_error_analyzer<B_64,R_64>;
#else
template struct Factory_error_analyzer<B,R>;
#endif
// ==================================================================================== explicit template instantiation