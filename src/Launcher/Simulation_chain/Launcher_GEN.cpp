#include <Launcher/Simulation_chain/Launcher_GEN.hpp>
#include <thread>
#include <string>
#include <iostream>

using namespace aff3ct::tools;
using namespace aff3ct::launcher;

template <typename B, typename R, typename Q>
Launcher_GEN<B,R,Q>
::Launcher_GEN(const int argc, const char **argv, std::ostream &stream)
: Launcher(argc, argv, stream), m_chain_params(new Factory_simulation_GEN::chain_parameters_GEN())
{
	this->chain_params = m_chain_params;
	this->simu_params  = m_chain_params->sim;
}

template <typename B, typename R, typename Q>
Launcher_GEN<B,R,Q>
::~Launcher_GEN()
{
	if (m_chain_params != nullptr)
		delete m_chain_params;
}

template <typename B, typename R, typename Q>
void Launcher_GEN<B,R,Q>
::build_args()
{
	Launcher::build_args();

	Factory_simulation_GEN::build_args(this->req_args, this->opt_args);
}

template <typename B, typename R, typename Q>
void Launcher_GEN<B,R,Q>
::store_args()
{
	Launcher::store_args();

	Factory_simulation_GEN::store_args(this->ar, *m_chain_params->sim);
}

template <typename B, typename R, typename Q>
void Launcher_GEN<B,R,Q>
::group_args()
{
	Launcher::group_args();

	Factory_simulation_GEN::group_args(this->arg_group);
}

template <typename B, typename R, typename Q>
void Launcher_GEN<B,R,Q>
::print_header()
{
	Factory_simulation_GEN::header(this->pl_sim, this->pl_cde, *m_chain_params->sim);

	Launcher::print_header();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::launcher::Launcher_GEN<B_8,R_8,Q_8>;
template class aff3ct::launcher::Launcher_GEN<B_16,R_16,Q_16>;
template class aff3ct::launcher::Launcher_GEN<B_32,R_32,Q_32>;
template class aff3ct::launcher::Launcher_GEN<B_64,R_64,Q_64>;
#else
template class aff3ct::launcher::Launcher_GEN<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
