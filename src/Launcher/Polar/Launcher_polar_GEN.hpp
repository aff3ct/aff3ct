#ifndef LAUNCHER_POLAR_GEN_HPP_
#define LAUNCHER_POLAR_GEN_HPP_

#include "Tools/Factory/Polar/Factory_decoder_polar_gen.hpp"
#include "Simulation/GEN/Code/Polar/Generation_polar.hpp"

namespace aff3ct
{
namespace launcher
{
template <class cLauncher, typename B = int, typename R = float, typename Q = R>
class Launcher_polar_GEN : public cLauncher
{
public:
	Launcher_polar_GEN(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_polar_GEN();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();
	virtual void print_header();

	virtual simulation::Simulation* build_simu();

	typename tools::Factory_decoder_polar_gen<B,Q>::decoder_parameters_polar_gen *m_dec = nullptr;
};
}
}

#include "Launcher_polar_GEN.hxx"

#endif /* LAUNCHER_POLAR_GEN_HPP_ */
