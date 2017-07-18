#include <iostream>

#include "Tools/Codec/RSC/Codec_RSC.hpp"
#include "Launcher_RSC.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B, typename R, typename Q, typename QD>
Launcher_RSC<C,B,R,Q,QD>
::Launcher_RSC(const int argc, const char **argv, std::ostream &stream)
: C(argc, argv, stream)
{
	params_enc = new tools::Factory_encoder_RSC::parameters();
	params_dec = new tools::Factory_decoder_RSC::parameters();

	if (this->params->enc != nullptr) { delete this->params->enc; this->params->enc = params_enc; }
	if (this->params->dec != nullptr) { delete this->params->dec; this->params->dec = params_dec; }
}

template <class C, typename B, typename R, typename Q, typename QD>
Launcher_RSC<C,B,R,Q,QD>
::~Launcher_RSC()
{
}

template <class C, typename B, typename R, typename Q, typename QD>
void Launcher_RSC<C,B,R,Q,QD>
::build_args()
{
	tools::Factory_encoder_RSC::build_args(this->req_args, this->opt_args);
	tools::Factory_decoder_RSC::build_args(this->req_args, this->opt_args);

	C::build_args();
}

template <class C, typename B, typename R, typename Q, typename QD>
void Launcher_RSC<C,B,R,Q,QD>
::store_args()
{
	tools::Factory_encoder_RSC::store_args(this->ar, *params_enc);
	tools::Factory_decoder_RSC::store_args(this->ar, *params_dec);

	C::store_args();
}

template <class C, typename B, typename R, typename Q, typename QD>
void Launcher_RSC<C,B,R,Q,QD>
::group_args()
{
	tools::Factory_encoder_RSC::group_args(this->arg_group);
	tools::Factory_decoder_RSC::group_args(this->arg_group);

	C::group_args();
}

template <class C, typename B, typename R, typename Q, typename QD>
void Launcher_RSC<C,B,R,Q,QD>
::print_header()
{
	tools::Factory_encoder_RSC::header(this->pl_enc, *params_enc);
	tools::Factory_decoder_RSC::header(this->pl_dec, *params_dec);

	C::print_header();
}

template <class C, typename B, typename R, typename Q, typename QD>
void Launcher_RSC<C,B,R,Q,QD>
::build_codec()
{
	this->codec = new tools::Codec_RSC<B,Q,QD>(*params_enc, *params_dec);
}
}
}
