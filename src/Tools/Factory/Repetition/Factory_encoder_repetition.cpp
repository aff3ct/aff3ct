#include <stdexcept>

#include "Module/Encoder/Repetition/Encoder_repetition_sys.hpp"

#include "Factory_encoder_repetition.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder_sys<B>* Factory_encoder_repetition<B>
::build(const std::string type,
        const int         K,
        const int         N,
        const bool        buffered,
        const int         n_frames)
{
	if (type == "REPETITION") return new Encoder_repetition_sys<B>(K, N, buffered, n_frames);

	throw std::runtime_error("aff3ct::tools::Factory_encoder_repetition: the factory could not allocate the object.");
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_encoder_repetition<B_8>;
template struct aff3ct::tools::Factory_encoder_repetition<B_16>;
template struct aff3ct::tools::Factory_encoder_repetition<B_32>;
template struct aff3ct::tools::Factory_encoder_repetition<B_64>;
#else
template struct aff3ct::tools::Factory_encoder_repetition<B>;
#endif
// ==================================================================================== explicit template instantiation
