#include "Tools/Exception/exception.hpp"

#include "Module/Puncturer/NO/Puncturer_NO.hpp"
#include "Module/Puncturer/Polar/Puncturer_polar_wangliu.hpp"

#include "Factory_puncturer_polar.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Puncturer<B,Q>* Factory_puncturer_polar<B,Q>
::build(const std::string              type,
        const int                      K,
        const int                      N,
        const Frozenbits_generator<B> *fb_generator,
        const int                      n_frames)
{
	if (type == "WANGLIU") return new Puncturer_polar_wangliu<B,Q>(K, N, *fb_generator, n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_puncturer_polar<B_8,Q_8>;
template struct aff3ct::tools::Factory_puncturer_polar<B_16,Q_16>;
template struct aff3ct::tools::Factory_puncturer_polar<B_32,Q_32>;
template struct aff3ct::tools::Factory_puncturer_polar<B_64,Q_64>;
#else
template struct aff3ct::tools::Factory_puncturer_polar<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
