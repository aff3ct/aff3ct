#include <iostream>

#include "Tools/Codec/Turbo/Codec_turbo.hpp"
#include "Launcher_turbo.hpp"

namespace aff3ct
{
namespace launcher
{
template <class cLauncher, typename B, typename R, typename Q, typename QD>
Launcher_turbo<cLauncher,B,R,Q,QD>
::Launcher_turbo(const int argc, const char **argv, std::ostream &stream)
: cLauncher(argc, argv, stream)
{
	m_enc = new tools::Factory_encoder_turbo  ::parameters();
	m_pct = new tools::Factory_puncturer_turbo::parameters();
	m_dec = new tools::Factory_decoder_turbo  ::parameters();

	this->m_chain_params->enc = m_enc;
	this->m_chain_params->pct = m_pct;
	this->m_chain_params->dec = m_dec;

//	this->params.quantizer  .n_bits         = 6;
//	this->params.quantizer  .n_decimals     = (typeid(Q) == typeid(short)) ? 3 : 2;
}

template <class cLauncher, typename B, typename R, typename Q, typename QD>
Launcher_turbo<cLauncher,B,R,Q,QD>
::~Launcher_turbo()
{
	if (this->m_chain_params->pct != nullptr)
		delete this->m_chain_params->pct;

	if (this->m_chain_params->enc != nullptr)
		delete this->m_chain_params->enc;

	if (this->m_chain_params->dec != nullptr)
		delete this->m_chain_params->dec;
}

template <class cLauncher, typename B, typename R, typename Q, typename QD>
void Launcher_turbo<cLauncher,B,R,Q,QD>
::build_args()
{
	cLauncher::build_args();

	tools::Factory_encoder_turbo  ::build_args(this->req_args, this->opt_args);
	tools::Factory_puncturer_turbo::build_args(this->req_args, this->opt_args);
	tools::Factory_decoder_turbo  ::build_args(this->req_args, this->opt_args);
}

template <class cLauncher, typename B, typename R, typename Q, typename QD>
void Launcher_turbo<cLauncher,B,R,Q,QD>
::store_args()
{
	cLauncher::store_args();

	tools::Factory_encoder_turbo  ::store_args(this->ar, *m_enc, this->m_chain_params->sim->K, this->m_chain_params->sim->N,               this->m_chain_params->sim->inter_frame_level);

	tools::Factory_puncturer_turbo::store_args(this->ar, *m_pct, this->m_chain_params->sim->K, this->m_chain_params->sim->N, m_enc->N_pct, this->m_chain_params->sim->inter_frame_level);

	bool activate_simd = !this->ar.exist_arg({"sim-inter-lvl"});
	bool activate_json = !m_enc->json_path.empty();
	tools::Factory_decoder_turbo  ::store_args(this->ar, *m_dec, this->m_chain_params->sim->K, this->m_chain_params->sim->N,               this->m_chain_params->sim->inter_frame_level, activate_simd, activate_json);

}

template <class cLauncher, typename B, typename R, typename Q, typename QD>
void Launcher_turbo<cLauncher,B,R,Q,QD>
::group_args()
{
	cLauncher::group_args();

	tools::Factory_encoder_turbo  ::group_args(this->arg_group);
	tools::Factory_puncturer_turbo::group_args(this->arg_group);
	tools::Factory_decoder_turbo  ::group_args(this->arg_group);
}

template <class cLauncher, typename B, typename R, typename Q, typename QD>
void Launcher_turbo<cLauncher,B,R,Q,QD>
::print_header()
{
	tools::Factory_encoder_turbo  ::header(this->pl_enc, this->pl_cde, this->pl_itl, *m_enc);
	tools::Factory_puncturer_turbo::header(this->pl_pct, *m_pct);
	tools::Factory_decoder_turbo  ::header(this->pl_dec, *m_dec, !this->pl_crc.empty());

	cLauncher::print_header();
}

template <class cLauncher, typename B, typename R, typename Q, typename QD>
void Launcher_turbo<cLauncher,B,R,Q,QD>
::build_codec()
{
	this->codec = new tools::Codec_turbo<B,Q,QD>(*m_enc, *m_dec, *m_pct, this->m_chain_params->sim->n_threads);
}
}
}
