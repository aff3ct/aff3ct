#include <stdexcept>

#include "Module/Encoder/NO/Encoder_NO.hpp"
#include "Module/Decoder/NO/Decoder_NO.hpp"

#include "Codec_uncoded.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Codec_uncoded<B,Q>
::Codec_uncoded(const parameters& params)
: Codec_SISO<B,Q>(params)
{
	if (params.code.K != params.code.N_code)
		throw std::invalid_argument("aff3ct::tools::Codec_uncoded: \"K\" has to be equal to \"N_code\".");
}

template <typename B, typename Q>
Codec_uncoded<B,Q>
::~Codec_uncoded()
{
}

template <typename B, typename Q>
Encoder<B>* Codec_uncoded<B,Q>
::build_encoder(const int tid, const Interleaver<int>* itl)
{
	return new Encoder_NO<B>(this->params.code.K,
	                         this->params.simulation.inter_frame_level);
}

template <typename B, typename Q>
SISO<Q>* Codec_uncoded<B,Q>
::build_siso(const int tid, const Interleaver<int>* itl, CRC<B>* crc)
{
	return new Decoder_NO<B,Q>(this->params.code.K,
	                           this->params.simulation.inter_frame_level);
}

template <typename B, typename Q>
Decoder<B,Q>* Codec_uncoded<B,Q>
::build_decoder(const int tid, const Interleaver<int>* itl, CRC<B>* crc)
{
	return new Decoder_NO<B,Q>(this->params.code.K,
	                           this->params.simulation.inter_frame_level);
}

template <typename B, typename Q>
void Codec_uncoded<B,Q>
::extract_sys_par(const mipp::vector<Q> &Y_N, mipp::vector<Q> &sys, mipp::vector<Q> &par)
{
	const auto K = this->params.code.K;

	if ((int)Y_N.size() != K * this->params.simulation.inter_frame_level)
		throw std::length_error("aff3ct::tools::Codec_uncoded: invalid \"Y_N\" size.");
	if ((int)sys.size() != K * this->params.simulation.inter_frame_level)
		throw std::length_error("aff3ct::tools::Codec_uncoded: invalid \"sys\" size.");
	if ((int)par.size() != 0)
		throw std::length_error("aff3ct::tools::Codec_uncoded: invalid \"par\" size.");

	std::copy(Y_N.begin(), Y_N.end(), sys.begin());
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Codec_uncoded<B_8,Q_8>;
template class aff3ct::tools::Codec_uncoded<B_16,Q_16>;
template class aff3ct::tools::Codec_uncoded<B_32,Q_32>;
template class aff3ct::tools::Codec_uncoded<B_64,Q_64>;
#else
template class aff3ct::tools::Codec_uncoded<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
