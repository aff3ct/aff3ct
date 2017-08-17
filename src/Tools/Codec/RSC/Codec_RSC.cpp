#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Codec_RSC.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename Q, typename QD>
Codec_RSC<B,Q,QD>
::Codec_RSC(const factory::Encoder_RSC::parameters &enc_params,
            const factory::Decoder_RSC::parameters &dec_params)
: Codec_SISO<B,Q>(enc_params, dec_params), enc_par(enc_params), dec_par(dec_params)
{
	auto enc_cpy = enc_params;
	enc_cpy.type = "RSC";

	auto encoder_RSC = factory::Encoder_RSC::build<B>(enc_cpy);
	trellis = encoder_RSC->get_trellis();
	delete encoder_RSC;
}

template <typename B, typename Q, typename QD>
Codec_RSC<B,Q,QD>
::~Codec_RSC()
{
}

template <typename B, typename Q, typename QD>
module::Encoder_RSC_sys<B>* Codec_RSC<B,Q,QD>
::build_encoder(const int tid, const module::Interleaver<int>* itl)
{
	return factory::Encoder_RSC::build<B>(enc_par, std::cout);
}

template <typename B, typename Q, typename QD>
module::Decoder_SISO_SIHO<B,Q>* Codec_RSC<B,Q,QD>
::build_decoder_siso(const int tid, const module::Interleaver<int>* itl, module::CRC<B>* crc)
{
	return factory::Decoder_RSC::build<B,Q,QD>(dec_par, trellis, std::cout);
}

template <typename B, typename Q, typename QD>
module::Decoder_SISO<Q>* Codec_RSC<B,Q,QD>
::build_siso(const int tid, const module::Interleaver<int>* itl, module::CRC<B>* crc)
{
	return this->build_decoder_siso(tid, itl, crc);
}

template <typename B, typename Q, typename QD>
module::Decoder_SIHO<B,Q>* Codec_RSC<B,Q,QD>
::build_decoder(const int tid, const module::Interleaver<int>* itl, module::CRC<B>* crc)
{
	return this->build_decoder_siso(tid, itl, crc);
}

template <typename B, typename Q, typename QD>
void Codec_RSC<B,Q,QD>
::extract_sys_par(const mipp::vector<Q> &Y_N, mipp::vector<Q> &sys, mipp::vector<Q> &par)
{
	const auto K    = dec_par.K;
	const auto N    = dec_par.N_cw;
	const auto tb_2 = enc_par.tail_length / 2;

	if ((int)Y_N.size() != N * dec_par.n_frames)
	{
		std::stringstream message;
		message << "'Y_N.size()' has to be equal to 'N' * 'inter_frame_level' ('Y_N.size()' = " << Y_N.size()
		        << ", 'N' = " << N << ", 'inter_frame_level' = " << dec_par.n_frames << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)sys.size() != (K + tb_2) * dec_par.n_frames)
	{
		std::stringstream message;
		message << "'sys.size()' has to be equal to ('K' + 'tb_2') * 'inter_frame_level' ('sys.size()' = " << sys.size()
		        << ", 'K' = " << K << ", 'tb_2' = " << tb_2 << ", 'inter_frame_level' = "
		        << dec_par.n_frames << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)par.size() != (K + tb_2) * dec_par.n_frames)
	{
		std::stringstream message;
		message << "'par.size()' has to be equal to ('K' + 'tb_2') * 'inter_frame_level' ('par.size()' = " << par.size()
		        << ", 'K' = " << K << ", 'tb_2' = " << tb_2 << ", 'inter_frame_level' = "
		        << dec_par.n_frames << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	// extract systematic and parity information
	for (auto f = 0; f < dec_par.n_frames; f++)
	{
		if (enc_par.buffered)
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
