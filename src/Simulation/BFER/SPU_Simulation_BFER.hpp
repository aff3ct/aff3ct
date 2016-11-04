#ifdef STARPU

#ifndef SPU_SIMULATION_BFER_HPP_
#define SPU_SIMULATION_BFER_HPP_

#include <thread>
#include <chrono>
#include <vector>

#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/params.h"
#include "Tools/Display/Terminal/Terminal.hpp"

#include "Module/Monitor/Standard/Monitor_reduction.hpp"

#include "Simulation_BFER_i.hpp"

template <typename B, typename R, typename Q>
class Simulation_BFER : public Simulation_BFER_i<B,R,Q>
{
protected:
	// array of threads
	std::vector<std::thread> threads;

	// data vector
	std::vector<mipp::vector<B>> U_K;  // information bit vector
	std::vector<mipp::vector<B>> X_N1; // encoded codeword
	std::vector<mipp::vector<B>> X_N2; // encoded and punctured codeword
	std::vector<mipp::vector<R>> X_N3; // modulate codeword
	std::vector<mipp::vector<R>> H_N;  // code gain for Rayleigh channels
	std::vector<mipp::vector<R>> Y_N1; // noisy codeword (after the channel noise)
	std::vector<mipp::vector<R>> Y_N2; // noisy codeword (after the filtering)
	std::vector<mipp::vector<R>> Y_N3; // noisy codeword (after the demodulation)
	std::vector<mipp::vector<Q>> Y_N4; // noisy codeword (after quantization)
	std::vector<mipp::vector<Q>> Y_N5; // noisy and depunctured codeword
	std::vector<mipp::vector<B>> V_K;  // decoded codeword 
	std::vector<mipp::vector<B>> V_N;  // decoded codeword (especially for simulation_bench and SC_FAST decoders)

	// objects
	Monitor_reduction<B> *monitor_red;
	Terminal             *terminal;

	// time points and durations
	std::chrono::nanoseconds d_decod_total_fake;

public:
	Simulation_BFER(const parameters& params);
	virtual ~Simulation_BFER();

protected:
	        void _launch         ();
	virtual void release_objects ();

private:
	static void build_communication_chain(Simulation_BFER<B,R,Q> *simu, const int tid = 0);
	static void Monte_Carlo_method       (Simulation_BFER<B,R,Q> *simu, const int tid = 0);
	static void simulation_loop          (Simulation_BFER<B,R,Q> *simu, const int tid = 0);

	Terminal* build_terminal(const int tid = 0);
};

#endif /* SPU_SIMULATION_BFER_HPP_ */

#endif /* STARPU */
