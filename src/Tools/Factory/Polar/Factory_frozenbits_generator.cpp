#include <stdexcept>

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_file.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_TV.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_GA.hpp"

#include "Factory_frozenbits_generator.hpp"

using namespace aff3ct::tools;

template <typename B>
Frozenbits_generator<B>* Factory_frozenbits_generator<B>
::build(const std::string type,
        const int         K,
        const int         N,
        const float       sigma,
        const std::string path_fb,
        const std::string path_pb)
{
	     if (type == "GA"  ) return new Frozenbits_generator_GA  <B>(K, N,                   sigma);
	else if (type == "TV"  ) return new Frozenbits_generator_TV  <B>(K, N, path_fb, path_pb, sigma);
	else if (type == "FILE") return new Frozenbits_generator_file<B>(K, N, path_fb                );

	throw std::runtime_error("aff3ct::tools::Factory_frozenbits_generator: the factory could not allocate the object.");
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_frozenbits_generator<B_8>;
template struct aff3ct::tools::Factory_frozenbits_generator<B_16>;
template struct aff3ct::tools::Factory_frozenbits_generator<B_32>;
template struct aff3ct::tools::Factory_frozenbits_generator<B_64>;
#else
template struct aff3ct::tools::Factory_frozenbits_generator<B>;
#ifndef PREC_32_BIT
template struct aff3ct::tools::Factory_frozenbits_generator<B_32>;
#endif
#endif
// ==================================================================================== explicit template instantiation
