#include <iostream>

#include "Tools/Codec/RSC_DB/Codec_RSC_DB.hpp"

#include "RSC_DB.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B, typename R, typename Q>
RSC_DB<C,B,R,Q>
::RSC_DB(const int argc, const char **argv, std::ostream &stream)
: C(argc, argv, stream)
{
	params_enc = new factory::Encoder_RSC_DB::parameters();
	params_dec = new factory::Decoder_RSC_DB::parameters();

	if (this->params->enc != nullptr) { delete this->params->enc; this->params->enc = params_enc; }
	if (this->params->dec != nullptr) { delete this->params->dec; this->params->dec = params_dec; }
}

template <class C, typename B, typename R, typename Q>
RSC_DB<C,B,R,Q>
::~RSC_DB()
{
}

template <class C, typename B, typename R, typename Q>
void RSC_DB<C,B,R,Q>
::build_args()
{
	factory::Encoder_RSC_DB::build_args(this->req_args, this->opt_args);
	factory::Decoder_RSC_DB::build_args(this->req_args, this->opt_args);

	this->opt_args.erase({"enc-fra",       "F"});
	this->opt_args.erase({"enc-seed",      "S"});
	this->req_args.erase({"dec-cw-size",   "N"});
	this->req_args.erase({"dec-info-bits", "K"});
	this->opt_args.erase({"dec-fra",       "F"});
	this->opt_args.erase({"dec-no-buff"       });

	C::build_args();
}

template <class C, typename B, typename R, typename Q>
void RSC_DB<C,B,R,Q>
::store_args()
{
	factory::Encoder_RSC_DB::store_args(this->ar.get_args(), *params_enc);

	params_dec->K        = params_enc->K;
	params_dec->N_cw     = params_enc->N_cw;
	params_dec->buffered = params_enc->buffered;

	factory::Decoder_RSC_DB::store_args(this->ar.get_args(), *params_dec);

	if (!params_enc->standard.empty() && !this->ar.exist_arg({"dec-implem"}))
		params_dec->implem = params_enc->standard;

	this->params->pct->type = "NO";
	this->params->pct->K    = params_enc->K;
	this->params->pct->N    = params_enc->N_cw;
	this->params->pct->N_cw = this->params->pct->N;

	if (std::is_same<Q,int8_t>() || std::is_same<Q,int16_t>())
	{
		this->params->qnt->n_bits     = 6;
		this->params->qnt->n_decimals = 3;
	}

	C::store_args();
}

template <class C, typename B, typename R, typename Q>
void RSC_DB<C,B,R,Q>
::print_header()
{
	if (params_enc->type != "NO")
		factory::Encoder_RSC_DB::make_header(this->pl_enc, *params_enc, false);
	factory::Decoder_RSC_DB::make_header(this->pl_dec, *params_dec, false);

	C::print_header();
}

template <class C, typename B, typename R, typename Q>
void RSC_DB<C,B,R,Q>
::build_codec()
{
	this->codec = new tools::Codec_RSC_DB<B,Q>(*params_enc, *params_dec);
}
}
}
