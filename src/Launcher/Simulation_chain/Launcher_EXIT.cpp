#include <thread>
#include <string>
#include <iostream>

#include "Launcher_EXIT.hpp"

using namespace aff3ct::tools;
using namespace aff3ct::launcher;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
Launcher_EXIT<B,R,Q>
::Launcher_EXIT(const int argc, const char **argv, std::ostream &stream)
: Launcher<B,R,Q>(argc, argv, stream), codec(nullptr), m_chain_params(new Factory_simulation_EXIT::chain_parameters_EXIT<B,R,Q>())
{
	this->chain_params =  m_chain_params;
	this->simu_params  = &m_chain_params->sim;
}

template <typename B, typename R, typename Q>
Launcher_EXIT<B,R,Q>
::~Launcher_EXIT()
{
	if (codec != nullptr)
		delete codec;

	if (m_chain_params != nullptr)
		delete m_chain_params;
}

template <typename B, typename R, typename Q>
void Launcher_EXIT<B,R,Q>
::build_args()
{
	Launcher<B,R,Q>::build_args();

	Factory_simulation_EXIT::build_args(this->req_args, this->opt_args);

	Factory_source<B>      ::build_args(this->req_args, this->opt_args);

	Factory_modem<B,R,Q>   ::build_args(this->req_args, this->opt_args);

	Factory_channel<R>     ::build_args(this->req_args, this->opt_args);

	Factory_terminal_EXIT  ::build_args(this->req_args, this->opt_args);
}

template <typename B, typename R, typename Q>
void Launcher_EXIT<B,R,Q>
::store_args()
{
	Launcher<B,R,Q>::store_args();

	Factory_simulation_EXIT::store_args(this->ar, m_chain_params->sim);

	Factory_source<B>      ::store_args(this->ar, m_chain_params->src);

	Factory_modem<B,R,Q>   ::store_args(this->ar, m_chain_params->modem, m_chain_params->sim.N);

	Factory_channel<R>     ::store_args(this->ar, m_chain_params->chn);

	Factory_terminal_EXIT  ::store_args(this->ar, m_chain_params->ter);
}

template <typename B, typename R, typename Q>
void Launcher_EXIT<B,R,Q>
::group_args()
{
	Launcher<B,R,Q>::group_args();

	Factory_simulation_EXIT::group_args(this->arg_group);

	Factory_source<B>      ::group_args(this->arg_group);

	Factory_modem<B,R,Q>   ::group_args(this->arg_group);

	Factory_channel<R>     ::group_args(this->arg_group);

	Factory_terminal_EXIT  ::group_args(this->arg_group);
}

template <typename B, typename R, typename Q>
void Launcher_EXIT<B,R,Q>
::print_header()
{
	Factory_simulation_EXIT::header(this->pl_sim, this->pl_cde, m_chain_params->sim);

	Factory_source<B>      ::header(this->pl_src, m_chain_params->src);

	Factory_modem<B,R,Q>   ::header(this->pl_mod, this->pl_demod, m_chain_params->modem);

	Factory_channel<R>     ::header(this->pl_chn, m_chain_params->chn);

	Factory_terminal_EXIT  ::header(this->pl_ter, m_chain_params->ter);

	Launcher<B,R,Q>::print_header();
}

template <typename B, typename R, typename Q>
Simulation* Launcher_EXIT<B,R,Q>
::build_simu()
{
	this->build_codec();

	return new Simulation_EXIT<B,R>(this->params, *codec);
}


// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::launcher::Launcher_EXIT<B_8,R_8,Q_8>;
template class aff3ct::launcher::Launcher_EXIT<B_16,R_16,Q_16>;
template class aff3ct::launcher::Launcher_EXIT<B_32,R_32,Q_32>;
template class aff3ct::launcher::Launcher_EXIT<B_64,R_64,Q_64>;
#else
template class aff3ct::launcher::Launcher_EXIT<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
