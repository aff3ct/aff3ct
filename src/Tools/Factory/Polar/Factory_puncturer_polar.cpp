#include "Tools/Display/bash_tools.h"

#include "Module/Puncturer/NO/Puncturer_NO.hpp"
#include "Module/Puncturer/Polar/Puncturer_polar_wangliu.hpp"

#include "Factory_puncturer_polar.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Puncturer<B,Q>* Factory_puncturer_polar<B,Q>
::build(const parameters &params, const Frozenbits_generator<B> *fb_generator)
{
	Puncturer<B,Q> *puncturer = nullptr;

	if (params.code.N != params.code.N_code)
		puncturer = new Puncturer_polar_wangliu<B,Q>(params.code.K, params.code.N, *fb_generator);
	else
		puncturer = new Puncturer_NO<B,Q>(params.code.K, params.code.N);

	return puncturer;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct Factory_puncturer_polar<B_8,Q_8>;
template struct Factory_puncturer_polar<B_16,Q_16>;
template struct Factory_puncturer_polar<B_32,Q_32>;
template struct Factory_puncturer_polar<B_64,Q_64>;
#else
template struct Factory_puncturer_polar<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
