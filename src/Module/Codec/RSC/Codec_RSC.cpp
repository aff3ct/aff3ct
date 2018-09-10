#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Factory/Module/Puncturer/Puncturer.hpp"

#include "Codec_RSC.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename Q>
Codec_RSC<B,Q>
::Codec_RSC(const factory::Encoder_RSC::parameters &enc_params,
            const factory::Decoder_RSC::parameters &dec_params)
: Codec          <B,Q>(enc_params.K, enc_params.N_cw, enc_params.N_cw, enc_params.tail_length, enc_params.n_frames),
  Codec_SISO_SIHO<B,Q>(enc_params.K, enc_params.N_cw, enc_params.N_cw, enc_params.tail_length, enc_params.n_frames),
  buffered_encoding(enc_params.buffered)
{
	const std::string name = "Codec_RSC";
	this->set_name(name);

	// ----------------------------------------------------------------------------------------------------- exceptions
	if (enc_params.K != dec_params.K)
	{
		std::stringstream message;
		message << "'enc_params.K' has to be equal to 'dec_params.K' ('enc_params.K' = " << enc_params.K
		        << ", 'dec_params.K' = " << dec_params.K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (enc_params.N_cw != dec_params.N_cw)
	{
		std::stringstream message;
		message << "'enc_params.N_cw' has to be equal to 'dec_params.N_cw' ('enc_params.N_cw' = " << enc_params.N_cw
		        << ", 'dec_params.N_cw' = " << dec_params.N_cw << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (enc_params.n_frames != dec_params.n_frames)
	{
		std::stringstream message;
		message << "'enc_params.n_frames' has to be equal to 'dec_params.n_frames' ('enc_params.n_frames' = "
		        << enc_params.n_frames << ", 'dec_params.n_frames' = " << dec_params.n_frames << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	// ---------------------------------------------------------------------------------------------------------- tools
	auto enc_cpy = enc_params;
	enc_cpy.type = "RSC";

	std::unique_ptr<Encoder_RSC_sys<B>> encoder_RSC(factory::Encoder_RSC::build<B>(enc_cpy));
	trellis = encoder_RSC->get_trellis();

	// ---------------------------------------------------------------------------------------------------- allocations
	factory::Puncturer::parameters pct_params;
	pct_params.type     = "NO";
	pct_params.K        = enc_params.K;
	pct_params.N        = enc_params.N_cw;
	pct_params.N_cw     = enc_params.N_cw;
	pct_params.n_frames = enc_params.n_frames;

	this->set_puncturer(std::shared_ptr<Puncturer<B,Q>>(factory::Puncturer::build<B,Q>(pct_params)));

	try
	{
		std::shared_ptr<Encoder_RSC_sys<B>> enc(factory::Encoder_RSC::build<B>(enc_params));
		this->set_encoder(std::static_pointer_cast<Encoder<B>>(enc));
	}
	catch (tools::cannot_allocate const&)
	{
		this->set_encoder(std::shared_ptr<Encoder<B>>(factory::Encoder::build<B>(enc_params)));
	}

	try
	{
		std::shared_ptr<Decoder_SISO_SIHO<B,Q>> decoder_siso_siho(factory::Decoder_RSC::build_siso<B,Q>(dec_params, trellis, std::cout, 1, this->get_encoder()));
		this->set_decoder_siho(std::static_pointer_cast<Decoder_SIHO<B,Q>>(decoder_siso_siho));
		this->set_decoder_siso(std::static_pointer_cast<Decoder_SISO<  Q>>(decoder_siso_siho));
	}
	catch (tools::cannot_allocate const&)
	{
		std::shared_ptr<Decoder_SIHO<B,Q>> dec(factory::Decoder_RSC::build<B,Q>(dec_params, trellis, std::cout, 1, this->get_encoder()));
		this->set_decoder_siho(dec);
	}
}

template <typename B, typename Q>
void Codec_RSC<B,Q>
::_extract_sys_par(const Q* Y_N, Q* sys, Q* par, const int frame_id)
{
	const auto tb_2 = this->tail_length / 2;
	const auto K    = this->K;
	const auto N_cw = this->N_cw;

	if (buffered_encoding)
	{
		std::copy(Y_N,            Y_N + K + tb_2, sys);
		std::copy(Y_N + K + tb_2, Y_N + N_cw,     par);
	}
	else
	{
		for (auto i = 0; i < K + tb_2; i++)
		{
			sys[i] = Y_N[i*2 +0];
			par[i] = Y_N[i*2 +1];
		}
	}
}

template <typename B, typename Q>
void Codec_RSC<B,Q>
::_extract_sys_llr(const Q* Y_N, Q* sys, const int frame_id)
{
	if (buffered_encoding)
		std::copy(Y_N, Y_N + this->K, sys);
	else
		for (auto i = 0; i < this->K; i++)
			sys[i] = Y_N[i*2];
}

template <typename B, typename Q>
void Codec_RSC<B,Q>
::_add_sys_ext(const Q* ext, Q* Y_N, const int frame_id)
{
	if (buffered_encoding)
		for (auto i = 0; i < this->K; i++)
			Y_N[i] += ext[i];
	else
		for (auto i = 0; i < this->K; i++)
			Y_N[i*2] += ext[i];
}

template <typename B, typename Q>
void Codec_RSC<B,Q>
::_extract_sys_bit(const Q *Y_N, B *V_K, const int frame_id)
{
	if (buffered_encoding)
		for (auto i = 0; i < this->K; i++)
			V_K[i] = Y_N[i] >= 0 ? (B)0 : (B)1;
	else
		for (auto i = 0; i < this->K; i++)
			V_K[i] = Y_N[2*i] >= 0 ? (B)0 : (B)1;
}


// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Codec_RSC<B_8,Q_8>;
template class aff3ct::module::Codec_RSC<B_16,Q_16>;
template class aff3ct::module::Codec_RSC<B_32,Q_32>;
template class aff3ct::module::Codec_RSC<B_64,Q_64>;
#else
template class aff3ct::module::Codec_RSC<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
