#include <iostream>

#include "Tools/Codec/Polar/Codec_polar.hpp"
#include "Launcher_polar.hpp"

namespace aff3ct
{
namespace launcher
{
template <class cLauncher, typename B, typename R, typename Q>
Launcher_polar<cLauncher,B,R,Q>
::Launcher_polar(const int argc, const char **argv, std::ostream &stream)
: cLauncher(argc, argv, stream)
{
	m_enc = new typename tools::Factory_encoder_polar<B  >::encoder_parameters      ();
	m_dec = new typename tools::Factory_decoder_polar<B,Q>::decoder_parameters_polar();

	this->m_chain_params->enc = m_enc;
	this->m_chain_params->dec = m_dec;

	//	this->params.quantizer .n_bits        = 6;
	//	this->params.quantizer .n_decimals    = 1;
}

template <class cLauncher, typename B, typename R, typename Q>
Launcher_polar<cLauncher,B,R,Q>
::~Launcher_polar()
{
	if (this->m_chain_params->enc != nullptr)
		delete this->m_chain_params->enc;

	if (this->m_chain_params->dec != nullptr)
		delete this->m_chain_params->dec;
}

template <class cLauncher, typename B, typename R, typename Q>
void Launcher_polar<cLauncher,B,R,Q>
::build_args()
{
	cLauncher::build_args();

	tools::Factory_encoder_polar<B  >::build_args(this->req_args, this->opt_args);
	tools::Factory_decoder_polar<B,Q>::build_args(this->req_args, this->opt_args);
}

template <class cLauncher, typename B, typename R, typename Q>
void Launcher_polar<cLauncher,B,R,Q>
::store_args()
{
	cLauncher::store_args();

	tools::Factory_encoder_polar<B  >::store_args(this->ar, *m_enc);
	tools::Factory_decoder_polar<B,Q>::store_args(this->ar, *m_dec, this->m_chain_params->sim.N);
}

template <class cLauncher, typename B, typename R, typename Q>
void Launcher_polar<cLauncher,B,R,Q>
::group_args()
{
	cLauncher::group_args();

	tools::Factory_encoder_polar<B  >::group_args(this->arg_group);
	tools::Factory_decoder_polar<B,Q>::group_args(this->arg_group);
}

template <class cLauncher, typename B, typename R, typename Q>
void Launcher_polar<cLauncher,B,R,Q>
::print_header()
{
	tools::Factory_encoder_polar<B  >::header(this->pl_enc, *m_enc);
	tools::Factory_decoder_polar<B,Q>::header(this->pl_dec, this->pl_cde, *m_dec);

	cLauncher::print_header();
}

template <class cLauncher, typename B, typename R, typename Q>
void Launcher_polar<cLauncher,B,R,Q>
::build_codec()
{
	this->codec = new tools::Codec_polar<B,Q>(this->params);
}
}
}
