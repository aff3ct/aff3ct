#ifdef SYSTEMC

#ifndef SC_SIMULATION_BFERI_HPP_
#define SC_SIMULATION_BFERI_HPP_

#include <chrono>
#include <vector>

#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/params.h"
#include "Tools/Threads/Barrier.hpp"

#include "Tools/SystemC/SC_Debug.hpp"
#include "Tools/SystemC/SC_Router.hpp"
#include "Tools/SystemC/SC_Predicate.hpp"
#include "Tools/SystemC/SC_Duplicator.hpp"

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

#include "Tools/Display/Terminal/Terminal.hpp"

#include "../Simulation.hpp"

template <typename B, typename R, typename Q>
class Simulation_BFERI : public Simulation
{
protected:
	const parameters &params; // simulation parameters

	Barrier barrier; // a barrier to synchronize the threads
	int n_frames;    // number of simulated frames per thread

	// code specifications
	float snr;
	float code_rate;
	float sigma;

	// code gain for Rayleigh channels
	std::vector<mipp::vector<R>> H;

	// data vector
	std::vector<mipp::vector<B>> X_N1; // fake encoded codeword (required to compile but never used)

	// communication chain
	std::vector<SC_Source<B>*>         source;
	std::vector<SC_CRC<B>*>            crc;
	std::vector<SC_Encoder<B>*>        encoder;
	            SC_Interleaver<int>   *interleaver_e;
	std::vector<SC_Modulator<B,R,Q>*>  modulator;
	std::vector<SC_Channel<R>*>        channel;
	std::vector<SC_Quantizer<R,Q>*>    quantizer;
	std::vector<SC_Interleaver<int>*>  interleaver;
	std::vector<SC_Coset<B,Q>*>        coset_real;
	            SC_Coset<B,Q>         *coset_real_i;
	std::vector<SC_SISO<Q>*>           siso;
	std::vector<SC_Decoder<B,Q>*>      decoder;
	std::vector<SC_Coset<B,B>*>        coset_bit;
	std::vector<SC_Monitor<B>*>        monitor;
	               Terminal           *terminal;

	SC_Duplicator *duplicator[5];
	SC_Router     *router;
	SC_Predicate  *predicate;

	SC_Debug<B> *dbg_B[6];
	SC_Debug<R> *dbg_R[3];
	SC_Debug<Q> *dbg_Q[7];

	// time points and durations
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_snr;
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_simu;
	std::chrono::nanoseconds d_snr;
	std::chrono::nanoseconds d_simu;

	std::chrono::nanoseconds d_decod_total_fake;

public:
	Simulation_BFERI(const parameters& params);
	virtual ~Simulation_BFERI();
	void launch();

private:
	void launch_simulation        ();
	void build_communication_chain();
	void bind_sockets             ();
	void bind_sockets_debug       ();

	static void terminal_temp_report(Simulation_BFERI<B,R,Q> *simu);

protected:
	virtual void              release_objects  ();
	virtual void              launch_precompute();
	virtual void              snr_precompute   ();

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

#include "SC_Simulation_BFERI.hxx"

#endif /* SC_SIMULATION_BFERI_HPP_ */

#endif /* SYSTEMC */
