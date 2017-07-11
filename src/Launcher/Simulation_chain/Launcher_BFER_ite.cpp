#include "Launcher_BFER_ite.hpp"
#include <thread>
#include <string>
#include <iostream>


using namespace aff3ct::tools;
using namespace aff3ct::launcher;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
Launcher_BFER_ite<B,R,Q>
::Launcher_BFER_ite(const int argc, const char **argv, std::ostream &stream)
: Launcher(argc, argv, stream), codec(nullptr), m_chain_params(new Factory_simulation_BFER_ite::chain_parameters_BFER_ite<B,R,Q>())
{
	m_sim = dynamic_cast<Factory_simulation_BFER_ite::simu_parameters_BFER_ite*>(m_chain_params->sim);
	this->chain_params = m_chain_params;
	this->simu_params  = m_chain_params->sim;
}

template <typename B, typename R, typename Q>
Launcher_BFER_ite<B,R,Q>
::~Launcher_BFER_ite()
{
	if (codec != nullptr)
		delete codec;
}

template <typename B, typename R, typename Q>
void Launcher_BFER_ite<B,R,Q>
::build_args()
{
	Launcher::build_args();

	Factory_simulation_BFER_ite::build_args(this->req_args, this->opt_args);

	Factory_source<B>          ::build_args(this->req_args, this->opt_args);

	Factory_CRC<B>             ::build_args(this->req_args, this->opt_args);

	Factory_interleaver<int>   ::build_args(this->req_args, this->opt_args);

	Factory_modem<B,R,Q>       ::build_args(this->req_args, this->opt_args);

	Factory_channel<R>         ::build_args(this->req_args, this->opt_args);

	Factory_quantizer<R,Q>     ::build_args(this->req_args, this->opt_args);

	Factory_monitor<B>         ::build_args(this->req_args, this->opt_args);

	Factory_terminal_BFER      ::build_args(this->req_args, this->opt_args);
}

template <typename B, typename R, typename Q>
void Launcher_BFER_ite<B,R,Q>
::store_args()
{
	Launcher::store_args();

	Factory_simulation_BFER_ite::store_args(this->ar, *m_sim);

	Factory_source<B>          ::store_args(this->ar, m_chain_params->src,   m_sim->K,           m_sim->inter_frame_level);

	Factory_CRC<B>             ::store_args(this->ar, m_chain_params->crc,   m_sim->K, m_sim->N, m_sim->inter_frame_level);

	Factory_interleaver<int>   ::store_args(this->ar, m_chain_params->itl,             m_sim->N, m_sim->inter_frame_level);

	Factory_modem<B,R,Q>       ::store_args(this->ar, m_chain_params->modem,           m_sim->N, m_sim->inter_frame_level);

	bool complex   = m_chain_params->modem.complex;
	bool add_users = (m_chain_params->modem.type == "SCMA");
	Factory_channel<R>         ::store_args(this->ar, m_chain_params->chn,   m_sim->N, complex, add_users, m_sim->inter_frame_level);

	Factory_quantizer<R,Q>     ::store_args(this->ar, m_chain_params->qua,             m_sim->N, m_sim->inter_frame_level);

	Factory_monitor<B>         ::store_args(this->ar, m_chain_params->mon,   m_sim->K,           m_sim->inter_frame_level);

	Factory_terminal_BFER      ::store_args(this->ar, m_chain_params->ter);
}

template <typename B, typename R, typename Q>
void Launcher_BFER_ite<B,R,Q>
::group_args()
{
	Launcher::group_args();

	Factory_simulation_BFER_ite::group_args(this->arg_group);

	Factory_source<B>          ::group_args(this->arg_group);

	Factory_CRC<B>             ::group_args(this->arg_group);

	Factory_interleaver<int>   ::group_args(this->arg_group);

	Factory_modem<B,R,Q>       ::group_args(this->arg_group);

	Factory_channel<R>         ::group_args(this->arg_group);

	Factory_quantizer<R,Q>     ::group_args(this->arg_group);

	Factory_monitor<B>         ::group_args(this->arg_group);

	Factory_terminal_BFER      ::group_args(this->arg_group);
}

template <typename B, typename R, typename Q>
void Launcher_BFER_ite<B,R,Q>
::print_header()
{
	Factory_simulation_BFER_ite::header(this->pl_sim, this->pl_cde, *m_sim);

	Factory_source<B>          ::header(this->pl_src, m_chain_params->src);

	Factory_CRC<B>             ::header(this->pl_crc, m_chain_params->crc);

	Factory_interleaver<int>   ::header(this->pl_itl, m_chain_params->itl);

	Factory_modem<B,R,Q>       ::header(this->pl_mod, this->pl_demod, m_chain_params->modem);

	Factory_channel<R>         ::header(this->pl_chn, m_chain_params->chn);

	Factory_quantizer<R,Q>     ::header(this->pl_qua, m_chain_params->qua);

	Factory_monitor<B>         ::header(this->pl_mon, m_chain_params->mon);

	Factory_terminal_BFER      ::header(this->pl_ter, m_chain_params->ter);

	Launcher::print_header();
}

template <typename B, typename R, typename Q>
Simulation* Launcher_BFER_ite<B,R,Q>
::build_simu()
{
	this->build_codec();

#if defined(SYSTEMC)
	return new SC_Simulation_BFER_ite     <B,R,Q>(*m_chain_params, *codec);
#elif defined(STARPU)
	return new SPU_Simulation_BFER_ite    <B,R,Q>(*m_chain_params, *codec);
#else
	return new Simulation_BFER_ite_threads<B,R,Q>(*m_chain_params, *codec);
#endif
}


// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::launcher::Launcher_BFER_ite<B_8,R_8,Q_8>;
template class aff3ct::launcher::Launcher_BFER_ite<B_16,R_16,Q_16>;
template class aff3ct::launcher::Launcher_BFER_ite<B_32,R_32,Q_32>;
template class aff3ct::launcher::Launcher_BFER_ite<B_64,R_64,Q_64>;
#else
template class aff3ct::launcher::Launcher_BFER_ite<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
