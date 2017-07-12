#include "Tools/Exception/exception.hpp"

#include "Module/Coset/Bit/Coset_bit.hpp"

#include "Factory_coset_bit.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Coset<B,B>* Factory_coset_bit
::build(const std::string type,
        const int         K,
        const int         n_frames)
{
	if (type == "STD") return new Coset_bit<B,B>(K, n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Coset<B_8 ,B_8 >* aff3ct::tools::Factory_coset_bit::build<B_8 >(const std::string, const int, const int);
template aff3ct::module::Coset<B_16,B_16>* aff3ct::tools::Factory_coset_bit::build<B_16>(const std::string, const int, const int);
template aff3ct::module::Coset<B_32,B_32>* aff3ct::tools::Factory_coset_bit::build<B_32>(const std::string, const int, const int);
template aff3ct::module::Coset<B_64,B_64>* aff3ct::tools::Factory_coset_bit::build<B_64>(const std::string, const int, const int);
#else
template aff3ct::module::Coset<B,B>* aff3ct::tools::Factory_coset_bit::build<B>(const std::string, const int, const int);
#endif
// ==================================================================================== explicit template instantiation
