#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Encoder/BCH/Encoder_BCH.hpp"
#include "Module/Decoder/BCH/Decoder_BCH.hpp"
#include "Module/Decoder/Turbo_product/Chase_pyndiah/Decoder_chase_pyndiah.hpp"
#include "Factory/Module/Puncturer/Puncturer.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"
#include "Factory/Tools/Interleaver/Interleaver_core.hpp"
#include "Tools/Codec/Turbo_product/Codec_turbo_product.hpp"
#include "Module/Decoder/Turbo_product/Chase_pyndiah/Decoder_chase_pyndiah.hpp"
#include "Module/Decoder/Turbo_product/Chase_pyndiah/Decoder_chase_pyndiah_fast.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename Q>
Codec_turbo_product<B,Q>
::Codec_turbo_product(const factory::Encoder_turbo_product &enc_params,
                      const factory::Decoder_turbo_product &dec_params,
                      const factory::Interleaver           &itl_params)
: Codec_SISO<B,Q>(enc_params.K, enc_params.N_cw, enc_params.N_cw),
  GF_poly(new BCH_polynomial_generator<B>(dec_params.sub->N_cw, dec_params.sub->t))
{
	// ----------------------------------------------------------------------------------------------------- exceptions
	if (enc_params.K != dec_params.K)
	{
		std::stringstream message;
		message << "'enc_params.K' has to be equal to 'dec_params.K' ('enc_params.K' = " << enc_params.K
		        << ", 'dec_params.K' = " << dec_params.K << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (enc_params.N_cw != dec_params.N_cw)
	{
		std::stringstream message;
		message << "'enc_params.N_cw' has to be equal to 'dec_params.N_cw' ('enc_params.N_cw' = " << enc_params.N_cw
		        << ", 'dec_params.N_cw' = " << dec_params.N_cw << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (dec_params.sub->implem == "GENIUS")
	{
		std::stringstream message;
		message << "sub decoder can't have a GENIUS implem (dec_params.sub->implem).";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	// ---------------------------------------------------------------------------------------------------- allocations
	factory::Puncturer pctno_params;
	pctno_params.type = "NO";
	pctno_params.K    = enc_params.K;
	pctno_params.N    = enc_params.N_cw;
	pctno_params.N_cw = enc_params.N_cw;

	this->set_puncturer  (pctno_params.build<B,Q>());
	this->set_interleaver(itl_params.core->build<>());

	int N_cw_p = enc_params.sub->N_cw + (dec_params.parity_extended ? 1 : 0);

	std::unique_ptr<module::Encoder_BCH<B>> enc_bch(enc_params.sub->build<B>(*GF_poly));
	enc_bch->set_n_frames(N_cw_p);

	std::unique_ptr<module::Decoder_BCH<B,Q>> dec_bch(
		dynamic_cast<module::Decoder_BCH<B,Q>*>(dec_params.sub->build<B,Q>(*GF_poly)));
	dec_bch->set_n_frames(N_cw_p);

	std::unique_ptr<module::Decoder_chase_pyndiah<B,Q>> dec_cp;
	if (dec_params.implem == "FAST")
	{
		dec_cp.reset(new module::Decoder_chase_pyndiah_fast<B,Q>(dec_bch->get_K(), N_cw_p,
		                                                         *dec_bch, *enc_bch,
		                                                         dec_params.n_least_reliable_positions,
		                                                         dec_params.n_test_vectors,
		                                                         dec_params.n_competitors,
		                                                         dec_params.cp_coef));
		dec_cp->set_n_frames(N_cw_p);
	}
	else
	{
		dec_cp.reset(new module::Decoder_chase_pyndiah<B,Q>(dec_bch->get_K(), N_cw_p,
		                                                    *dec_bch, *enc_bch,
		                                                    dec_params.n_least_reliable_positions,
		                                                    dec_params.n_test_vectors,
		                                                    dec_params.n_competitors,
		                                                    dec_params.cp_coef));
		dec_cp->set_n_frames(N_cw_p);
	}

	(*const_cast<std::string*>(&dec_params.implem)) = "STD";

	try
	{
		this->set_encoder(enc_params.build<B>(*enc_bch, *enc_bch, this->get_interleaver_bit()));
	}
	catch (cannot_allocate const&)
	{
		this->set_encoder(static_cast<const factory::Encoder*>(&enc_params)->build<B>());
	}

	try
	{
		this->set_decoder_siso(dec_params.build_siso<B,Q>(*dec_cp, *dec_cp, this->get_interleaver_llr()));
	}
	catch (cannot_allocate const&)
	{
		this->set_decoder_siho(dec_params.build<B,Q>(*dec_cp, *dec_cp, this->get_interleaver_llr()));
	}
}

template <typename B, typename Q>
Codec_turbo_product<B,Q>* Codec_turbo_product<B,Q>
::clone() const
{
	auto t = new Codec_turbo_product(*this);
	t->deep_copy(*this);
	return t;
}

template <typename B, typename Q>
const BCH_polynomial_generator<B>& Codec_turbo_product<B,Q>
::get_GF_poly() const
{
	return *this->GF_poly;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Codec_turbo_product<B_8,Q_8>;
template class aff3ct::tools::Codec_turbo_product<B_16,Q_16>;
template class aff3ct::tools::Codec_turbo_product<B_32,Q_32>;
template class aff3ct::tools::Codec_turbo_product<B_64,Q_64>;
#else
template class aff3ct::tools::Codec_turbo_product<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
