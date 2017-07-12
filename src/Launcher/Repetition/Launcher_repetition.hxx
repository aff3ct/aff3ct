#include <iostream>

#include "Tools/Codec/Repetition/Codec_repetition.hpp"
#include "Launcher_repetition.hpp"

namespace aff3ct
{
namespace launcher
{
template <class cLauncher, typename B, typename R, typename Q>
Launcher_repetition<cLauncher,B,R,Q>
::Launcher_repetition(const int argc, const char **argv, std::ostream &stream)
: cLauncher(argc, argv, stream)
{
	m_enc = new tools::Factory_encoder_repetition::parameters();
	m_dec = new tools::Factory_decoder_repetition::parameters();

	this->m_chain_params->enc = m_enc;
	this->m_chain_params->dec = m_dec;

	//	this->params.quantizer.n_bits     = 6;
	//	this->params.quantizer.n_decimals = 2;
}

template <class cLauncher, typename B, typename R, typename Q>
Launcher_repetition<cLauncher,B,R,Q>
::~Launcher_repetition()
{
	if (this->m_chain_params->enc != nullptr)
		delete this->m_chain_params->enc;

	if (this->m_chain_params->dec != nullptr)
		delete this->m_chain_params->dec;
}

template <class cLauncher, typename B, typename R, typename Q>
void Launcher_repetition<cLauncher,B,R,Q>
::build_args()
{
	cLauncher::build_args();

	tools::Factory_encoder_repetition::build_args(this->req_args, this->opt_args);
	tools::Factory_decoder_repetition::build_args(this->req_args, this->opt_args);
}

template <class cLauncher, typename B, typename R, typename Q>
void Launcher_repetition<cLauncher,B,R,Q>
::store_args()
{
	cLauncher::store_args();

	tools::Factory_encoder_repetition::store_args(this->ar, *m_enc, this->m_chain_params->sim->K, this->m_chain_params->sim->N, this->m_chain_params->sim->inter_frame_level);
	tools::Factory_decoder_repetition::store_args(this->ar, *m_dec, this->m_chain_params->sim->K, this->m_chain_params->sim->N, this->m_chain_params->sim->inter_frame_level);
}

template <class cLauncher, typename B, typename R, typename Q>
void Launcher_repetition<cLauncher,B,R,Q>
::group_args()
{
	cLauncher::group_args();

	tools::Factory_encoder_repetition::group_args(this->arg_group);
	tools::Factory_decoder_repetition::group_args(this->arg_group);
}

template <class cLauncher, typename B, typename R, typename Q>
void Launcher_repetition<cLauncher,B,R,Q>
::print_header()
{
	tools::Factory_encoder_repetition::header(this->pl_enc, this->pl_cde, *m_enc);
	tools::Factory_decoder_repetition::header(this->pl_dec, *m_dec);

	cLauncher::print_header();
}

template <class cLauncher, typename B, typename R, typename Q>
void Launcher_repetition<cLauncher,B,R,Q>
::build_codec()
{
	this->codec = new tools::Codec_repetition<B,Q>(*m_enc, *m_dec);
}
}
}
