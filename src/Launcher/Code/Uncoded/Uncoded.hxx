#include <iostream>

#include "Tools/Codec/Uncoded/Codec_uncoded.hpp"

#include "Uncoded.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B, typename R, typename Q>
Uncoded<C,B,R,Q>
::Uncoded(const int argc, const char **argv, std::ostream &stream)
: C(argc, argv, stream)
{
	params_enc = new factory::Encoder   ::parameters();
	params_dec = new factory::Decoder_NO::parameters();

	if (this->params->enc != nullptr) { delete this->params->enc; this->params->enc = params_enc; }
	if (this->params->dec != nullptr) { delete this->params->dec; this->params->dec = params_dec; }
}

template <class C, typename B, typename R, typename Q>
Uncoded<C,B,R,Q>
::~Uncoded()
{
}

template <class C, typename B, typename R, typename Q>
void Uncoded<C,B,R,Q>
::build_args()
{
	C::build_args();
}

template <class C, typename B, typename R, typename Q>
void Uncoded<C,B,R,Q>
::store_args()
{
	C::store_args();

	params_enc->type = "NO";
	params_enc->K    = this->params->src->K;
	params_enc->N_cw = params_enc->K;
	params_enc->R    = 1.f;

	params_dec->type   = "NONE";
	params_dec->implem = "HARD_DECISION";
	params_dec->K      = params_enc->K;
	params_dec->N_cw   = params_enc->N_cw;
	params_dec->R      = 1.f;

	this->params->pct->type = "NO";
	this->params->pct->K    = params_enc->K;
	this->params->pct->N    = this->params->pct->K;
	this->params->pct->N_cw = this->params->pct->N;
	this->params->pct->R    = 1.f;
}

template <class C, typename B, typename R, typename Q>
void Uncoded<C,B,R,Q>
::print_header()
{
	if (params_enc->type != "NO")
		factory::Encoder::make_header(this->pl_enc, *params_enc);
	factory::Decoder_NO::make_header(this->pl_dec, *params_dec);

	C::print_header();
}

template <class C, typename B, typename R, typename Q>
void Uncoded<C,B,R,Q>
::build_codec()
{
	this->codec = new tools::Codec_uncoded<B,Q>(*params_enc, *params_dec);
}
}
}
