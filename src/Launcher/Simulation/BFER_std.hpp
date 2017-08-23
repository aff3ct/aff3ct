#ifndef LAUNCHER_BFER_STD_HPP_
#define LAUNCHER_BFER_STD_HPP_

#include "Factory/Simulation/BFER/BFER_std.hpp"

#include "../Launcher.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B = int, typename R = float, typename Q = R, int CRC = 0, int ITL = 0>
class BFER_std : public Launcher
{
protected:
	factory::BFER_std::parameters<C> params;

public:
	BFER_std(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~BFER_std();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();

	virtual simulation::Simulation* build_simu();

	virtual void print_header();
};
}
}

#include "BFER_std.hxx"

#endif /* LAUNCHER_BFER_STD_HPP_ */
