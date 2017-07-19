#include <iostream>

#include "Tools/Codec/Turbo/Codec_turbo.hpp"
#include "Launcher_turbo.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B, typename R, typename Q, typename QD>
Launcher_turbo<C,B,R,Q,QD>
::Launcher_turbo(const int argc, const char **argv, std::ostream &stream)
: C(argc, argv, stream)
{
	params_enc = new tools::Factory_encoder_turbo  ::parameters();
	params_pct = new tools::Factory_puncturer_turbo::parameters();
	params_dec = new tools::Factory_decoder_turbo  ::parameters();

	if (this->params->enc != nullptr) { delete this->params->enc; this->params->enc = params_enc; }
	if (this->params->pct != nullptr) { delete this->params->pct; this->params->pct = params_pct; }
	if (this->params->dec != nullptr) { delete this->params->dec; this->params->dec = params_dec; }
}

template <class C, typename B, typename R, typename Q, typename QD>
Launcher_turbo<C,B,R,Q,QD>
::~Launcher_turbo()
{
}

template <class C, typename B, typename R, typename Q, typename QD>
void Launcher_turbo<C,B,R,Q,QD>
::build_args()
{
	tools::Factory_encoder_turbo  ::build_args(this->req_args, this->opt_args);
	tools::Factory_puncturer_turbo::build_args(this->req_args, this->opt_args);
	tools::Factory_decoder_turbo  ::build_args(this->req_args, this->opt_args);

	this->opt_args.erase({"enc-fra",       "F"});
	this->opt_args.erase({"enc-seed",      "S"});
	this->req_args.erase({"dec-cw-size",   "N"});
	this->req_args.erase({"dec-info-bits", "K"});
	this->opt_args.erase({"dec-fra",       "F"});
	this->opt_args.erase({"dec-no-buff"       });
	this->opt_args.erase({"dec-poly"          });
	this->opt_args.erase({"dec-std"           });
	this->opt_args.erase({"itl-seed",      "S"});
	this->req_args.erase({"pct-info-bits", "K"});
	this->opt_args.erase({"pct-no-buff"       });
	this->opt_args.erase({"pct-fra",       "F"});
	this->opt_args.erase({"pct-tail-length"   });

	C::build_args();
}

template <class C, typename B, typename R, typename Q, typename QD>
void Launcher_turbo<C,B,R,Q,QD>
::store_args()
{
	tools::Factory_encoder_turbo::store_args(this->ar, *params_enc);

	params_pct->K           = params_enc->K;
	params_pct->N_cw        = params_enc->N_cw;
	params_pct->buffered    = params_enc->buffered;
	params_pct->tail_length = params_enc->tail_length;

	tools::Factory_puncturer_turbo::store_args(this->ar, *params_pct);

	params_dec->K        = params_enc->K;
	params_dec->N_cw     = params_enc->N_cw;
	params_dec->buffered = params_enc->buffered;
	params_dec->poly     = params_enc->poly;
	params_dec->standard = params_enc->standard;

	tools::Factory_decoder_turbo::store_args(this->ar, *params_dec);

	if (params_dec->simd_strategy == "INTER")
		this->params->src->n_frames = mipp::N<Q>();
	if (params_dec->simd_strategy == "INTRA")
		this->params->src->n_frames = (int)std::ceil(mipp::N<Q>() / 8.f);

	C::store_args();
}

template <class C, typename B, typename R, typename Q, typename QD>
void Launcher_turbo<C,B,R,Q,QD>
::group_args()
{
	tools::Factory_encoder_turbo  ::group_args(this->arg_group);
	tools::Factory_puncturer_turbo::group_args(this->arg_group);
	tools::Factory_decoder_turbo  ::group_args(this->arg_group);

	C::group_args();
}

template <class C, typename B, typename R, typename Q, typename QD>
void Launcher_turbo<C,B,R,Q,QD>
::print_header()
{
	if (params_enc->type != "NO")
		tools::Factory_encoder_turbo::header(this->pl_enc, this->pl_itl, *params_enc);
	if (params_pct->type != "NO")
		tools::Factory_puncturer_turbo::header(this->pl_pct, *params_pct);
	tools::Factory_decoder_turbo::header(this->pl_dec, *params_dec);

	C::print_header();
}

template <class C, typename B, typename R, typename Q, typename QD>
void Launcher_turbo<C,B,R,Q,QD>
::build_codec()
{
	this->codec = new tools::Codec_turbo<B,Q,QD>(*params_enc, *params_dec, *params_pct, this->params->n_threads);
}
}
}
