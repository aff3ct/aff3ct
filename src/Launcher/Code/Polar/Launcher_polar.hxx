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
	m_fb  = new tools::Factory_frozenbits_generator::parameters();
	m_pct = new tools::Factory_puncturer_polar     ::parameters();
	m_enc = new tools::Factory_encoder_polar       ::parameters();
	m_dec = new tools::Factory_decoder_polar       ::parameters();

	this->m_chain_params->enc = m_enc;
	this->m_chain_params->pct = m_pct;
	this->m_chain_params->dec = m_dec;
}

template <class C, typename B, typename R, typename Q>
Launcher_polar<C,B,R,Q>
::~Launcher_polar()
{
	if (m_fb != nullptr)
	{
		delete m_fb;
		m_fb = nullptr;
	}

	if (this->m_chain_params->pct != nullptr)
	{
		delete this->m_chain_params->pct;
		this->m_chain_params->pct = nullptr;
	}

	if (this->m_chain_params->enc != nullptr)
	{
		delete this->m_chain_params->enc;
		this->m_chain_params->enc = nullptr;
	}

	if (this->m_chain_params->dec != nullptr)
	{
		delete this->m_chain_params->dec;
		this->m_chain_params->dec = nullptr;
	}
}

template <class C, typename B, typename R, typename Q>
void Launcher_polar<C,B,R,Q>
::build_args()
{
	tools::Factory_frozenbits_generator::build_args(this->req_args, this->opt_args);
	tools::Factory_encoder_polar       ::build_args(this->req_args, this->opt_args);
	tools::Factory_puncturer_polar     ::build_args(this->req_args, this->opt_args);
	tools::Factory_decoder_polar       ::build_args(this->req_args, this->opt_args);

	this->opt_args.erase({"pct-fra",       "F"});
	this->req_args.erase({"enc-cw-size",   "N"});
	this->req_args.erase({"enc-info-bits", "K"});
	this->req_args.erase({"enc-info-bits", "K"});
	this->opt_args.erase({"enc-fra",       "F"});
	this->opt_args.erase({"enc-fb-sigma"      });
	this->opt_args.erase({"enc-seed",      "S"});
	this->req_args.erase({"dec-cw-size",   "N"});
	this->req_args.erase({"dec-info-bits", "K"});
	this->opt_args.erase({"dec-fra",       "F"});
	this->opt_args.erase({"dec-no-sys"        });

	C::build_args();
}

template <class C, typename B, typename R, typename Q>
void Launcher_polar<C,B,R,Q>
::store_args()
{
	tools::Factory_puncturer_polar::store_args(this->ar, *m_pct);

	m_fb->K    = m_pct->K;
	m_fb->N_cw = m_pct->N_cw;

	tools::Factory_frozenbits_generator::store_args(this->ar, *m_fb);

	m_enc->K    = m_pct->K;
	m_enc->N_cw = m_pct->N_cw;

	tools::Factory_encoder_polar::store_args(this->ar, *m_enc);

	m_dec->K          = m_pct->K;
	m_dec->N_cw       = m_pct->N_cw;
	m_dec->systematic = m_enc->systematic;

	tools::Factory_decoder_polar::store_args(this->ar, *m_dec);

	C::store_args();
}

template <class C, typename B, typename R, typename Q>
void Launcher_polar<C,B,R,Q>
::group_args()
{
	tools::Factory_frozenbits_generator::group_args(this->arg_group);
	tools::Factory_puncturer_polar     ::group_args(this->arg_group);
	tools::Factory_encoder_polar       ::group_args(this->arg_group);
	tools::Factory_decoder_polar       ::group_args(this->arg_group);

	C::group_args();
}

template <class C, typename B, typename R, typename Q>
void Launcher_polar<C,B,R,Q>
::print_header()
{
	tools::Factory_encoder_polar       ::header(this->pl_enc, *m_enc);
	tools::Factory_puncturer_polar     ::header(this->pl_pct, *m_pct);
	tools::Factory_frozenbits_generator::header(this->pl_enc, *m_fb);
	tools::Factory_decoder_polar       ::header(this->pl_dec, *m_dec);

	C::print_header();
}

template <class C, typename B, typename R, typename Q>
void Launcher_polar<C,B,R,Q>
::build_codec()
{
	this->codec = new tools::Codec_polar<B,Q>(*m_fb, *m_pct, *m_enc, *m_dec, this->m_chain_params->sim->n_threads);
}
}
}
