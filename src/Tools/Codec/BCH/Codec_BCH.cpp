#include "Tools/Factory/BCH/Factory_encoder_BCH.hpp"
#include "Tools/Factory/BCH/Factory_decoder_BCH.hpp"

#include "Codec_BCH.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Codec_BCH<B,Q>
::Codec_BCH(const parameters& params)
: Codec<B,Q>(params),
  GF(params.code.K, params.code.N, params.code.m, params.code.t)
{
	// assertion are made in the Galois Field (GF)
}

template <typename B, typename Q>
Codec_BCH<B,Q>
::~Codec_BCH()
{
}

template <typename B, typename Q>
Encoder<B>* Codec_BCH<B,Q>
::build_encoder(const int tid, const module::Interleaver<int>* itl)
{
	return Factory_encoder_BCH<B>::build(this->params.encoder.type,
	                                     this->params.code.K,
	                                     this->params.code.N_code,
	                                     GF,
	                                     this->params.simulation.inter_frame_level);
}

template <typename B, typename Q>
Decoder<B,Q>* Codec_BCH<B,Q>
::build_decoder(const int tid, const module::Interleaver<int>* itl, module::CRC<B>* crc)
{
	return Factory_decoder_BCH<B,Q>::build(this->params.decoder.type,
	                                       this->params.decoder.implem,
	                                       this->params.code.K,
	                                       this->params.code.N_code,
	                                       this->params.code.t,
	                                       GF,
	                                       this->params.simulation.inter_frame_level);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Codec_BCH<B_8,Q_8>;
template class aff3ct::tools::Codec_BCH<B_16,Q_16>;
template class aff3ct::tools::Codec_BCH<B_32,Q_32>;
template class aff3ct::tools::Codec_BCH<B_64,Q_64>;
#else
template class aff3ct::tools::Codec_BCH<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
