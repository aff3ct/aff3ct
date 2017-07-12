#ifndef LAUNCHER_BFER_ITE_HPP_
#define LAUNCHER_BFER_ITE_HPP_

#include "Tools/Codec/Codec_SISO.hpp"
#include "Tools/Factory/Simulation/BFER/Factory_simulation_BFER_ite.hpp"

#if defined(SYSTEMC)
#include "Simulation/BFER/Iterative/SystemC/SC_Simulation_BFER_ite.hpp"
#elif defined(STARPU)
#include "Simulation/BFER/Iterative/StarPU/SPU_Simulation_BFER_ite.hpp"
#else
#include "Simulation/BFER/Iterative/Threads/Simulation_BFER_ite_threads.hpp"
#endif

#include "../Launcher.hpp"

namespace aff3ct
{
namespace launcher
{
template <typename B = int, typename R = float, typename Q = R>
class Launcher_BFER_ite : public Launcher
{
protected:
	tools::Codec_SISO<B,Q> *codec = nullptr;

	tools::Factory_simulation_BFER_ite::chain_parameters *m_chain_params = nullptr;
	tools::Factory_simulation_BFER_ite::parameters       *m_sim          = nullptr;

public:
	Launcher_BFER_ite(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_BFER_ite();

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

#endif /* LAUNCHER_BFER_ITE_HPP_ */
