#if !defined(AFF3CT_8BIT_PREC) && !defined(AFF3CT_16BIT_PREC)

#ifndef LAUNCHER_EXIT_HPP_
#define LAUNCHER_EXIT_HPP_

#include <iostream>

#include "Factory/Simulation/EXIT/EXIT.hpp"
#include "Simulation/Simulation.hpp"
#include "Launcher/Launcher.hpp"

namespace aff3ct
{
namespace launcher
{
template <typename B = int, typename R = float>
class EXIT : public Launcher
{
protected:
	factory::EXIT::parameters params;

public:
	EXIT(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~EXIT() = default;

protected:
	virtual void get_description_args();
	virtual void store_args();

	virtual simulation::Simulation* build_simu();
};
}
}

#endif /* LAUNCHER_EXIT_HPP_ */

#endif