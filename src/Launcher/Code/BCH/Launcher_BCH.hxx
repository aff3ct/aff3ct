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

//	this->params.quantizer.n_bits     = 7;
//	this->params.quantizer.n_decimals = 2;
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
	C::build_args();

	tools::Factory_encoder_BCH::build_args(this->req_args, this->opt_args);
	tools::Factory_decoder_BCH::build_args(this->req_args, this->opt_args);
}

template <class C, typename B, typename R, typename Q>
void Launcher_BCH<C,B,R,Q>
::store_args()
{
	C::store_args();

	tools::Factory_encoder_BCH::store_args(this->ar, *m_enc, this->m_chain_params->sim->K, this->m_chain_params->sim->N, this->m_chain_params->sim->inter_frame_level);
	tools::Factory_decoder_BCH::store_args(this->ar, *m_dec, this->m_chain_params->sim->K, this->m_chain_params->sim->N, this->m_chain_params->sim->inter_frame_level);
}

template <class C, typename B, typename R, typename Q>
void Launcher_BCH<C,B,R,Q>
::group_args()
{
	C::group_args();

	tools::Factory_encoder_BCH::group_args(this->arg_group);
	tools::Factory_decoder_BCH::group_args(this->arg_group);
}

template <class C, typename B, typename R, typename Q>
void Launcher_BCH<C,B,R,Q>
::print_header()
{
	tools::Factory_encoder_BCH::header(this->pl_enc, this->pl_cde, *m_enc);
	tools::Factory_decoder_BCH::header(this->pl_dec, this->pl_cde, *m_dec);

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
