#include <iostream>

#include "Tools/Codec/RSC/Codec_RSC.hpp"
#include "Launcher_RSC.hpp"

namespace aff3ct
{
namespace launcher
{
template <class cLauncher, typename B, typename R, typename Q, typename QD>
Launcher_RSC<cLauncher,B,R,Q,QD>
::Launcher_RSC(const int argc, const char **argv, std::ostream &stream)
: cLauncher(argc, argv, stream)
{
	m_enc = new tools::Factory_encoder_RSC::parameters();
	m_dec = new tools::Factory_decoder_RSC::parameters();

	this->m_chain_params->enc = m_enc;
	this->m_chain_params->dec = m_dec;

//	this->params.quantizer.n_bits        = 6;
//	this->params.quantizer.n_decimals    = 3;
}

template <class cLauncher, typename B, typename R, typename Q, typename QD>
Launcher_RSC<cLauncher,B,R,Q,QD>
::~Launcher_RSC()
{
	if (this->m_chain_params->enc != nullptr)
		delete this->m_chain_params->enc;

	if (this->m_chain_params->dec != nullptr)
		delete this->m_chain_params->dec;
}

template <class cLauncher, typename B, typename R, typename Q, typename QD>
void Launcher_RSC<cLauncher,B,R,Q,QD>
::build_args()
{
	cLauncher::build_args();

	tools::Factory_encoder_RSC::build_args(this->req_args, this->opt_args);
	tools::Factory_decoder_RSC::build_args(this->req_args, this->opt_args);
}

template <class cLauncher, typename B, typename R, typename Q, typename QD>
void Launcher_RSC<cLauncher,B,R,Q,QD>
::store_args()
{
	cLauncher::store_args();

	tools::Factory_encoder_RSC::store_args(this->ar, *m_enc, this->m_chain_params->sim->K, this->m_chain_params->sim->N, this->m_chain_params->sim->inter_frame_level);

	bool activate_simd = !this->ar.exist_arg({"sim-inter-lvl"});
	tools::Factory_decoder_RSC::store_args(this->ar, *m_dec, this->m_chain_params->sim->K, this->m_chain_params->sim->N, this->m_chain_params->sim->inter_frame_level, activate_simd);
}

template <class cLauncher, typename B, typename R, typename Q, typename QD>
void Launcher_RSC<cLauncher,B,R,Q,QD>
::group_args()
{
	cLauncher::group_args();

	tools::Factory_encoder_RSC::group_args(this->arg_group);
	tools::Factory_decoder_RSC::group_args(this->arg_group);
}

template <class cLauncher, typename B, typename R, typename Q, typename QD>
void Launcher_RSC<cLauncher,B,R,Q,QD>
::print_header()
{
	tools::Factory_encoder_RSC::header(this->pl_enc, this->pl_cde, *m_enc);
	tools::Factory_decoder_RSC::header(this->pl_dec, *m_dec);

	cLauncher::print_header();
}

template <class cLauncher, typename B, typename R, typename Q, typename QD>
void Launcher_RSC<cLauncher,B,R,Q,QD>
::build_codec()
{
	this->codec = new tools::Codec_RSC<B,Q,QD>(*m_enc, *m_dec);
}
}
}
