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
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();

	virtual simulation::Simulation* build_simu();

	virtual void print_header();
};
}
}

#endif /* LAUNCHER_EXIT_HPP_ */
