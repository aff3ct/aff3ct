#include <iostream>
#include <mipp.h>

#include "Tools/Codec/Polar/Codec_polar.hpp"

#include "Polar.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B, typename R, typename Q>
Polar<C,B,R,Q>
::Polar(const int argc, const char **argv, std::ostream &stream)
: C(argc, argv, stream)
{
	params_fb  = new factory::Frozenbits_generator::parameters();
	params_pct = new factory::Puncturer_polar     ::parameters();
	params_enc = new factory::Encoder_polar       ::parameters();
	params_dec = new factory::Decoder_polar       ::parameters();

	if (this->params->enc != nullptr) { delete this->params->enc; this->params->enc = params_enc; }
	if (this->params->pct != nullptr) { delete this->params->pct; this->params->pct = params_pct; }
	if (this->params->dec != nullptr) { delete this->params->dec; this->params->dec = params_dec; }
}

template <class C, typename B, typename R, typename Q>
Polar<C,B,R,Q>
::~Polar()
{
	if (params_fb != nullptr)
	{
		delete params_fb;
		params_fb = nullptr;
	}
}

template <class C, typename B, typename R, typename Q>
void Polar<C,B,R,Q>
::build_args()
{
	factory::Frozenbits_generator::build_args(this->req_args, this->opt_args, "enc-fb");
	factory::Encoder_polar       ::build_args(this->req_args, this->opt_args);
	factory::Puncturer_polar     ::build_args(this->req_args, this->opt_args);
	factory::Decoder_polar       ::build_args(this->req_args, this->opt_args);

	this->opt_args.erase({"pct-fra",          "F"});
	this->req_args.erase({"enc-cw-size",      "N"});
	this->req_args.erase({"enc-info-bits",    "K"});
	this->opt_args.erase({"enc-fra",          "F"});
	this->opt_args.erase({"enc-seed",         "S"});
	this->req_args.erase({"dec-cw-size",      "N"});
	this->req_args.erase({"dec-info-bits",    "K"});
	this->opt_args.erase({"dec-fra",          "F"});
	this->opt_args.erase({"dec-no-sys"           });
	this->req_args.erase({"enc-fb-cw-size",   "N"});
	this->req_args.erase({"enc-fb-info-bits", "K"});

	C::build_args();
}

template <class C, typename B, typename R, typename Q>
void Polar<C,B,R,Q>
::store_args()
{
	factory::Puncturer_polar::store_args(this->ar.get_args(), *params_pct);

	params_fb->K    = params_pct->K;
	params_fb->N_cw = params_pct->N_cw;

	factory::Frozenbits_generator::store_args(this->ar.get_args(), *params_fb, "enc-fb");

	params_enc->K    = params_pct->K;
	params_enc->N_cw = params_pct->N_cw;

	factory::Encoder_polar::store_args(this->ar.get_args(), *params_enc);

	params_dec->K          = params_pct->K;
	params_dec->N_cw       = params_pct->N_cw;
	params_dec->systematic = params_enc->systematic;

	factory::Decoder_polar::store_args(this->ar.get_args(), *params_dec);

	if (params_dec->simd_strategy == "INTER")
		this->params->src->n_frames = mipp::N<Q>();

	if (std::is_same<Q,int8_t>() || std::is_same<Q,int16_t>())
	{
		this->params->qnt->n_bits     = 6;
		this->params->qnt->n_decimals = 1;
	}

	C::store_args();
}

template <class C, typename B, typename R, typename Q>
void Polar<C,B,R,Q>
::print_header()
{
	if (params_enc->type != "NO")
		factory::Encoder_polar::make_header(this->pl_enc, *params_enc, false);
	if (params_pct->type != "NO")
		factory::Puncturer_polar::make_header(this->pl_pct, *params_pct, false);
	factory::Frozenbits_generator::make_header(this->pl_enc, *params_fb, false);
	factory::Decoder_polar::make_header(this->pl_dec, *params_dec, false);

	C::print_header();
}

template <class C, typename B, typename R, typename Q>
void Polar<C,B,R,Q>
::build_codec()
{
	this->codec = new tools::Codec_polar<B,Q>(*params_fb, *params_pct, *params_enc, *params_dec, this->params->n_threads);
}
}
}
