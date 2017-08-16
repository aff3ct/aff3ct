#include <iostream>
#include <mipp.h>

#include "Tools/Codec/Turbo/Codec_turbo.hpp"

#include "Turbo.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B, typename R, typename Q, typename QD>
Turbo<C,B,R,Q,QD>
::Turbo(const int argc, const char **argv, std::ostream &stream)
: C(argc, argv, stream)
{
	params_enc = new factory::Encoder_turbo  ::parameters<>();
	params_pct = new factory::Puncturer_turbo::parameters  ();
	params_dec = new factory::Decoder_turbo  ::parameters<>();

	if (this->params->enc != nullptr) { delete this->params->enc; this->params->enc = params_enc; }
	if (this->params->pct != nullptr) { delete this->params->pct; this->params->pct = params_pct; }
	if (this->params->dec != nullptr) { delete this->params->dec; this->params->dec = params_dec; }
}

template <class C, typename B, typename R, typename Q, typename QD>
Turbo<C,B,R,Q,QD>
::~Turbo()
{
}

template <class C, typename B, typename R, typename Q, typename QD>
void Turbo<C,B,R,Q,QD>
::build_args()
{
	factory::Encoder_turbo  ::build_args(this->req_args, this->opt_args);
	factory::Puncturer_turbo::build_args(this->req_args, this->opt_args);
	factory::Decoder_turbo  ::build_args(this->req_args, this->opt_args);

	this->opt_args.erase({"enc-fra",       "F"});
	this->opt_args.erase({"enc-seed",      "S"});
	this->req_args.erase({"dec-cw-size",   "N"});
	this->req_args.erase({"dec-info-bits", "K"});
	this->opt_args.erase({"dec-fra",       "F"});
	this->opt_args.erase({"dec-sub-no-buff"   });
	this->opt_args.erase({"dec-sub-poly"      });
	this->opt_args.erase({"dec-sub-std"       });
	this->opt_args.erase({"dec-json"          });
	this->opt_args.erase({"itl-seed",      "S"});
	this->req_args.erase({"pct-info-bits", "K"});
	this->opt_args.erase({"pct-no-buff"       });
	this->opt_args.erase({"pct-fra",       "F"});
	this->opt_args.erase({"pct-tail-length"   });

	C::build_args();
}

template <class C, typename B, typename R, typename Q, typename QD>
void Turbo<C,B,R,Q,QD>
::store_args()
{
	factory::Encoder_turbo::store_args(this->ar.get_args(), *params_enc);

	params_pct->K           = params_enc->K;
	params_pct->N_cw        = params_enc->N_cw;
	params_pct->buffered    = params_enc->sub1.buffered;
	params_pct->tail_length = params_enc->tail_length;

	factory::Puncturer_turbo::store_args(this->ar.get_args(), *params_pct);

	params_dec->K             = params_enc->K;
	params_dec->N_cw          = params_enc->N_cw;
	params_dec->sub1.buffered = params_enc->sub1.buffered;
	params_dec->sub2.buffered = params_enc->sub2.buffered;
	params_dec->sub1.poly     = params_enc->sub1.poly;
	params_dec->sub2.poly     = params_enc->sub2.poly;
	params_dec->sub1.standard = params_enc->sub1.standard;
	params_dec->sub2.standard = params_enc->sub2.standard;
	params_dec->enable_json   = !params_enc->json_path.empty();

	factory::Decoder_turbo::store_args(this->ar.get_args(), *params_dec);

	if (params_dec->sub1.simd_strategy == "INTER")
		this->params->src->n_frames = mipp::N<Q>();
	if (params_dec->sub1.simd_strategy == "INTRA")
		this->params->src->n_frames = (int)std::ceil(mipp::N<Q>() / 8.f);

	if (std::is_same<Q,int8_t>())
	{
		this->params->qnt->n_bits     = 6;
		this->params->qnt->n_decimals = 2;
	}
	else if (std::is_same<Q,int16_t>())
	{
		this->params->qnt->n_bits     = 6;
		this->params->qnt->n_decimals = 3;
	}

	C::store_args();

	params_enc->itl.core.seed     = this->params->global_seed;
	params_dec->itl.core.seed     = this->params->global_seed;
	params_enc->itl.core.n_frames = this->params->src->n_frames;
	params_dec->itl.core.n_frames = this->params->src->n_frames;

	params_enc->sub1.n_frames = params_enc->n_frames;
	params_enc->sub2.n_frames = params_enc->n_frames;
	params_dec->sub1.n_frames = params_dec->n_frames;
	params_dec->sub2.n_frames = params_dec->n_frames;

	if (this->params->err_track_revert)
	{
		params_enc->itl.core.type = "USER";
		params_dec->itl.core.type = "USER";
		params_enc->itl.core.path = this->params->err_track_path + std::string("_$snr.itl");
		params_dec->itl.core.path = this->params->err_track_path + std::string("_$snr.itl");
	}
}

template <class C, typename B, typename R, typename Q, typename QD>
void Turbo<C,B,R,Q,QD>
::print_header()
{
	if (params_enc->type != "NO")
		factory::Encoder_turbo::make_header(this->pl_enc, this->pl_itl, *params_enc, false);
	if (params_pct->type != "NO")
		factory::Puncturer_turbo::make_header(this->pl_pct, *params_pct, false);

	factory::params_list trash;
	factory::Decoder_turbo::make_header(this->pl_dec, trash, *params_dec, false);

	C::print_header();
}

template <class C, typename B, typename R, typename Q, typename QD>
void Turbo<C,B,R,Q,QD>
::build_codec()
{
	this->codec = new tools::Codec_turbo<B,Q,QD>(*params_enc, *params_dec, *params_pct, this->params->n_threads);
}
}
}
