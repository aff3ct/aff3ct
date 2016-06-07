#include "../Polar/Puncturer/Puncturer_polar_wangliu.hpp"

#include "../bash_tools.h"

#include "Factory_puncturer_polar.hpp"

template <typename B, typename R>
Puncturer_polar<B,R>* Factory_puncturer_polar<B,R>
::build(const t_code_param &code_params, const t_decoder_param &deco_params, 
        const Frozenbits_generator<B> *fb_generator)
{
	Puncturer_polar<B,R> *puncturer = nullptr;

	// build the generator
	if (code_params.puncturer == "WANGLIU")
	{
		puncturer = new Puncturer_polar_wangliu<B,R>(code_params.N, code_params.K, *fb_generator);
	}
	else
	{
		std::cerr << bold_red("(EE) Unknown puncturing type.") << std::endl;
		exit(EXIT_FAILURE);
	}

	return puncturer;
}

// ==================================================================================== explicit template instantiation 
#include "../types.h"
#ifdef MULTI_PREC
template struct Factory_puncturer_polar<B_8,Q_8>;
template struct Factory_puncturer_polar<B_16,Q_16>;
template struct Factory_puncturer_polar<B_32,Q_32>;
template struct Factory_puncturer_polar<B_64,Q_64>;
#else
template struct Factory_puncturer_polar<B,Q>;
#endif
// ==================================================================================== explicit template instantiation