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
	m_enc = new typename tools::Factory_encoder_RSC<B     >::encoder_parameters_RSC();
	m_dec = new typename tools::Factory_decoder_RSC<B,Q,QD>::decoder_parameters_RSC();

	this->m_chain_params->enc = m_enc;
	this->m_chain_params->dec = m_dec;

//	this->params.code     .tail_length   = 2*3;
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

	tools::Factory_encoder_RSC<B     >::build_args(this->req_args, this->opt_args);
	tools::Factory_decoder_RSC<B,Q,QD>::build_args(this->req_args, this->opt_args);
}

template <class cLauncher, typename B, typename R, typename Q, typename QD>
void Launcher_RSC<cLauncher,B,R,Q,QD>
::store_args()
{
	cLauncher::store_args();

	tools::Factory_encoder_RSC<B     >::store_args(this->ar, *m_enc, this->m_chain_params->sim.K, this->m_chain_params->sim.N, this->m_chain_params->sim.inter_frame_level);
	tools::Factory_decoder_RSC<B,Q,QD>::store_args(this->ar, *m_dec, this->m_chain_params->sim.K, this->m_chain_params->sim.N, this->m_chain_params->sim.inter_frame_level);
}

template <class cLauncher, typename B, typename R, typename Q, typename QD>
void Launcher_RSC<cLauncher,B,R,Q,QD>
::group_args()
{
	cLauncher::group_args();

	tools::Factory_encoder_RSC<B     >::group_args(this->arg_group);
	tools::Factory_decoder_RSC<B,Q,QD>::group_args(this->arg_group);
}

template <class cLauncher, typename B, typename R, typename Q, typename QD>
void Launcher_RSC<cLauncher,B,R,Q,QD>
::print_header()
{
	tools::Factory_encoder_RSC<B     >::header(this->pl_enc, *m_enc);
	tools::Factory_decoder_RSC<B,Q,QD>::header(this->pl_dec, this->pl_cde, *m_dec);

	cLauncher::print_header();
}

template <class cLauncher, typename B, typename R, typename Q, typename QD>
void Launcher_RSC<cLauncher,B,R,Q,QD>
::build_codec()
{
	this->codec = new tools::Codec_RSC<B,Q,QD>(this->params);
}
}
}
