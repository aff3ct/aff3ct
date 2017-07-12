#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_file.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_TV.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_GA.hpp"

#include "Factory_frozenbits_generator.hpp"

using namespace aff3ct::tools;

template <typename B>
Frozenbits_generator<B>* Factory_frozenbits_generator
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

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::tools::Frozenbits_generator<B_8 >* aff3ct::tools::Factory_frozenbits_generator::build<B_8 >(const std::string, const int, const int, const float, const std::string, const std::string);
template aff3ct::tools::Frozenbits_generator<B_16>* aff3ct::tools::Factory_frozenbits_generator::build<B_16>(const std::string, const int, const int, const float, const std::string, const std::string);
template aff3ct::tools::Frozenbits_generator<B_32>* aff3ct::tools::Factory_frozenbits_generator::build<B_32>(const std::string, const int, const int, const float, const std::string, const std::string);
template aff3ct::tools::Frozenbits_generator<B_64>* aff3ct::tools::Factory_frozenbits_generator::build<B_64>(const std::string, const int, const int, const float, const std::string, const std::string);
#else
template aff3ct::tools::Frozenbits_generator<B>* aff3ct::tools::Factory_frozenbits_generator::build<B>(const std::string, const int, const int, const float, const std::string, const std::string);
#ifndef PREC_32_BIT
template aff3ct::tools::Frozenbits_generator<B_32>* aff3ct::tools::Factory_frozenbits_generator::build<B_32>(const std::string, const int, const int, const float, const std::string, const std::string);
#endif
#endif
// ==================================================================================== explicit template instantiation
