#include <cmath>

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_file.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_TV.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_GA.hpp"

#include "Factory_frozenbits_generator.hpp"

template <typename B>
Frozenbits_generator<B>* Factory_frozenbits_generator<B>
::build(const parameters &params)
{
	Frozenbits_generator<B> *fb_generator = nullptr;

	// build the frozen bits generator
	if (!params.code.awgn_fb_file.empty())
	{
		fb_generator = new Frozenbits_generator_file<B>(params.code.K, params.code.N_code, params.code.awgn_fb_file);
	}
	else
	{
		if (params.code.fb_gen_method == "GA")
			fb_generator = new Frozenbits_generator_GA<B>(params.code.K, params.code.N_code, params.code.sigma);
#ifdef ENABLE_POLAR_BOUNDS
		else if (params.code.fb_gen_method == "TV")
			fb_generator = new Frozenbits_generator_TV<B>(params.code.K, params.code.N_code, params.code.awgn_fb_path,
			                                              params.simulation.bin_pb_path, params.code.sigma);
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
