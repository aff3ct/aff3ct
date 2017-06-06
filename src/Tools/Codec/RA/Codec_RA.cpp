#include "Tools/Factory/Factory_interleaver.hpp"
#include "Tools/Factory/RA/Factory_encoder_RA.hpp"
#include "Tools/Factory/RA/Factory_decoder_RA.hpp"

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
	return Factory_interleaver<int>::build(this->params.interleaver.type,
	                                       this->params.code.N_code,
	                                       this->params.interleaver.path,
	                                       this->params.interleaver.uniform,
	                                       this->params.interleaver.n_cols,
	                                       seed,
	                                       this->params.simulation.inter_frame_level);
}

template <typename B, typename Q>
Encoder<B>* Codec_RA<B,Q>
::build_encoder(const int tid, const Interleaver<int>* itl)
{
	if (itl == nullptr)
		throw std::runtime_error("aff3ct::tools::Codec_RA: \"itl\" should not be null.");

	return Factory_encoder_RA<B>::build(this->params.encoder.type,
	                                    this->params.code.K,
	                                    this->params.code.N_code,
	                                    *itl,
	                                    this->params.simulation.inter_frame_level);
}

template <typename B, typename Q>
Decoder<B,Q>* Codec_RA<B,Q>
::build_decoder(const int tid, const Interleaver<int>* itl, CRC<B>* crc)
{
	if (itl == nullptr)
		throw std::runtime_error("aff3ct::tools::Codec_RA: \"itl\" should not be null.");

	return Factory_decoder_RA<B,Q>::build(this->params.decoder.type,
	                                      this->params.decoder.implem,
	                                      this->params.code.K,
	                                      this->params.code.N_code,
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
