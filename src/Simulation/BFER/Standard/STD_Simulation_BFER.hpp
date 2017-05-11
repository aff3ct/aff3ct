#ifdef SYSTEMC
#include "../SystemC/SC_Simulation_BFER.hpp"
#elif defined(STARPU)
#include "../StarPU/SPU_Simulation_BFER.hpp"
#else

#ifndef SIMULATION_BFER_HPP_
#define SIMULATION_BFER_HPP_

#include <map>
#include <thread>
#include <chrono>
#include <vector>
#include <utility>

#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/params.h"
#include "Tools/Display/Terminal/Terminal.hpp"

#include "Module/Monitor/Standard/Monitor_reduction.hpp"

#include "../Simulation_BFER.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float, typename Q = R>
class Simulation_BFER : public Simulation_BFER_i<B,R,Q>
{
protected:
	// array of threads
	std::vector<std::thread> threads;

	// data vector
	std::vector<mipp::vector<B>> U_K1; // information bit vector
	std::vector<mipp::vector<B>> U_K2; // information bit vector + CRC bits
	std::vector<mipp::vector<B>> X_N1; // encoded codeword
	std::vector<mipp::vector<B>> X_N2; // encoded and punctured codeword
	std::vector<mipp::vector<R>> X_N3; // modulate codeword
	std::vector<mipp::vector<R>> H_N;  // code gain for Rayleigh channels
	std::vector<mipp::vector<R>> Y_N1; // noisy codeword (after the channel noise)
	std::vector<mipp::vector<R>> Y_N2; // noisy codeword (after the filtering)
	std::vector<mipp::vector<R>> Y_N3; // noisy codeword (after the demodulation)
	std::vector<mipp::vector<Q>> Y_N4; // noisy codeword (after quantization)
	std::vector<mipp::vector<Q>> Y_N5; // noisy and depunctured codeword
	std::vector<mipp::vector<B>> V_K1; // decoded bits + CRC bits
	std::vector<mipp::vector<B>> V_K2; // decoded bits

	// objects
	module::Monitor_reduction<B,R> *monitor_red;

	// terminal (for the output of the code)
	tools::Terminal *terminal;

	// time points and durations
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_simu;
	std::chrono::nanoseconds d_snr;
	std::chrono::nanoseconds d_simu;

	std::vector<std::map<std::pair<int, std::string>, std::chrono::nanoseconds>> durations;
	            std::map<std::pair<int, std::string>, std::chrono::nanoseconds>  durations_red;
	            std::map<std::pair<int, std::string>, std::chrono::nanoseconds>  durations_sum;

public:
	Simulation_BFER(const tools::parameters& params);
	virtual ~Simulation_BFER();

protected:
	        void _launch           ();
	virtual void release_objects   ();
	virtual void launch_postcompute();

private:
	static void build_communication_chain(Simulation_BFER<B,R,Q> *simu, const int tid = 0);
	static void Monte_Carlo_method       (Simulation_BFER<B,R,Q> *simu, const int tid = 0);
	static void simulation_loop          (Simulation_BFER<B,R,Q> *simu, const int tid = 0);
	static void simulation_loop_bench    (Simulation_BFER<B,R,Q> *simu, const int tid = 0);
	static void simulation_loop_debug    (Simulation_BFER<B,R,Q> *simu);

	void time_reduction(const bool is_snr_done = false  );
	void time_report   (std::ostream &stream = std::clog);

	tools::Terminal* build_terminal();
};
}
}

#endif /* SIMULATION_BFER_HPP_ */

#endif /* SYSTEMC */
