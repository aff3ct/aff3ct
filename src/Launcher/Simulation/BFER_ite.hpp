#ifndef LAUNCHER_BFER_ITE_HPP_
#define LAUNCHER_BFER_ITE_HPP_

#include <iostream>

#include "Factory/Simulation/BFER/BFER_ite.hpp"
#include "Simulation/Simulation.hpp"
#include "Launcher/Launcher.hpp"

namespace aff3ct
{
namespace launcher
{
template <typename B = int, typename R = float, typename Q = R>
class BFER_ite : public Launcher
{
protected:
	factory::BFER_ite::parameters params;

public:
	BFER_ite(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~BFER_ite() = default;

protected:
	virtual void get_description_args();
	virtual void store_args();

	virtual simulation::Simulation* build_simu();
};
}
}

#endif /* LAUNCHER_BFER_ITE_HPP_ */
