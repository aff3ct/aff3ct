#include "Module/Coset/Real/Coset_real.hpp"

#include "Factory_coset_real.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Coset<B,Q>* Factory_coset_real<B,Q>
::build(const std::string type,
        const int         N,
        const int         n_frames)
{
	if (type == "STD") return new Coset_real<B,Q>(N, n_frames);

	return nullptr;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_coset_real<B_8,  Q_8>;
template struct aff3ct::tools::Factory_coset_real<B_16, Q_16>;
template struct aff3ct::tools::Factory_coset_real<B_32, Q_32>;
template struct aff3ct::tools::Factory_coset_real<B_64, Q_64>;
#else
template struct aff3ct::tools::Factory_coset_real<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
