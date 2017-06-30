#ifndef LAUNCHER_BFER_ITE_HPP_
#define LAUNCHER_BFER_ITE_HPP_

#include "Tools/Codec/Codec_SISO.hpp"
#include "Tools/Factory/Simulation/Factory_simulation_BFER_ite.hpp"

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
class Launcher_BFER_ite : public Launcher<B,R,Q>
{
protected:
	tools::Codec_SISO<B,Q> *codec = nullptr;

	tools::Factory_simulation_BFER_ite::chain_parameters_BFER_ite<B,R,Q> * m_chain_params = nullptr;

	tools::Header::params_list pl_sim, pl_cde, pl_src, pl_crc, pl_itl, pl_mod, pl_demod,
	                           pl_chn, pl_qua, pl_enc, pl_dec, pl_mon, pl_ter, pl_pct;

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
