#include <stdexcept>

#include "Tools/Factory/RSC/Factory_encoder_RSC.hpp"
#include "Tools/Factory/RSC/Factory_decoder_RSC.hpp"

#include "Codec_RSC.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q, typename QD>
Codec_RSC<B,Q,QD>
::Codec_RSC(const parameters& params)
: Codec_SISO<B,Q>(params)
{
	auto encoder_RSC = this->build_encoder();
	if (encoder_RSC != nullptr)
	{
		trellis = encoder_RSC->get_trellis();
		delete encoder_RSC;
	}
	else
		throw std::runtime_error("aff3ct::tools::Codec_RSC: \"trellis\" can't be created.");
}

template <typename B, typename Q, typename QD>
Codec_RSC<B,Q,QD>
::~Codec_RSC()
{
}

template <typename B, typename Q, typename QD>
Encoder_RSC_sys<B>* Codec_RSC<B,Q,QD>
::build_encoder(const int tid, const module::Interleaver<int>* itl)
{
	return Factory_encoder_RSC<B>::build(this->params.encoder.type,
	                                     this->params.code.K,
	                                     this->params.code.N_code,
	                                     this->params.encoder.buffered,
	                                     this->params.encoder.poly,
	                                     std::cout,
	                                     this->params.simulation.inter_frame_level);
}

template <typename B, typename Q, typename QD>
Decoder_SISO<B,Q>* Codec_RSC<B,Q,QD>
::build_decoder_siso(const int tid, const module::Interleaver<int>* itl, module::CRC<B>* crc)
{
	return Factory_decoder_RSC<B,Q,QD>::build(this->params.decoder.type,
	                                          this->params.decoder.implem,
	                                          this->params.code.K,
	                                          trellis,
	                                          this->params.decoder.max,
	                                          this->params.decoder.simd_strategy,
	                                          this->params.encoder.buffered,
	                                          std::cout,
	                                          1,
	                                          this->params.simulation.inter_frame_level);
}

template <typename B, typename Q, typename QD>
SISO<Q>* Codec_RSC<B,Q,QD>
::build_siso(const int tid, const module::Interleaver<int>* itl, module::CRC<B>* crc)
{
	return this->build_decoder_siso(tid, itl, crc);
}

template <typename B, typename Q, typename QD>
Decoder<B,Q>* Codec_RSC<B,Q,QD>
::build_decoder(const int tid, const module::Interleaver<int>* itl, module::CRC<B>* crc)
{
	return this->build_decoder_siso(tid, itl, crc);
}

template <typename B, typename Q, typename QD>
void Codec_RSC<B,Q,QD>
::extract_sys_par(const mipp::vector<Q> &Y_N, mipp::vector<Q> &sys, mipp::vector<Q> &par)
{
	const auto K    = this->params.code.K;
	const auto N    = this->params.code.N_code;
	const auto tb_2 = this->params.code.tail_length / 2;

	if ((int)Y_N.size() != N * this->params.simulation.inter_frame_level)
		throw std::length_error("aff3ct::tools::Codec_RSC: invalid \"Y_N\" size.");
	if ((int)sys.size() != (K + tb_2) * this->params.simulation.inter_frame_level)
		throw std::length_error("aff3ct::tools::Codec_RSC: invalid \"sys\" size.");
	if ((int)par.size() != (K + tb_2) * this->params.simulation.inter_frame_level)
		throw std::length_error("aff3ct::tools::Codec_RSC: invalid \"par\" size.");

	// extract systematic and parity information
	for (auto f = 0; f < this->params.simulation.inter_frame_level; f++)
	{
		if (this->params.encoder.buffered)
		{
			std::copy(Y_N.begin() + f * N,
			          Y_N.begin() + f * N + K + tb_2,
			          sys.begin() + f *    (K + tb_2));

			std::copy(Y_N.begin() + f * N + K + tb_2,
			          Y_N.begin() + f * N + N,
			          par.begin() + f *    (K + tb_2));
		}
		else
		{
			for (auto i = 0; i < K + tb_2; i++)
			{
				sys[f * (K + tb_2) +i] = Y_N[f * N + i*2 +0];
				par[f * (K + tb_2) +i] = Y_N[f * N + i*2 +1];
			}
		}
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Codec_RSC<B_8,Q_8,QD_8>;
template class aff3ct::tools::Codec_RSC<B_16,Q_16,QD_16>;
template class aff3ct::tools::Codec_RSC<B_32,Q_32,QD_32>;
template class aff3ct::tools::Codec_RSC<B_64,Q_64,QD_64>;
#else
template class aff3ct::tools::Codec_RSC<B,Q,QD>;
#endif
// ==================================================================================== explicit template instantiation
