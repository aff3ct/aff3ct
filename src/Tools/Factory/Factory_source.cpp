#include "../../Source/AZCW/Source_AZCW.hpp"
#include "../../Source/Random/Source_random.hpp"
#include "../../Source/Random/Source_random_fast.hpp"
#include "../../Source/Fixed/Source_fixed.hpp"

#include "Factory_source.hpp"

template <typename B>
Source<B>* Factory_source<B>
::build(const t_code_param &code_params, const int seed)
{
	Source<B> *source = nullptr;

	// build the generator
	if (code_params.generation_method == "RAND_FAST")
		source = new Source_random_fast<B>(code_params.K, seed);
	else if (code_params.generation_method == "RAND")
		source = new Source_random<B>(code_params.K, seed);
	else if (code_params.generation_method == "AZCW")
		source = new Source_AZCW<B>(code_params.K);
	else if (code_params.generation_method == "FIXED")
		source = new Source_fixed<B>(code_params.K);


	return source;
}

// ==================================================================================== explicit template instantiation 
#include "../types.h"
#ifdef MULTI_PREC
template struct Factory_source<B_8>;
template struct Factory_source<B_16>;
template struct Factory_source<B_32>;
template struct Factory_source<B_64>;
#else
template struct Factory_source<B>;
#endif
// ==================================================================================== explicit template instantiation
