#include <iostream>

#include "Tools/Codec/RA/Codec_RA.hpp"
#include "Launcher_RA.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B, typename R, typename Q>
Launcher_RA<C,B,R,Q>
::Launcher_RA(const int argc, const char **argv, std::ostream &stream)
: C(argc, argv, stream)
{
	m_enc = new tools::Factory_encoder_RA::parameters();
	m_dec = new tools::Factory_decoder_RA::parameters();

	this->m_chain_params->enc = m_enc;
	this->m_chain_params->dec = m_dec;

//	this->params.quantizer  .n_bits     = 7;
//	this->params.quantizer  .n_decimals = 2;
}

template <class C, typename B, typename R, typename Q>
Launcher_RA<C,B,R,Q>
::~Launcher_RA()
{
	if (this->m_chain_params->enc != nullptr)
		delete this->m_chain_params->enc;

	if (this->m_chain_params->dec != nullptr)
		delete this->m_chain_params->dec;
}

template <class C, typename B, typename R, typename Q>
void Launcher_RA<C,B,R,Q>
::build_args()
{
	C::build_args();

	tools::Factory_encoder_RA::build_args(this->req_args, this->opt_args);
	tools::Factory_decoder_RA::build_args(this->req_args, this->opt_args);
}

template <class C, typename B, typename R, typename Q>
void Launcher_RA<C,B,R,Q>
::store_args()
{
	C::store_args();

	tools::Factory_encoder_RA::store_args(this->ar, *m_enc);
	tools::Factory_decoder_RA::store_args(this->ar, *m_dec);
}

template <class C, typename B, typename R, typename Q>
void Launcher_RA<C,B,R,Q>
::group_args()
{
	C::group_args();

	tools::Factory_encoder_RA::group_args(this->arg_group);
	tools::Factory_decoder_RA::group_args(this->arg_group);
}

template <class C, typename B, typename R, typename Q>
void Launcher_RA<C,B,R,Q>
::print_header()
{
	tools::Factory_encoder_RA::header(this->pl_enc, *m_enc);
	tools::Factory_decoder_RA::header(this->pl_dec, this->pl_itl, *m_dec);

	C::print_header();
}

template <class C, typename B, typename R, typename Q>
void Launcher_RA<C,B,R,Q>
::build_codec()
{
	this->codec = new tools::Codec_RA<B,Q>(*m_enc, *m_dec);
}
}
}
