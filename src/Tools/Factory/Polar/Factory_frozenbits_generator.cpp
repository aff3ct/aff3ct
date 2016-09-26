#include <cmath>

#include "Tools/Polar/Frozenbits_generator/Frozenbits_generator_file.hpp"
#include "Tools/Polar/Frozenbits_generator/Frozenbits_generator_TV.hpp"
#include "Tools/Polar/Frozenbits_generator/Frozenbits_generator_GA.hpp"

#include "Factory_frozenbits_generator.hpp"

template <typename B>
Frozenbits_generator<B>* Factory_frozenbits_generator<B>
::build(const t_simulation_param &simu_params, const t_code_param &code_params)
{
	Frozenbits_generator<B> *fb_generator = nullptr;

	// build the frozen bits generator
	if (!simu_params.awgn_codes_file.empty())
	{
		fb_generator = new Frozenbits_generator_file<B>(code_params.K, code_params.N_code, simu_params.awgn_codes_file);
	}
	else
	{
		if (code_params.fb_gen_method == "GA")
			fb_generator = new Frozenbits_generator_GA<B>(code_params.K, code_params.N_code, code_params.sigma);
#ifdef ENABLE_POLAR_BOUNDS
		else if (code_params.fb_gen_method == "TV")
			fb_generator = new Frozenbits_generator_TV<B>(code_params.K, code_params.N_code, simu_params.awgn_codes_dir, 
			                                              simu_params.bin_pb_path, code_params.sigma);
#endif
	}

	return fb_generator;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct Factory_frozenbits_generator<B_8>;
template struct Factory_frozenbits_generator<B_16>;
template struct Factory_frozenbits_generator<B_32>;
template struct Factory_frozenbits_generator<B_64>;
#else
template struct Factory_frozenbits_generator<B>;
#endif
// ==================================================================================== explicit template instantiation
