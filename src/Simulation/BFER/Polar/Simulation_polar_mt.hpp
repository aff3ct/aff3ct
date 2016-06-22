#ifndef SIMULATION_POLAR_MT_HPP_
#define SIMULATION_POLAR_MT_HPP_

#include <thread>
/*#ifdef _WIN32
// see https://github.com/meganz/mingw-std-threads
#include <mingw.thread.h>
#endif*/

#include <chrono>
#include <vector>
#include "../../../Tools/MIPP/mipp.h"

#include "../../Simulation.hpp"

#include "../../../Source/Source.hpp"
#include "../../../CRC/CRC.hpp"
#include "../../../Encoder/Polar/Encoder_polar.hpp"
#include "../../../Modulator/Modulator.hpp"
#include "../../../Channel/Channel.hpp"
#include "../../../Tools/Polar/Puncturer/Puncturer_polar.hpp"
#include "../../../Quantizer/Quantizer.hpp"
#include "../../../Decoder/Decoder.hpp"
#include "../../../Error/Error_analyzer.hpp"
#include "../../../Error/Error_analyzer_reduction.hpp"
#include "../../../Terminal/Terminal.hpp"

#include "../../../Tools/Polar/Frozenbits_generator/Frozenbits_generator.hpp"
#include "../../../Tools/params.h"
#include "../../../Tools/Threads/Barrier.hpp"

template <typename B, typename R, typename Q>
class Simulation_polar_mt : public Simulation
{
protected:
	// if N is     a power of two, N_2 = N,
	// if N is NOT a power of two, N_2 = next power of two from N (required for puncturing)
	const int N_2;

	// simulation parameters
	const t_simulation_param simu_params;
	const t_code_param       code_params;
	const t_encoder_param    enco_params;
	const t_channel_param    chan_params;
	const t_decoder_param    deco_params;

	// number of threads
	const int n_threads;

	// array of those threads
	std::vector<std::thread> threads;

	// data vector
	std::vector<mipp::vector<B>> frozen_bits; // known bits (alias frozen bits) are set to true
	std::vector<mipp::vector<B>> U_K;         // information vector without frozen bits inserted
	std::vector<mipp::vector<B>> U_N;         // information vector with frozen bits inserted: transmitted codeword
	std::vector<mipp::vector<B>> X_N;         // encoded codeword
	std::vector<mipp::vector<R>> Y_N1;        // noisy codeword (before quantization)
	std::vector<mipp::vector<Q>> Y_N2;        // noisy codeword (after  quantization)
	std::vector<mipp::vector<B>> V_N;         // decoded codeword without frozen bits inserted

	// code specifications
	float code_rate;
	float sigma;

	const bool is_generated_decoder;
	Frozenbits_generator<B> *fb_generator;

	// communication chain objects
	std::vector<Source<B>*>                    source;
	std::vector<CRC<B>*>                       crc;
	std::vector<Encoder_polar<B>*>             encoder;
	std::vector<Modulator<B,R>*>               modulator;
	std::vector<Channel<B,R>*>                 channel;
	std::vector<Puncturer_polar<B,Q>*>         puncturer;
	std::vector<Quantizer<R,Q>*>               quantizer;
	std::vector<Decoder<B,Q>*>                 decoder;
	std::vector<Error_analyzer<B,R>*>          analyzer;
	            Error_analyzer_reduction<B,R> *analyzer_red;
	            Terminal                      *terminal;

	// number of frames to encode/decode per encoder/decoder instance in the simulation loop
	int n_frames;

	// time points and durations
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_snr;
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_simu;
	std::chrono::nanoseconds d_snr;
	std::chrono::nanoseconds d_simu;
	std::vector<std::chrono::nanoseconds> d_sourc_total;
	std::vector<std::chrono::nanoseconds> d_encod_total;
	std::vector<std::chrono::nanoseconds> d_modul_total;
	std::vector<std::chrono::nanoseconds> d_chann_total;
	std::vector<std::chrono::nanoseconds> d_punct_total;
	std::vector<std::chrono::nanoseconds> d_quant_total;
	std::vector<std::chrono::nanoseconds> d_load_total;
	std::vector<std::chrono::nanoseconds> d_decod_total;
	std::vector<std::chrono::nanoseconds> d_store_total;
	std::vector<std::chrono::nanoseconds> d_check_total;

	std::chrono::nanoseconds d_sourc_total_red;
	std::chrono::nanoseconds d_encod_total_red;
	std::chrono::nanoseconds d_modul_total_red;
	std::chrono::nanoseconds d_chann_total_red;
	std::chrono::nanoseconds d_punct_total_red;
	std::chrono::nanoseconds d_quant_total_red;
	std::chrono::nanoseconds d_load_total_red;
	std::chrono::nanoseconds d_decod_total_red;
	std::chrono::nanoseconds d_store_total_red;
	std::chrono::nanoseconds d_check_total_red;

	Barrier barrier;

public:
	Simulation_polar_mt(const t_simulation_param& simu_params,
	                    const t_code_param&       code_params,
	                    const t_encoder_param&    enco_params,
	                    const t_channel_param&    chan_params,
	                    const t_decoder_param&    deco_params,
	                    const int n_threads = 1);

	virtual ~Simulation_polar_mt();

	void launch();

protected:
	void snr_loop();
	static void build_communication_chain(Simulation_polar_mt<B,R,Q> *simu, const R& snr, const int tid = 0);
	static void simulation_Monte_Carlo(Simulation_polar_mt<B,R,Q> *simu, const R snr, const int tid);

protected:
	static void simulation_loop(Simulation_polar_mt<B,R,Q> *simu, const int tid = 0);
	static void time_reduction(Simulation_polar_mt<B,R,Q> *simu);
};

#endif /* SIMULATION_POLAR_MT_HPP_ */