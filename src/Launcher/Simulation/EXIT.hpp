#if !defined(PREC_8_BIT) && !defined(PREC_16_BIT)

#ifndef LAUNCHER_EXIT_HPP_
#define LAUNCHER_EXIT_HPP_

#include "Factory/Simulation/EXIT/EXIT.hpp"

#include "../Launcher.hpp"

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
	virtual ~EXIT();

protected:
	virtual void get_description_args();
	virtual void store_args();

	virtual simulation::Simulation* build_simu();
};
}
}

#endif /* LAUNCHER_EXIT_HPP_ */

#endif