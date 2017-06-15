#include "Tools/Exceptions/Cannot_allocate.hpp"

#include "Module/Puncturer/NO/Puncturer_NO.hpp"
#include "Module/Puncturer/Turbo/Puncturer_turbo.hpp"

#include "Factory_puncturer_turbo.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Puncturer<B,Q>* Factory_puncturer_turbo<B,Q>
::build(const std::string type,
        const int         K,
        const int         N,
        const int         tail_length,
        const std::string pattern,
        const bool        buffered,
        const int         n_frames)
{
	if (type == "TURBO") return new Puncturer_turbo<B,Q>(K, N, tail_length, pattern, buffered, n_frames);

	throw Cannot_allocate("aff3ct::tools::Factory_puncturer_turbo: the factory could not allocate the object.");
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_puncturer_turbo<B_8,Q_8>;
template struct aff3ct::tools::Factory_puncturer_turbo<B_16,Q_16>;
template struct aff3ct::tools::Factory_puncturer_turbo<B_32,Q_32>;
template struct aff3ct::tools::Factory_puncturer_turbo<B_64,Q_64>;
#else
template struct aff3ct::tools::Factory_puncturer_turbo<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
