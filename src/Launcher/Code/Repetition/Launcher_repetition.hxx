#include <iostream>

#include "Tools/Codec/Repetition/Codec_repetition.hpp"
#include "Launcher_repetition.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B, typename R, typename Q>
Launcher_repetition<C,B,R,Q>
::Launcher_repetition(const int argc, const char **argv, std::ostream &stream)
: C(argc, argv, stream)
{
	params_enc = new tools::Factory_encoder_repetition::parameters();
	params_dec = new tools::Factory_decoder_repetition::parameters();

	if (this->params->enc != nullptr) { delete this->params->enc; this->params->enc = params_enc; }
	if (this->params->dec != nullptr) { delete this->params->dec; this->params->dec = params_dec; }
}

template <class C, typename B, typename R, typename Q>
Launcher_repetition<C,B,R,Q>
::~Launcher_repetition()
{
}

template <class C, typename B, typename R, typename Q>
void Launcher_repetition<C,B,R,Q>
::build_args()
{
	tools::Factory_encoder_repetition::build_args(this->req_args, this->opt_args);
	tools::Factory_decoder_repetition::build_args(this->req_args, this->opt_args);

	C::build_args();
}

template <class C, typename B, typename R, typename Q>
void Launcher_repetition<C,B,R,Q>
::store_args()
{
	tools::Factory_encoder_repetition::store_args(this->ar, *params_enc);
	tools::Factory_decoder_repetition::store_args(this->ar, *params_dec);

	C::store_args();
}

template <class C, typename B, typename R, typename Q>
void Launcher_repetition<C,B,R,Q>
::group_args()
{
	tools::Factory_encoder_repetition::group_args(this->arg_group);
	tools::Factory_decoder_repetition::group_args(this->arg_group);

	C::group_args();
}

template <class C, typename B, typename R, typename Q>
void Launcher_repetition<C,B,R,Q>
::print_header()
{
	tools::Factory_encoder_repetition::header(this->pl_enc, *params_enc);
	tools::Factory_decoder_repetition::header(this->pl_dec, *params_dec);

	C::print_header();
}

template <class C, typename B, typename R, typename Q>
void Launcher_repetition<C,B,R,Q>
::build_codec()
{
	this->codec = new tools::Codec_repetition<B,Q>(*params_enc, *params_dec);
}
}
}
