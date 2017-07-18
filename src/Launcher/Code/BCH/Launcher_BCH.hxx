#include <iostream>

#include "Tools/Codec/BCH/Codec_BCH.hpp"
#include "Launcher_BCH.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B, typename R, typename Q>
Launcher_BCH<C,B,R,Q>
::Launcher_BCH(const int argc, const char **argv, std::ostream &stream)
: C(argc, argv, stream)
{
	m_enc = new tools::Factory_encoder_BCH::parameters();
	m_dec = new tools::Factory_decoder_BCH::parameters();

	this->m_chain_params->enc = m_enc;
	this->m_chain_params->dec = m_dec;
}

template <class C, typename B, typename R, typename Q>
Launcher_BCH<C,B,R,Q>
::~Launcher_BCH()
{
	if (this->m_chain_params->enc != nullptr)
		delete this->m_chain_params->enc;

	if (this->m_chain_params->dec != nullptr)
		delete this->m_chain_params->dec;
}

template <class C, typename B, typename R, typename Q>
void Launcher_BCH<C,B,R,Q>
::build_args()
{
	tools::Factory_encoder_BCH::build_args(this->req_args, this->opt_args);
	tools::Factory_decoder_BCH::build_args(this->req_args, this->opt_args);

	C::build_args();
}

template <class C, typename B, typename R, typename Q>
void Launcher_BCH<C,B,R,Q>
::store_args()
{
	tools::Factory_encoder_BCH::store_args(this->ar, *m_enc);

	m_dec->K    = m_enc->K;
	m_dec->N_cw = m_enc->N_cw;

	tools::Factory_decoder_BCH::store_args(this->ar, *m_dec);

	C::store_args();
}

template <class C, typename B, typename R, typename Q>
void Launcher_BCH<C,B,R,Q>
::group_args()
{
	tools::Factory_encoder_BCH::group_args(this->arg_group);
	tools::Factory_decoder_BCH::group_args(this->arg_group);

	C::group_args();
}

template <class C, typename B, typename R, typename Q>
void Launcher_BCH<C,B,R,Q>
::print_header()
{
	tools::Factory_encoder_BCH::header(this->pl_enc, *m_enc);
	tools::Factory_decoder_BCH::header(this->pl_dec, *m_dec);

	C::print_header();
}

template <class C, typename B, typename R, typename Q>
void Launcher_BCH<C,B,R,Q>
::build_codec()
{
	this->codec = new tools::Codec_BCH<B,Q>(*m_enc, *m_dec);
}
}
}
