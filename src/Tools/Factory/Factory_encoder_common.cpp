#include "Tools/Exceptions/Cannot_allocate.hpp"

#include "Module/Encoder/NO/Encoder_NO.hpp"
#include "Module/Encoder/AZCW/Encoder_AZCW.hpp"
#include "Module/Encoder/Coset/Encoder_coset.hpp"
#include "Module/Encoder/User/Encoder_user.hpp"

#include "Factory_encoder_common.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder<B>* Factory_encoder_common<B>
::build(const std::string type,
        const int         K,
        const int         N,
        const std::string path,
        const int         seed,
        const int         n_frames)
{
	     if (type == "NO"   ) return new Encoder_NO   <B>(K,          n_frames);
	else if (type == "AZCW" ) return new Encoder_AZCW <B>(K, N,       n_frames);
	else if (type == "COSET") return new Encoder_coset<B>(K, N, seed, n_frames);
	else if (type == "USER" ) return new Encoder_user <B>(K, N, path, n_frames);

	throw Cannot_allocate("aff3ct::tools::Factory_encoder_common: the factory could not allocate the object.");
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_encoder_common<B_8>;
template struct aff3ct::tools::Factory_encoder_common<B_16>;
template struct aff3ct::tools::Factory_encoder_common<B_32>;
template struct aff3ct::tools::Factory_encoder_common<B_64>;
#else
template struct aff3ct::tools::Factory_encoder_common<B>;
#endif
// ==================================================================================== explicit template instantiation
