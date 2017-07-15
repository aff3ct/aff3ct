#include <iostream>

#include "Tools/Codec/Uncoded/Codec_uncoded.hpp"
#include "Launcher_NO.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B, typename R, typename Q>
Launcher_NO<C,B,R,Q>
::Launcher_NO(const int argc, const char **argv, std::ostream &stream)
: C(argc, argv, stream)
{
	m_enc = new tools::Factory_encoder   ::parameters();
	m_dec = new tools::Factory_decoder_NO::parameters();

	this->m_chain_params->enc = m_enc;
	this->m_chain_params->dec = m_dec;

//	this->params.quantizer.n_bits     = 6;
//	this->params.quantizer.n_decimals = 2;
}

template <class C, typename B, typename R, typename Q>
Launcher_NO<C,B,R,Q>
::~Launcher_NO()
{
	if (this->m_chain_params->enc != nullptr)
		delete this->m_chain_params->enc;

	if (this->m_chain_params->dec != nullptr)
		delete this->m_chain_params->dec;
}

template <class C, typename B, typename R, typename Q>
void Launcher_NO<C,B,R,Q>
::build_args()
{
	C::build_args();

	tools::Factory_encoder   ::build_args(this->req_args, this->opt_args);
	tools::Factory_decoder_NO::build_args(this->req_args, this->opt_args);
}

template <class C, typename B, typename R, typename Q>
void Launcher_NO<C,B,R,Q>
::store_args()
{
	C::store_args();

	m_enc->K        = this->m_chain_params->sim->K;
	m_enc->N_cw     = this->m_chain_params->sim->N;
	m_enc->n_frames = this->m_chain_params->sim->inter_frame_level;

	tools::Factory_encoder   ::store_args(this->ar, *m_enc);

	m_dec->K        = this->m_chain_params->sim->K;
	m_dec->N_cw     = this->m_chain_params->sim->N;
	m_dec->n_frames = this->m_chain_params->sim->inter_frame_level;

	tools::Factory_decoder_NO::store_args(this->ar, *m_dec);
}

template <class C, typename B, typename R, typename Q>
void Launcher_NO<C,B,R,Q>
::group_args()
{
	C::group_args();

	tools::Factory_encoder   ::group_args(this->arg_group);
	tools::Factory_decoder_NO::group_args(this->arg_group);
}

template <class C, typename B, typename R, typename Q>
void Launcher_NO<C,B,R,Q>
::print_header()
{
	tools::Factory_encoder   ::header(this->pl_enc, *m_enc);
	tools::Factory_decoder_NO::header(this->pl_dec, *m_dec);

	C::print_header();
}

template <class C, typename B, typename R, typename Q>
void Launcher_NO<C,B,R,Q>
::build_codec()
{
	this->codec = new tools::Codec_uncoded<B,Q>(*m_enc, *m_dec);
}
}
}
