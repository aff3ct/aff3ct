#include "Factory_puncturer_turbo.hpp"
#include "Module/Puncturer/NO/Puncturer_NO.hpp"
#include "Module/Puncturer/Turbo/Puncturer_turbo.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Puncturer<B,Q>* Factory_puncturer_turbo<B,Q>
::build(const parameters &params)
{
	Puncturer<B,Q> *puncturer = nullptr;

	if (params.code.N + params.code.tail_length != params.code.N_code + params.code.tail_length)
		puncturer = new Puncturer_turbo<B,Q>(params.code.K,
		                                     params.code.N,
		                                     params.code.tail_length,
		                                     params.puncturer.pattern,
		                                     params.encoder.buffered,
		                                     params.simulation.inter_frame_level);
	else
		puncturer = new Puncturer_NO<B,Q>(params.code.K,
		                                  params.code.N + params.code.tail_length,
		                                  params.simulation.inter_frame_level);

	return puncturer;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_puncturer_turbo<B_8,Q_8>;
template struct aff3ct::tools::Factory_puncturer_turbo<B_16,Q_16>;
template struct aff3ct::tools::Factory_puncturer_turbo<B_32,Q_32>;
template struct aff3ct::tools::Factory_puncturer_turbo<B_64,Q_64>;
#else
template struct aff3ct::tools::Factory_puncturer_turbo<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
