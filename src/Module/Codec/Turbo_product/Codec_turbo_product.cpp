#include "Tools/Exception/exception.hpp"
#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Encoder/BCH/Encoder_BCH.hpp"
#include "Factory/Module/Decoder/BCH/Decoder_BCH.hpp"
#include "Factory/Module/Puncturer/Puncturer.hpp"

#include "Codec_turbo_product.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename Q>
Codec_turbo_product<B,Q>
::Codec_turbo_product(const factory::Encoder_turbo_product::parameters &enc_params,
                      const factory::Decoder_turbo_product::parameters &dec_params)
: Codec          <B,Q>(enc_params.K, enc_params.N_cw, enc_params.N_cw, 0, enc_params.n_frames),
  Codec_SISO_SIHO<B,Q>(enc_params.K, enc_params.N_cw, enc_params.N_cw, 0, enc_params.n_frames),
  GF_poly(dec_params.sub->N_cw, dec_params.sub->t),
  enc_bch(nullptr),
  dec_bch(nullptr),
  cp     (nullptr)
{
	const std::string name = "Codec_turbo_product";
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

	// ---------------------------------------------------------------------------------------------------- allocations
	this->set_interleaver(factory::Interleaver_core::build<>(*dec_params.itl->core));

	factory::Puncturer::parameters pctno_params;
	pctno_params.type     = "NO";
	pctno_params.K        = enc_params.K;
	pctno_params.N        = enc_params.N_cw;
	pctno_params.N_cw     = enc_params.N_cw;
	pctno_params.n_frames = enc_params.n_frames;

	this->set_puncturer(factory::Puncturer::build<B,Q>(pctno_params));


	enc_bch = factory::Encoder_BCH::build     <B  >(*enc_params.sub, GF_poly);
	dec_bch = dynamic_cast<Decoder_BCH<B,Q>*>(factory::Decoder_BCH::build_hiho<B,Q>(*dec_params.sub, GF_poly));
	cp = new Decoder_chase_pyndiah<B,Q>(dec_bch->get_K(), dec_bch->get_N() + (dec_params.parity_extended?1:0),
	                                    *dec_bch, *enc_bch,
	                                    dec_params.n_least_reliable_positions,
	                                    dec_params.n_test_vectors,
	                                    dec_params.n_competitors,
	                                    dec_params.cp_coef);


	Encoder_turbo_product<B> *encoder_tpc = nullptr;
	try
	{
		encoder_tpc = factory::Encoder_turbo_product::build<B>(enc_params, this->get_interleaver_bit(), *enc_bch, *enc_bch);
		this->set_encoder(encoder_tpc);
	}
	catch (tools::cannot_allocate const&)
	{
		this->set_encoder(factory::Encoder::build<B>(enc_params));
	}

	try
	{
		auto decoder_siso_siho = factory::Decoder_turbo_product::build_siso<B,Q>(dec_params, this->get_interleaver_llr(), *cp, *cp);
		this->set_decoder_siso(decoder_siso_siho);
		this->set_decoder_siho(decoder_siso_siho);
	}
	catch (tools::cannot_allocate const&)
	{
		this->set_decoder_siho(factory::Decoder_turbo_product::build<B,Q>(dec_params, this->get_interleaver_llr(), *cp, *cp));
	}
}

template <typename B, typename Q>
Codec_turbo_product<B,Q>
::~Codec_turbo_product()
{
	if (enc_bch != nullptr) { delete enc_bch; enc_bch = nullptr; }
	if (dec_bch != nullptr) { delete dec_bch; dec_bch = nullptr; }
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Codec_turbo_product<B_8,Q_8>;
template class aff3ct::module::Codec_turbo_product<B_16,Q_16>;
template class aff3ct::module::Codec_turbo_product<B_32,Q_32>;
template class aff3ct::module::Codec_turbo_product<B_64,Q_64>;
#else
template class aff3ct::module::Codec_turbo_product<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
