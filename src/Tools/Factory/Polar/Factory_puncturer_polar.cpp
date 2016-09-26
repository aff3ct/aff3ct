#include "Tools/bash_tools.h"

#include "Module/Puncturer/NO/Puncturer_NO.hpp"
#include "Module/Puncturer/Polar/Puncturer_polar_wangliu.hpp"

#include "Factory_puncturer_polar.hpp"

template <typename B, typename Q>
Puncturer<B,Q>* Factory_puncturer_polar<B,Q>
::build(const t_code_param &code_params, const t_decoder_param &deco_params, 
        const Frozenbits_generator<B> *fb_generator)
{
	Puncturer<B,Q> *puncturer = nullptr;

	if (code_params.N != code_params.N_code)
		puncturer = new Puncturer_polar_wangliu<B,Q>(code_params.K, code_params.N, *fb_generator);
	else
		puncturer = new Puncturer_NO<B,Q>(code_params.K, code_params.N);

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
