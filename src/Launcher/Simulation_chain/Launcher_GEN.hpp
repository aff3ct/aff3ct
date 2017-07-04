#ifndef LAUNCHER_GEN_HPP_
#define LAUNCHER_GEN_HPP_

#include "Tools/Factory/Simulation/GEN/Factory_simulation_GEN.hpp"

#include "../Launcher.hpp"

namespace aff3ct
{
namespace launcher
{
template <typename B = int, typename R = float, typename Q = R>
class Launcher_GEN : public Launcher
{
protected:
	tools::Factory_simulation_GEN::chain_parameters_GEN * m_chain_params = nullptr;

public:
	Launcher_GEN(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_GEN();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();

	virtual simulation::Simulation* build_simu() = 0;

	virtual void print_header();
};
}
}

#endif /* LAUNCHER_GEN_HPP_ */
