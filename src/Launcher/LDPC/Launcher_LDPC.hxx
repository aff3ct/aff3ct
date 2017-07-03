#include <iostream>

#include "Tools/Codec/LDPC/Codec_LDPC.hpp"
#include "Launcher_LDPC.hpp"

namespace aff3ct
{
namespace launcher
{
template <class cLauncher, typename B, typename R, typename Q>
Launcher_LDPC<cLauncher,B,R,Q>
::Launcher_LDPC(const int argc, const char **argv, std::ostream &stream)
: cLauncher(argc, argv, stream)
{
	m_enc = new typename tools::Factory_encoder_LDPC<B  >::encoder_parameters    ();
	m_dec = new typename tools::Factory_decoder_LDPC<B,Q>::decoder_parameters_LDPC();

	this->m_chain_params->enc = m_enc;
	this->m_chain_params->dec = m_dec;
}

template <class cLauncher, typename B, typename R, typename Q>
Launcher_LDPC<cLauncher,B,R,Q>
::~Launcher_LDPC()
{
	if (this->m_chain_params->enc != nullptr)
		delete this->m_chain_params->enc;

	if (this->m_chain_params->dec != nullptr)
		delete this->m_chain_params->dec;
}

template <class cLauncher, typename B, typename R, typename Q>
void Launcher_LDPC<cLauncher,B,R,Q>
::build_args()
{
	cLauncher::build_args();

	tools::Factory_encoder_LDPC<B  >::build_args(this->req_args, this->opt_args);
	tools::Factory_decoder_LDPC<B,Q>::build_args(this->req_args, this->opt_args);
}

template <class cLauncher, typename B, typename R, typename Q>
void Launcher_LDPC<cLauncher,B,R,Q>
::store_args()
{
	cLauncher::store_args();

	tools::Factory_encoder_LDPC<B  >::store_args(this->ar, *m_enc, this->m_chain_params->sim.K, this->m_chain_params->sim.N, this->m_chain_params->sim.inter_frame_level);
	tools::Factory_decoder_LDPC<B,Q>::store_args(this->ar, *m_dec, this->m_chain_params->sim.K, this->m_chain_params->sim.N, this->m_chain_params->sim.inter_frame_level);
}

template <class cLauncher, typename B, typename R, typename Q>
void Launcher_LDPC<cLauncher,B,R,Q>
::group_args()
{
	cLauncher::group_args();

	tools::Factory_encoder_LDPC<B  >::group_args(this->arg_group);
	tools::Factory_decoder_LDPC<B,Q>::group_args(this->arg_group);
}

template <class cLauncher, typename B, typename R, typename Q>
void Launcher_LDPC<cLauncher,B,R,Q>
::print_header()
{
	tools::Factory_encoder_LDPC<B  >::header(this->pl_enc, *m_enc);
	tools::Factory_decoder_LDPC<B,Q>::header(this->pl_dec, this->pl_cde, *m_dec);

	cLauncher::print_header();
}

template <class cLauncher, typename B, typename R, typename Q>
void Launcher_LDPC<cLauncher,B,R,Q>
::build_codec()
{
	this->codec = new tools::Codec_LDPC<B,Q>(*m_enc, *m_dec, this->m_chain_params->sim.n_threads);
}
}
}
