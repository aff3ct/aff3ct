#include "Tools/Exceptions/Cannot_allocate.hpp"

#include "Module/Source/AZCW/Source_AZCW.hpp"
#include "Module/Source/Random/Source_random.hpp"
#include "Module/Source/Random/Source_random_fast.hpp"
#include "Module/Source/User/Source_user.hpp"

#include "Factory_source.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Source<B>* Factory_source<B>
::build(const std::string type,
        const int         K,
        const std::string path,
        const int         seed,
        const int         n_frames)
{
	     if (type == "RAND"     ) return new Source_random     <B>(K, seed, n_frames);
	else if (type == "RAND_FAST") return new Source_random_fast<B>(K, seed, n_frames);
	else if (type == "AZCW"     ) return new Source_AZCW       <B>(K,       n_frames);
	else if (type == "USER"     ) return new Source_user       <B>(K, path, n_frames);

	throw Cannot_allocate("aff3ct::tools::Factory_source: the factory could not allocate the object.");
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_source<B_8>;
template struct aff3ct::tools::Factory_source<B_16>;
template struct aff3ct::tools::Factory_source<B_32>;
template struct aff3ct::tools::Factory_source<B_64>;
#else
template struct aff3ct::tools::Factory_source<B>;
#endif
// ==================================================================================== explicit template instantiation
