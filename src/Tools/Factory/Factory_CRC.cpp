#include "Tools/Exceptions/Cannot_allocate.hpp"

#include "Module/CRC/NO/CRC_NO.hpp"
#include "Module/CRC/Polynomial/CRC_polynomial.hpp"
#include "Module/CRC/Polynomial/CRC_polynomial_fast.hpp"
#include "Module/CRC/Polynomial/CRC_polynomial_inter.hpp"

#include "Factory_CRC.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
CRC<B>* Factory_CRC<B>
::build(const std::string type,
        const int         K,
        const int         size,
        const std::string poly,
        const int         n_frames)
{
	     if (type == "STD"  ) return new CRC_polynomial      <B>(K, poly, size, n_frames);
	else if (type == "FAST" ) return new CRC_polynomial_fast <B>(K, poly, size, n_frames);
	else if (type == "INTER") return new CRC_polynomial_inter<B>(K, poly, size, n_frames);
	else if (type == "NO"   ) return new CRC_NO              <B>(K,             n_frames);

	throw Cannot_allocate("aff3ct::tools::Factory_CRC: the factory could not allocate the object.");
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_CRC<B_8>;
template struct aff3ct::tools::Factory_CRC<B_16>;
template struct aff3ct::tools::Factory_CRC<B_32>;
template struct aff3ct::tools::Factory_CRC<B_64>;
#else
template struct aff3ct::tools::Factory_CRC<B>;
#endif
// ==================================================================================== explicit template instantiation
