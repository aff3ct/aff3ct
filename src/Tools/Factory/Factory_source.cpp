#include "../../Source/Source_random.hpp"
#include "../../Source/Source_random_fast.hpp"

#include "Factory_source.hpp"

template <typename B>
Source<B>* Factory_source<B>
::build(const t_code_param &code_params)
{
	Source<B> *source = nullptr;

	// build the generator
	if (code_params.generation_method == "RAND_FAST")
		source = new Source_random_fast<B>();
	else if (code_params.generation_method == "RAND")
		source = new Source_random<B>();
	else if (code_params.generation_method == "AZCW")
		source = new Source_random_fast<B>();

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
