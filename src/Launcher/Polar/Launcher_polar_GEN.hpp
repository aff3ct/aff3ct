#ifndef LAUNCHER_POLAR_GEN_HPP_
#define LAUNCHER_POLAR_GEN_HPP_

#include "Tools/Factory/Polar/Factory_polar_generator.hpp"
#include "Simulation/GEN/Code/Polar/Generation_polar.hpp"

namespace aff3ct
{
namespace launcher
{
template <class cLauncher, typename B = int, typename R = float, typename Q = R>
class Launcher_polar_GEN : public cLauncher
{
protected:
	typename tools::Factory_polar_generator<B,Q>::parameters_polar_generator *m_dec = nullptr;

public:
	Launcher_polar_GEN(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_polar_GEN();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();
	virtual void print_header();

	virtual simulation::Simulation* build_simu();
};
}
}

#include "Launcher_polar_GEN.hxx"

#endif /* LAUNCHER_POLAR_GEN_HPP_ */
