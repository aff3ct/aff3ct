#include "Tools/Factory/Factory_interleaver.hpp"

#include "Module/Encoder/RA/Encoder_RA.hpp"
#include "Module/Decoder/RA/Decoder_RA.hpp"

#include "Codec_RA.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Codec_RA<B,Q>
::Codec_RA(const parameters& params)
: Codec<B,Q>(params)
{
}

template <typename B, typename Q>
Codec_RA<B,Q>
::~Codec_RA()
{
}

template <typename B, typename Q>
Interleaver<int>* Codec_RA<B,Q>
::build_interleaver(const int tid, const int seed)
{
	return Factory_interleaver<int>::build(this->params, this->params.code.N, seed);
}

template <typename B, typename Q>
Encoder<B>* Codec_RA<B,Q>
::build_encoder(const int tid, const module::Interleaver<int>* itl)
{
	if (itl == nullptr)
		throw std::runtime_error("aff3ct::tools::Codec_RA: \"itl\" should not be null.");

	return new Encoder_RA<B>(this->params.code.K, this->params.code.N, *itl, this->params.simulation.inter_frame_level);
}

template <typename B, typename Q>
Decoder<B,Q>* Codec_RA<B,Q>
::build_decoder(const int tid, const module::Interleaver<int>* itl, module::CRC<B>* crc)
{
	if (itl == nullptr)
		throw std::runtime_error("aff3ct::tools::Codec_RA: \"itl\" should not be null.");

	return new Decoder_RA<B,Q>(this->params.code.K,
	                           this->params.code.N,
	                           *itl,
	                           this->params.decoder.n_ite,
	                           this->params.simulation.inter_frame_level);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Codec_RA<B_8,Q_8>;
template class aff3ct::tools::Codec_RA<B_16,Q_16>;
template class aff3ct::tools::Codec_RA<B_32,Q_32>;
template class aff3ct::tools::Codec_RA<B_64,Q_64>;
#else
template class aff3ct::tools::Codec_RA<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
