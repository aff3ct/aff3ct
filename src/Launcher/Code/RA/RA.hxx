#include <iostream>

#include "Tools/Codec/RA/Codec_RA.hpp"

#include "RA.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B, typename R, typename Q>
RA<C,B,R,Q>
::RA(const int argc, const char **argv, std::ostream &stream)
: C(argc, argv, stream)
{
	params_enc = new factory::Encoder_RA::parameters();
	params_dec = new factory::Decoder_RA::parameters();

	if (this->params->enc != nullptr) { delete this->params->enc; this->params->enc = params_enc; }
	if (this->params->dec != nullptr) { delete this->params->dec; this->params->dec = params_dec; }
}

template <class C, typename B, typename R, typename Q>
RA<C,B,R,Q>
::~RA()
{
}

template <class C, typename B, typename R, typename Q>
void RA<C,B,R,Q>
::build_args()
{
	factory::Encoder_RA::build_args(this->req_args, this->opt_args);
	factory::Decoder_RA::build_args(this->req_args, this->opt_args);

	this->opt_args.erase({"enc-fra",       "F"});
	this->opt_args.erase({"enc-seed",      "S"});
	this->req_args.erase({"dec-cw-size",   "N"});
	this->req_args.erase({"dec-info-bits", "K"});
	this->opt_args.erase({"dec-fra",       "F"});
	this->opt_args.erase({"dec-implem"        });
	this->opt_args.erase({"dec-type",      "D"});
	this->opt_args.erase({"itl-seed",      "S"});

	C::build_args();
}

template <class C, typename B, typename R, typename Q>
void RA<C,B,R,Q>
::store_args()
{
	factory::Encoder_RA::store_args(this->ar.get_args(), *params_enc);

	params_dec->K    = params_enc->K;
	params_dec->N_cw = params_enc->N_cw;

	factory::Decoder_RA::store_args(this->ar.get_args(), *params_dec);

	this->params->pct->type = "NO";
	this->params->pct->K    = params_enc->K;
	this->params->pct->N    = params_enc->N_cw;
	this->params->pct->N_cw = this->params->pct->N;

	C::store_args();

	params_enc->itl.core.seed     = this->params->global_seed;
	params_dec->itl.core.seed     = this->params->global_seed;
	params_enc->itl.core.n_frames = this->params->src->n_frames;
	params_dec->itl.core.n_frames = this->params->src->n_frames;

	if (this->params->err_track_revert)
	{
		params_enc->itl.core.type = "USER";
		params_dec->itl.core.type = "USER";
		params_enc->itl.core.path = this->params->err_track_path + std::string("_$snr.itl");
		params_dec->itl.core.path = this->params->err_track_path + std::string("_$snr.itl");
	}
}

template <class C, typename B, typename R, typename Q>
void RA<C,B,R,Q>
::print_header()
{
	if (params_enc->type != "NO")
		factory::Encoder_RA::make_header(this->pl_enc, this->pl_itl, *params_enc, false);

	factory::params_list trash;
	factory::Decoder_RA::make_header(this->pl_dec, trash, *params_dec, false);

	C::print_header();
}

template <class C, typename B, typename R, typename Q>
void RA<C,B,R,Q>
::build_codec()
{
	this->codec = new tools::Codec_RA<B,Q>(*params_enc, *params_dec);
}
}
}
