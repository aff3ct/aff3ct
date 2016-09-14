#include "Module/Source/AZCW/Source_AZCW.hpp"
#include "Module/Source/Random/Source_random.hpp"
#include "Module/Source/Random/Source_random_fast.hpp"
#include "Module/Source/Fixed/Source_fixed.hpp"

#include "Factory_source.hpp"

template <typename B>
Source<B>* Factory_source<B>
::build(const parameters &params, const int seed)
{
	Source<B> *source = nullptr;

	// build the generator
	if (params.code.generation_method == "RAND_FAST")
		source = new Source_random_fast<B>(params.code.K, seed);
	else if (params.code.generation_method == "RAND")
		source = new Source_random<B>(params.code.K, seed);
	else if (params.code.generation_method == "AZCW")
		source = new Source_AZCW<B>(params.code.K);
	else if (params.code.generation_method == "FIXED")
		source = new Source_fixed<B>(params.code.K);


	return source;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct Factory_source<B_8>;
template struct Factory_source<B_16>;
template struct Factory_source<B_32>;
template struct Factory_source<B_64>;
#else
template struct Factory_source<B>;
#endif
// ==================================================================================== explicit template instantiation
