#include <iostream>

#include "Tools/Codec/BCH/Codec_BCH.hpp"
#include "Launcher_BCH.hpp"

namespace aff3ct
{
namespace launcher
{
template <class cLauncher, typename B, typename R, typename Q>
Launcher_BCH<cLauncher,B,R,Q>
::Launcher_BCH(const int argc, const char **argv, std::ostream &stream)
: cLauncher(argc, argv, stream)
{
	m_enc = new typename tools::Factory_encoder_BCH<B  >::encoder_parameters    ();
	m_dec = new typename tools::Factory_decoder_BCH<B,Q>::decoder_parameters_BCH();

	this->m_chain_params->enc = m_enc;
	this->m_chain_params->dec = m_dec;

//	this->params.quantizer.n_bits     = 7;
//	this->params.quantizer.n_decimals = 2;
}

template <class cLauncher, typename B, typename R, typename Q>
Launcher_BCH<cLauncher,B,R,Q>
::~Launcher_BCH()
{
	if (this->m_chain_params->enc != nullptr)
		delete this->m_chain_params->enc;

	if (this->m_chain_params->dec != nullptr)
		delete this->m_chain_params->dec;
}

template <class cLauncher, typename B, typename R, typename Q>
void Launcher_BCH<cLauncher,B,R,Q>
::build_args()
{
	cLauncher::build_args();

	tools::Factory_encoder_BCH<B  >::build_args(this->req_args, this->opt_args);
	tools::Factory_decoder_BCH<B,Q>::build_args(this->req_args, this->opt_args);
}

template <class cLauncher, typename B, typename R, typename Q>
void Launcher_BCH<cLauncher,B,R,Q>
::store_args()
{
	cLauncher::store_args();

	tools::Factory_encoder_BCH<B  >::store_args(this->ar, *m_enc, this->m_chain_params->sim.K, this->m_chain_params->sim.N, this->m_chain_params->sim.inter_frame_level);
	tools::Factory_decoder_BCH<B,Q>::store_args(this->ar, *m_dec, this->m_chain_params->sim.K, this->m_chain_params->sim.N, this->m_chain_params->sim.inter_frame_level);
}

template <class cLauncher, typename B, typename R, typename Q>
void Launcher_BCH<cLauncher,B,R,Q>
::group_args()
{
	cLauncher::group_args();

	tools::Factory_encoder_BCH<B  >::group_args(this->arg_group);
	tools::Factory_decoder_BCH<B,Q>::group_args(this->arg_group);
}

template <class cLauncher, typename B, typename R, typename Q>
void Launcher_BCH<cLauncher,B,R,Q>
::print_header()
{
	tools::Factory_encoder_BCH<B  >::header(this->pl_enc, *m_enc);
	tools::Factory_decoder_BCH<B,Q>::header(this->pl_dec, this->pl_cde, *m_dec);

	cLauncher::print_header();
}

template <class cLauncher, typename B, typename R, typename Q>
void Launcher_BCH<cLauncher,B,R,Q>
::build_codec()
{
	this->codec = new tools::Codec_BCH<B,Q>(this->params);
}
}
}
