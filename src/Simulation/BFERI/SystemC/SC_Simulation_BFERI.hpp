#ifdef SYSTEMC

#ifndef SC_SIMULATION_BFERI_HPP_
#define SC_SIMULATION_BFERI_HPP_

#include <condition_variable>
#include <mutex>
#include <chrono>
#include <vector>

#include "Tools/params.h"
#include "Tools/SystemC/SC_Debug.hpp"
#include "Tools/SystemC/SC_Router.hpp"
#include "Tools/SystemC/SC_Predicate.hpp"
#include "Tools/SystemC/SC_Duplicator.hpp"
#include "Tools/Display/Terminal/Terminal.hpp"

#include "../Simulation_BFERI.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B, typename R, typename Q>
class Simulation_BFERI : public Simulation_BFERI_i<B,R,Q>
{
private:
	std::mutex mutex_terminal;
	std::condition_variable cond_terminal;

protected:
	tools::Interleaver<int> *interleaver_e;
	module::Coset<B,Q>      *coset_real_i;
	tools::Terminal         *terminal;

	tools::SC_Duplicator *duplicator[5];
	tools::SC_Router     *router;
	tools::SC_Predicate  *predicate;

	tools::SC_Debug<B> *dbg_B[6];
	tools::SC_Debug<R> *dbg_R[4];
	tools::SC_Debug<Q> *dbg_Q[7];

	// time points and durations
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_simu;
	std::chrono::nanoseconds d_snr;
	std::chrono::nanoseconds d_simu;

public:
	Simulation_BFERI(const tools::parameters& params);
	virtual ~Simulation_BFERI();

protected:
	        void _launch        ();
	virtual void release_objects();

private:
	void build_communication_chain();
	void launch_simulation        ();
	void bind_sockets             ();
	void bind_sockets_debug       ();

	tools::Terminal* build_terminal(const int tid = 0);

	static void terminal_temp_report(Simulation_BFERI<B,R,Q> *simu);
};
}
}

#endif /* SC_SIMULATION_BFERI_HPP_ */

#endif /* SYSTEMC */
