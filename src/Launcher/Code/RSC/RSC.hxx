#include <iostream>

#include "Tools/Codec/RSC/Codec_RSC.hpp"

#include "RSC.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B, typename R, typename Q, typename QD>
RSC<C,B,R,Q,QD>
::RSC(const int argc, const char **argv, std::ostream &stream)
: C(argc, argv, stream)
{
	params_enc = new factory::Encoder_RSC::parameters();
	params_dec = new factory::Decoder_RSC::parameters();

	if (this->params->enc != nullptr) { delete this->params->enc; this->params->enc = params_enc; }
	if (this->params->dec != nullptr) { delete this->params->dec; this->params->dec = params_dec; }
}

template <class C, typename B, typename R, typename Q, typename QD>
RSC<C,B,R,Q,QD>
::~RSC()
{
}

template <class C, typename B, typename R, typename Q, typename QD>
void RSC<C,B,R,Q,QD>
::build_args()
{
	factory::Encoder_RSC::build_args(this->req_args, this->opt_args);
	factory::Decoder_RSC::build_args(this->req_args, this->opt_args);

	this->opt_args.erase({"enc-fra",       "F"});
	this->opt_args.erase({"enc-seed",      "S"});
	this->req_args.erase({"dec-cw-size",   "N"});
	this->req_args.erase({"dec-info-bits", "K"});
	this->opt_args.erase({"dec-fra",       "F"});
	this->opt_args.erase({"dec-no-buff"       });
	this->opt_args.erase({"dec-poly"          });
	this->opt_args.erase({"dec-std"           });

	C::build_args();
}

template <class C, typename B, typename R, typename Q, typename QD>
void RSC<C,B,R,Q,QD>
::store_args()
{
	factory::Encoder_RSC::store_args(this->ar.get_args(), *params_enc);

	params_dec->K        = params_enc->K;
	params_dec->N_cw     = params_enc->N_cw;
	params_dec->buffered = params_enc->buffered;
	params_dec->poly     = params_enc->poly;
	params_dec->standard = params_enc->standard;

	factory::Decoder_RSC::store_args(this->ar.get_args(), *params_dec);

	this->params->pct->type = "NO";
	this->params->pct->K    = params_enc->K;
	this->params->pct->N    = params_enc->N_cw;
	this->params->pct->N_cw = this->params->pct->N;
	this->params->pct->R    = (float)this->params->pct->K / (float)this->params->pct->N;

	if (params_dec->simd_strategy == "INTER")
		this->params->src->n_frames = mipp::N<Q>();
	if (params_dec->simd_strategy == "INTRA")
		this->params->src->n_frames = (int)std::ceil(mipp::N<Q>() / 8.f);

	if (std::is_same<Q,int8_t>() || std::is_same<Q,int16_t>())
	{
		this->params->qnt->n_bits     = 6;
		this->params->qnt->n_decimals = 3;
	}

	C::store_args();
}

template <class C, typename B, typename R, typename Q, typename QD>
void RSC<C,B,R,Q,QD>
::print_header()
{
	if (params_enc->type != "NO")
		factory::Encoder_RSC::make_header(this->pl_enc, *params_enc);
	factory::Decoder_RSC::make_header(this->pl_dec, *params_dec);

	C::print_header();
}

template <class C, typename B, typename R, typename Q, typename QD>
void RSC<C,B,R,Q,QD>
::build_codec()
{
	this->codec = new tools::Codec_RSC<B,Q,QD>(*params_enc, *params_dec);
}
}
}
