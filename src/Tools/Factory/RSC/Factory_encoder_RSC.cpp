#include "Tools/Exception/cannot_allocate.hpp"

#include "Module/Encoder/RSC/Encoder_RSC_generic_sys.hpp"
#include "Module/Encoder/RSC/Encoder_RSC_generic_json_sys.hpp"

#include "Factory_encoder_RSC.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder_RSC_sys<B>* Factory_encoder_RSC<B>
::build(const std::string       type,
        const int               K,
        const int               N,
        const bool              buffered,
        const std::vector<int>  poly,
              std::ostream     &stream,
        const int               n_frames)
{
	     if (type == "RSC_JSON") return new Encoder_RSC_generic_json_sys<B>(K, N, buffered, poly, stream, n_frames);
	else if (type == "RSC"     ) return new Encoder_RSC_generic_sys     <B>(K, N, buffered, poly,         n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_encoder_RSC<B_8>;
template struct aff3ct::tools::Factory_encoder_RSC<B_16>;
template struct aff3ct::tools::Factory_encoder_RSC<B_32>;
template struct aff3ct::tools::Factory_encoder_RSC<B_64>;
#else
template struct aff3ct::tools::Factory_encoder_RSC<B>;
#endif
// ==================================================================================== explicit template instantiation
