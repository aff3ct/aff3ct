#include <iostream>

#include "Tools/Display/bash_tools.h"

#if defined(SYSTEMC)
#include "Simulation/BFER/Standard/SystemC/SC_Simulation_BFER_std.hpp"
#elif defined(STARPU)
#include "Simulation/BFER/Standard/StarPU/SPU_Simulation_BFER_std.hpp"
#else
#include "Simulation/BFER/Standard/Threads/Simulation_BFER_std_threads.hpp"
#endif
#include "Tools/Codec/BCH/Codec_BCH.hpp"

#include "Launcher_BFER_BCH.hpp"

using namespace aff3ct::tools;
using namespace aff3ct::simulation;
using namespace aff3ct::launcher;

template <typename B, typename R, typename Q>
Launcher_BFER_BCH<B,R,Q>
::Launcher_BFER_BCH(const int argc, const char **argv, std::ostream &stream)
: Launcher_BFER<B,R,Q>(argc, argv, stream)
{
	m_enc = new typename Factory_encoder_BCH<B  >::encoder_parameters    ();
	m_dec = new typename Factory_decoder_BCH<B,Q>::decoder_parameters_BCH();

	this->m_chain_params->enc = m_enc;
	this->m_chain_params->dec = m_dec;

	this->params.quantizer.n_bits     = 7;
	this->params.quantizer.n_decimals = 2;
}

template <typename B, typename R, typename Q>
Launcher_BFER_BCH<B,R,Q>
::~Launcher_BFER_BCH()
{
	if (this->m_chain_params->enc != nullptr)
		delete this->m_chain_params->enc;

	if (this->m_chain_params->dec != nullptr)
		delete this->m_chain_params->dec;
}

template <typename B, typename R, typename Q>
void Launcher_BFER_BCH<B,R,Q>
::build_args()
{
	Launcher_BFER<B,R,Q>::build_args();

	Factory_encoder_BCH<B  >::build_args(this->req_args, this->opt_args);
	Factory_decoder_BCH<B,Q>::build_args(this->req_args, this->opt_args);
}

template <typename B, typename R, typename Q>
void Launcher_BFER_BCH<B,R,Q>
::store_args()
{
	Launcher_BFER<B,R,Q>::store_args();

	Factory_encoder_BCH<B  >::store_args(this->ar, *m_enc);
	Factory_decoder_BCH<B,Q>::store_args(this->ar, *m_dec, this->m_chain_params->sim.K, this->m_chain_params->sim.N);
}

template <typename B, typename R, typename Q>
void Launcher_BFER_BCH<B,R,Q>
::group_args()
{
	Launcher_BFER<B,R,Q>::group_args();

	Factory_encoder_BCH<B  >::group_args(this->arg_group);
	Factory_decoder_BCH<B,Q>::group_args(this->arg_group);
}

template <typename B, typename R, typename Q>
void Launcher_BFER_BCH<B,R,Q>
::print_header()
{
	Factory_encoder_BCH<B  >::header(this->enc, *m_enc);
	Factory_decoder_BCH<B,Q>::header(this->dec, this->cde, *m_dec);

	Launcher_BFER<B,R,Q>::print_header();
}

template <typename B, typename R, typename Q>
Simulation* Launcher_BFER_BCH<B,R,Q>
::build_simu()
{
	this->codec = new Codec_BCH<B,Q>(this->params);
#if defined(SYSTEMC)
	return new SC_Simulation_BFER_std     <B,R,Q>(this->params, *this->codec);
#elif defined(STARPU)
	return new SPU_Simulation_BFER_std    <B,R,Q>(this->params, *this->codec);
#else
	return new Simulation_BFER_std_threads<B,R,Q>(this->params, *this->codec);
#endif
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::launcher::Launcher_BFER_BCH<B_8,R_8,Q_8>;
template class aff3ct::launcher::Launcher_BFER_BCH<B_16,R_16,Q_16>;
template class aff3ct::launcher::Launcher_BFER_BCH<B_32,R_32,Q_32>;
template class aff3ct::launcher::Launcher_BFER_BCH<B_64,R_64,Q_64>;
#else
template class aff3ct::launcher::Launcher_BFER_BCH<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
