#include <iostream>

#include "Tools/Codec/LDPC/Codec_LDPC.hpp"
#include "Launcher_LDPC.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B, typename R, typename Q>
Launcher_LDPC<C,B,R,Q>
::Launcher_LDPC(const int argc, const char **argv, std::ostream &stream)
: C(argc, argv, stream)
{
	params_enc = new tools::Factory_encoder_LDPC::parameters();
	params_dec = new tools::Factory_decoder_LDPC::parameters();

	if (this->params->enc != nullptr) { delete this->params->enc; this->params->enc = params_enc; }
	if (this->params->dec != nullptr) { delete this->params->dec; this->params->dec = params_dec; }
}

template <class C, typename B, typename R, typename Q>
Launcher_LDPC<C,B,R,Q>
::~Launcher_LDPC()
{
}

template <class C, typename B, typename R, typename Q>
void Launcher_LDPC<C,B,R,Q>
::build_args()
{
	tools::Factory_encoder_LDPC::build_args(this->req_args, this->opt_args);
	tools::Factory_decoder_LDPC::build_args(this->req_args, this->opt_args);

	this->opt_args.erase({"enc-fra",       "F"});
	this->opt_args.erase({"enc-seed",      "S"});
	this->opt_args.erase({"enc-h-path",       });
	this->req_args.erase({"dec-cw-size",   "N"});
	this->req_args.erase({"dec-info-bits", "K"});
	this->opt_args.erase({"dec-fra",       "F"});

	C::build_args();
}

template <class C, typename B, typename R, typename Q>
void Launcher_LDPC<C,B,R,Q>
::store_args()
{
	tools::Factory_encoder_LDPC::store_args(this->ar, *params_enc);

	params_dec->K    = params_enc->K;
	params_dec->N_cw = params_enc->N_cw;

	tools::Factory_decoder_LDPC::store_args(this->ar, *params_dec);

	params_enc->H_alist_path = params_dec->H_alist_path;

	this->params->pct->type = "NO";
	this->params->pct->K    = params_enc->K;
	this->params->pct->N    = params_enc->N_cw;
	this->params->pct->N_cw = this->params->pct->N;
	this->params->pct->R    = (float)this->params->pct->K / (float)this->params->pct->N;

	if (params_dec->simd_strategy == "INTER")
		this->params->src->n_frames = mipp::N<Q>();

	C::store_args();
}

template <class C, typename B, typename R, typename Q>
void Launcher_LDPC<C,B,R,Q>
::group_args()
{
	tools::Factory_encoder_LDPC::group_args(this->arg_group);
	tools::Factory_decoder_LDPC::group_args(this->arg_group);

	C::group_args();
}

template <class C, typename B, typename R, typename Q>
void Launcher_LDPC<C,B,R,Q>
::print_header()
{
	if (params_enc->type != "NO")
		tools::Factory_encoder_LDPC::header(this->pl_enc, *params_enc);
	tools::Factory_decoder_LDPC::header(this->pl_dec, *params_dec);

	C::print_header();
}

template <class C, typename B, typename R, typename Q>
void Launcher_LDPC<C,B,R,Q>
::build_codec()
{
	this->codec = new tools::Codec_LDPC<B,Q>(*params_enc, *params_dec, this->params->n_threads);
}
}
}
