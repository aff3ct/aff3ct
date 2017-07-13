#include <iostream>

#include "Tools/Codec/Polar/Codec_polar.hpp"
#include "Launcher_polar.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B, typename R, typename Q>
Launcher_polar<C,B,R,Q>
::Launcher_polar(const int argc, const char **argv, std::ostream &stream)
: C(argc, argv, stream)
{
	m_enc = new tools::Factory_encoder_polar  ::parameters();
	m_pct = new tools::Factory_puncturer_polar::parameters();
	m_dec = new tools::Factory_decoder_polar  ::parameters();

	this->m_chain_params->enc = m_enc;
	this->m_chain_params->pct = m_pct;
	this->m_chain_params->dec = m_dec;

	//	this->params.quantizer .n_bits        = 6;
	//	this->params.quantizer .n_decimals    = 1;
}

template <class C, typename B, typename R, typename Q>
Launcher_polar<C,B,R,Q>
::~Launcher_polar()
{
	if (this->m_chain_params->pct != nullptr)
		delete this->m_chain_params->pct;

	if (this->m_chain_params->enc != nullptr)
		delete this->m_chain_params->enc;

	if (this->m_chain_params->dec != nullptr)
		delete this->m_chain_params->dec;
}

template <class C, typename B, typename R, typename Q>
void Launcher_polar<C,B,R,Q>
::build_args()
{
	C::build_args();

	tools::Factory_encoder_polar  ::build_args(this->req_args, this->opt_args);
	tools::Factory_puncturer_polar::build_args(this->req_args, this->opt_args);
	tools::Factory_decoder_polar  ::build_args(this->req_args, this->opt_args);
}

template <class C, typename B, typename R, typename Q>
void Launcher_polar<C,B,R,Q>
::store_args()
{
	C::store_args();

	m_enc->K        = this->m_chain_params->sim->K;
	m_enc->N_cw     = this->m_chain_params->sim->N;
	m_enc->n_frames = this->m_chain_params->sim->inter_frame_level;

	tools::Factory_encoder_polar  ::store_args(this->ar, *m_enc);
	tools::Factory_decoder_polar  ::store_args(this->ar, *m_dec, this->m_chain_params->sim->K, this->m_chain_params->sim->N, this->m_chain_params->sim->inter_frame_level);

	m_pct->K = this->m_chain_params->sim->K;
	m_pct->N = this->m_chain_params->sim->N;
	m_pct->n_frames = this->m_chain_params->sim->inter_frame_level;

	tools::Factory_puncturer_polar::store_args(this->ar, *m_pct);
}

template <class C, typename B, typename R, typename Q>
void Launcher_polar<C,B,R,Q>
::group_args()
{
	C::group_args();

	tools::Factory_encoder_polar  ::group_args(this->arg_group);
	tools::Factory_puncturer_polar::group_args(this->arg_group);
	tools::Factory_decoder_polar  ::group_args(this->arg_group);
}

template <class C, typename B, typename R, typename Q>
void Launcher_polar<C,B,R,Q>
::print_header()
{
	tools::Factory_encoder_polar  ::header(this->pl_enc, *m_enc);
	tools::Factory_puncturer_polar::header(this->pl_pct, *m_pct);
	tools::Factory_decoder_polar  ::header(this->pl_dec, this->pl_cde, *m_dec);

	C::print_header();
}

template <class C, typename B, typename R, typename Q>
void Launcher_polar<C,B,R,Q>
::build_codec()
{
	this->codec = new tools::Codec_polar<B,Q>(*m_enc, *m_dec, *m_pct, this->m_chain_params->sim->n_threads);
}
}
}
