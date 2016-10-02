#ifdef SYSTEMC
#include "SC_Simulation_BFERI.hpp"
#else

#ifndef SIMULATION_BFERI_HPP_
#define SIMULATION_BFERI_HPP_

#include <thread>
#include <chrono>
#include <vector>

#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/params.h"
#include "Tools/Threads/Barrier.hpp"

#include "Module/Source/Source.hpp"
#include "Module/CRC/CRC.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Modulator/Modulator.hpp"
#include "Module/Channel/Channel.hpp"
#include "Module/Quantizer/Quantizer.hpp"
#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Coset/Coset.hpp"
#include "Module/Decoder/SISO.hpp"
#include "Module/Decoder/Decoder.hpp"
#include "Module/Monitor/Monitor.hpp"
#include "Module/Monitor/Standard/Monitor_reduction.hpp"

#include "Tools/Display/Terminal/Terminal.hpp"

#include "../Simulation.hpp"

template <typename B, typename R, typename Q>
class Simulation_BFERI : public Simulation
{
protected:
	const parameters &params;         // simulation parameters

	std::vector<std::thread> threads; // array of threads
	Barrier barrier;                  // a barrier to synchronize the threads
	int n_frames;                     // number of simulated frames per thread

	// code specifications
	float snr;
	float code_rate;
	float sigma;

	// data vector
	std::vector<mipp::vector<B>> U_K;  // information bit vector
	std::vector<mipp::vector<B>> X_N1; // encoded codeword
	std::vector<mipp::vector<B>> X_N2; // interleaved codeword
	std::vector<mipp::vector<R>> X_N3; // modulate codeword
	std::vector<mipp::vector<R>> Y_N1; // noisy codeword (after the channel noise)
	std::vector<mipp::vector<R>> Y_N2; // noisy codeword (after the filtering)
	std::vector<mipp::vector<Q>> Y_N3; // noisy codeword (after the quatization)
	std::vector<mipp::vector<Q>> Y_N4; // noisy codeword (after the demodulation)
	std::vector<mipp::vector<Q>> Y_N5; // noisy codeword (after the deinterleaving process)
	std::vector<mipp::vector<Q>> Y_N6; // noisy codeword (after the decoding process)
	std::vector<mipp::vector<Q>> Y_N7; // noisy codeword (after the interleaving process)
	std::vector<mipp::vector<B>> V_K;  // decoded codeword

	// communication chain
	std::vector<Source<B>*>         source;
	std::vector<CRC<B>*>            crc;
	std::vector<Encoder<B>*>        encoder;
	std::vector<Modulator<B,R,Q>*>  modulator;
	std::vector<Channel<R>*>        channel;
	std::vector<Quantizer<R,Q>*>    quantizer;
	std::vector<Interleaver<int>*>  interleaver;
	std::vector<Coset<B,Q>*>        coset_real;
	std::vector<SISO<Q>*>           siso;
	std::vector<Decoder<B,Q>*>      decoder;
	std::vector<Coset<B,B>*>        coset_bit;
	std::vector<Monitor<B>*>        monitor;
	Monitor_reduction<B>           *monitor_red;
	Terminal                       *terminal;

	// time points and durations
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_snr;
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
	void launch();

private:
	static void Monte_Carlo_method       (Simulation_BFERI<B,R,Q> *simu, const int tid = 0);
	static void build_communication_chain(Simulation_BFERI<B,R,Q> *simu, const int tid = 0);
	static void simulation_loop          (Simulation_BFERI<B,R,Q> *simu, const int tid = 0);
	static void simulation_loop_bench    (Simulation_BFERI<B,R,Q> *simu, const int tid = 0);
	static void simulation_loop_debug    (Simulation_BFERI<B,R,Q> *simu);

	void time_reduction(const bool is_snr_done = false  );
	void time_report   (std::ostream &stream = std::clog);

protected:
	virtual void               release_objects  ();
	virtual void               launch_precompute();
	virtual void               snr_precompute   ();

	virtual Source<B>*        build_source     (                const int tid = 0);
	virtual CRC<B>*           build_crc        (                const int tid = 0);
	virtual Encoder<B>*       build_encoder    (                const int tid = 0);
	virtual Interleaver<int>* build_interleaver(                const int tid = 0);
	virtual Modulator<B,R,Q>* build_modulator  (                const int tid = 0);
	virtual Channel<R>*       build_channel    (const int size, const int tid = 0);
	virtual Quantizer<R,Q>*   build_quantizer  (const int size, const int tid = 0);
	virtual SISO<Q>*          build_siso       (                const int tid = 0) = 0;
	virtual Coset<B,Q>*       build_coset_real (                const int tid = 0);
	virtual Decoder<B,Q>*     build_decoder    (                const int tid = 0) = 0;
	virtual Coset<B,B>*       build_coset_bit  (                const int tid = 0);
	virtual Monitor<B>*       build_monitor    (                const int tid = 0);
	        Terminal*         build_terminal   (                const int tid = 0);
};

#endif /* SIMULATION_BFERI_HPP_ */

#endif /* SYSTEMC */
