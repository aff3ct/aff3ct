#include "Module/Source/AZCW/Source_AZCW.hpp"
#include "Module/Source/Random/Source_random.hpp"
#include "Module/Source/Random/Source_random_fast.hpp"
#include "Module/Source/User/Source_user.hpp"

#include "Factory_source.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Source<B>* Factory_source<B>
::build(const parameters &params, const int seed)
{
	Source<B> *source = nullptr;

	// build the generator
	if (params.source.type == "RAND_FAST")
		source = new Source_random_fast<B>(params.code.K, seed);
	else if (params.source.type == "RAND")
		source = new Source_random<B>(params.code.K, seed);
	else if (params.source.type == "AZCW")
		source = new Source_AZCW<B>(params.code.K);
	else if (params.source.type == "USER")
		source = new Source_user<B>(params.code.K, params.source.path);

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
