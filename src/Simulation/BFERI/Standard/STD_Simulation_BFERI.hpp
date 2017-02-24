#ifdef SYSTEMC
#include "../SystemC/SC_Simulation_BFERI.hpp"
#else

#ifndef SIMULATION_BFERI_HPP_
#define SIMULATION_BFERI_HPP_

#include <thread>
#include <chrono>
#include <vector>

#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/params.h"
#include "Tools/Display/Terminal/Terminal.hpp"

#include "Module/Monitor/Standard/Monitor_reduction.hpp"

#include "../Simulation_BFERI.hpp"

template <typename B, typename R, typename Q>
class Simulation_BFERI : public Simulation_BFERI_i<B,R,Q>
{
protected:
	// array of threads
	std::vector<std::thread> threads;

	// data vector
	std::vector<mipp::vector<B>> U_K;  // information bit vector
	std::vector<mipp::vector<B>> X_N1; // encoded codeword
	std::vector<mipp::vector<B>> X_N2; // interleaved codeword
	std::vector<mipp::vector<R>> X_N3; // modulate codeword
	std::vector<mipp::vector<R>> H_N;  // code gain for Rayleigh channels
	std::vector<mipp::vector<R>> Y_N1; // noisy codeword (after the channel noise)
	std::vector<mipp::vector<R>> Y_N2; // noisy codeword (after the filtering)
	std::vector<mipp::vector<Q>> Y_N3; // noisy codeword (after the quatization)
	std::vector<mipp::vector<Q>> Y_N4; // noisy codeword (after the demodulation)
	std::vector<mipp::vector<Q>> Y_N5; // noisy codeword (after the deinterleaving process)
	std::vector<mipp::vector<Q>> Y_N6; // noisy codeword (after the decoding process)
	std::vector<mipp::vector<Q>> Y_N7; // noisy codeword (after the interleaving process)
	std::vector<mipp::vector<B>> V_K;  // decoded codeword

	// objects
	Monitor_reduction<B,R> *monitor_red;
	Terminal               *terminal;

	// time points and durations
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_simu;
	std::chrono::nanoseconds d_snr;
	std::chrono::nanoseconds d_simu;
	std::vector<std::chrono::nanoseconds> d_sourc_total;
	std::vector<std::chrono::nanoseconds> d_crc_total;
	std::vector<std::chrono::nanoseconds> d_encod_total;
	std::vector<std::chrono::nanoseconds> d_inter_total;
	std::vector<std::chrono::nanoseconds> d_modul_total;
	std::vector<std::chrono::nanoseconds> d_chann_total;
	std::vector<std::chrono::nanoseconds> d_filte_total;
	std::vector<std::chrono::nanoseconds> d_quant_total;
	std::vector<std::chrono::nanoseconds> d_demod_total;
	std::vector<std::chrono::nanoseconds> d_corea_total;
	std::vector<std::chrono::nanoseconds> d_decod_total;
	std::vector<std::chrono::nanoseconds> d_deint_total;
	std::vector<std::chrono::nanoseconds> d_cobit_total;
	std::vector<std::chrono::nanoseconds> d_check_total;

	std::chrono::nanoseconds d_sourc_total_red;
	std::chrono::nanoseconds d_crc_total_red;
	std::chrono::nanoseconds d_encod_total_red;
	std::chrono::nanoseconds d_inter_total_red;
	std::chrono::nanoseconds d_modul_total_red;
	std::chrono::nanoseconds d_chann_total_red;
	std::chrono::nanoseconds d_filte_total_red;
	std::chrono::nanoseconds d_quant_total_red;
	std::chrono::nanoseconds d_demod_total_red;
	std::chrono::nanoseconds d_corea_total_red;
	std::chrono::nanoseconds d_decod_total_red;
	std::chrono::nanoseconds d_deint_total_red;
	std::chrono::nanoseconds d_cobit_total_red;
	std::chrono::nanoseconds d_check_total_red;

	std::chrono::nanoseconds d_sourc_total_sum;
	std::chrono::nanoseconds d_crc_total_sum;
	std::chrono::nanoseconds d_encod_total_sum;
	std::chrono::nanoseconds d_inter_total_sum;
	std::chrono::nanoseconds d_modul_total_sum;
	std::chrono::nanoseconds d_chann_total_sum;
	std::chrono::nanoseconds d_filte_total_sum;
	std::chrono::nanoseconds d_quant_total_sum;
	std::chrono::nanoseconds d_demod_total_sum;
	std::chrono::nanoseconds d_corea_total_sum;
	std::chrono::nanoseconds d_decod_total_sum;
	std::chrono::nanoseconds d_deint_total_sum;
	std::chrono::nanoseconds d_cobit_total_sum;
	std::chrono::nanoseconds d_check_total_sum;

public:
	Simulation_BFERI(const parameters& params);
	virtual ~Simulation_BFERI();

protected:
	        void _launch           ();
	virtual void release_objects   ();
	virtual void launch_postcompute();

private:
	static void build_communication_chain(Simulation_BFERI<B,R,Q> *simu, const int tid = 0);
	static void Monte_Carlo_method       (Simulation_BFERI<B,R,Q> *simu, const int tid = 0);
	static void simulation_loop          (Simulation_BFERI<B,R,Q> *simu, const int tid = 0);
	static void simulation_loop_debug    (Simulation_BFERI<B,R,Q> *simu);

	void time_reduction(const bool is_snr_done = false  );
	void time_report   (std::ostream &stream = std::clog);

	Terminal* build_terminal(const int tid = 0);
};

#endif /* SIMULATION_BFERI_HPP_ */

#endif /* SYSTEMC */
