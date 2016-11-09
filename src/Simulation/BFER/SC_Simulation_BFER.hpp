#ifdef SYSTEMC

#ifndef SC_SIMULATION_BFER_HPP_
#define SC_SIMULATION_BFER_HPP_

#include <chrono>
#include <vector>

#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/params.h"

#include "Tools/SystemC/SC_Debug.hpp"
#include "Tools/SystemC/SC_Duplicator.hpp"
#include "Tools/Display/Terminal/Terminal.hpp"

#include "Simulation_BFER_i.hpp"

template <typename B, typename R, typename Q>
class Simulation_BFER : public Simulation_BFER_i<B,R,Q>
{
protected:
	Terminal *terminal;

	SC_Duplicator *duplicator[3];
	SC_Debug<B>   *dbg_B     [6];
	SC_Debug<R>   *dbg_R     [5];
	SC_Debug<Q>   *dbg_Q     [3];

	// time points and durations
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_simu;
	std::chrono::nanoseconds d_snr;
	std::chrono::nanoseconds d_simu;
	std::chrono::nanoseconds d_decod_total_fake;

public:
	Simulation_BFER(const parameters& params);
	virtual ~Simulation_BFER();

protected:
	        void _launch        ();
	virtual void release_objects();

private:
	void build_communication_chain();
	void launch_simulation        ();
	void bind_sockets             ();
	void bind_sockets_debug       ();

	static void terminal_temp_report(Simulation_BFER<B,R,Q> *simu);

	Terminal* build_terminal();
};

#endif /* SIMULATION_SC_BFER_HPP_ */

#endif /* SYSTEMC */
