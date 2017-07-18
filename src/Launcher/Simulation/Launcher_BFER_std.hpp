#ifndef LAUNCHER_BFER_STD_HPP_
#define LAUNCHER_BFER_STD_HPP_

#include "Tools/Codec/Codec.hpp"
#include "Tools/Factory/Simulation/BFER/Factory_simulation_BFER_std.hpp"

#if defined(SYSTEMC)
#include "Simulation/BFER/Standard/SystemC/SC_Simulation_BFER_std.hpp"
#elif defined(STARPU)
#include "Simulation/BFER/Standard/StarPU/SPU_Simulation_BFER_std.hpp"
#else
#include "Simulation/BFER/Standard/Threads/Simulation_BFER_std_threads.hpp"
#endif

#include "../Launcher.hpp"

namespace aff3ct
{
namespace launcher
{
template <typename B = int, typename R = float, typename Q = R>
class Launcher_BFER_std : public Launcher
{
protected:
	tools::Codec<B,Q> *codec = nullptr;
	tools::Factory_simulation_BFER_std::parameters *params;

public:
	Launcher_BFER_std(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_BFER_std();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();

	virtual void build_codec() = 0;

	virtual simulation::Simulation* build_simu();

	virtual void print_header();
};
}
}

#endif /* LAUNCHER_BFER_STD_HPP_ */
