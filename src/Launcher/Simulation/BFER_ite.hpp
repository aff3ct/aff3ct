#ifndef LAUNCHER_BFER_ITE_HPP_
#define LAUNCHER_BFER_ITE_HPP_

#include "Factory/Simulation/BFER/BFER_ite.hpp"

#include "../Launcher.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B = int, typename R = float, typename Q = R, int CRC = 0>
class BFER_ite : public Launcher
{
protected:
	factory::BFER_ite::parameters<C> params;

public:
	BFER_ite(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~BFER_ite();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();

	virtual simulation::Simulation* build_simu();

	virtual void print_header();
};
}
}

#include "BFER_ite.hxx"

#endif /* LAUNCHER_BFER_ITE_HPP_ */
