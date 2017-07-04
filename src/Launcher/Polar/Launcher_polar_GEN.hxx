#include <iostream>

#include "Launcher_polar_GEN.hpp"

namespace aff3ct
{
namespace launcher
{
template <class cLauncher, typename B, typename R, typename Q>
Launcher_polar_GEN<cLauncher,B,R,Q>
::Launcher_polar_GEN(const int argc, const char **argv, std::ostream &stream)
: cLauncher(argc, argv, stream)
{
	m_dec = new typename tools::Factory_polar_generator<B,Q>::parameters_polar_generator();

	this->m_chain_params->dec = m_dec;
}

template <class cLauncher, typename B, typename R, typename Q>
Launcher_polar_GEN<cLauncher,B,R,Q>
::~Launcher_polar_GEN()
{
	if (this->m_chain_params->dec != nullptr)
		delete this->m_chain_params->dec;
}

template <class cLauncher, typename B, typename R, typename Q>
void Launcher_polar_GEN<cLauncher,B,R,Q>
::build_args()
{
	cLauncher::build_args();

	tools::Factory_polar_generator<B,Q>::build_args(this->req_args, this->opt_args);
}

template <class cLauncher, typename B, typename R, typename Q>
void Launcher_polar_GEN<cLauncher,B,R,Q>
::store_args()
{
	cLauncher::store_args();

	tools::Factory_polar_generator<B,Q>::store_args(this->ar, *m_dec, this->simu_params->K, this->simu_params->N);
}

template <class cLauncher, typename B, typename R, typename Q>
void Launcher_polar_GEN<cLauncher,B,R,Q>
::group_args()
{
	cLauncher::group_args();

	tools::Factory_polar_generator<B,Q>::group_args(this->arg_group);
}

template <class cLauncher, typename B, typename R, typename Q>
void Launcher_polar_GEN<cLauncher,B,R,Q>
::print_header()
{
	tools::Factory_polar_generator<B,Q>::header(this->pl_dec, this->pl_cde, *m_dec);

	cLauncher::print_header();
}

template <class cLauncher, typename B, typename R, typename Q>
simulation::Simulation* Launcher_polar_GEN<cLauncher,B,R,Q>
::build_simu()
{
//	return new simulation::Generation_polar(this->params);
	return nullptr;
}
}
}
