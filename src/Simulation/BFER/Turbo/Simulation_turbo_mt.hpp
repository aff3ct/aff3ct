#ifndef SIMULATION_TURBO_MT_HPP_
#define SIMULATION_TURBO_MT_HPP_

#include <thread>
/*#ifdef _WIN32
// see https://github.com/meganz/mingw-std-threads
#include <mingw.thread.h>
#endif*/

#include <chrono>
#include <vector>
#include "../../../Tools/MIPP/mipp.h"

#include "../../Simulation.hpp"

#include "../../../Interleaver/Interleaver.hpp"
#include "../../../Source/Source.hpp"
#include "../../../CRC/CRC.hpp"
#include "../../../Encoder/Encoder.hpp"
#include "../../../Encoder/RSC/Encoder_RSC_sys.hpp"
#include "../../../Modulator/Modulator.hpp"
#include "../../../Channel/Channel.hpp"
#include "../../../Quantizer/Quantizer.hpp"
#include "../../../Tools/Turbo/Scaling_factor/Scaling_factor.hpp"
#include "../../../Decoder/Decoder.hpp"
#include "../../../Decoder/SISO.hpp"
#include "../../../Error/Error_analyzer.hpp"
#include "../../../Error/Error_analyzer_reduction.hpp"
#include "../../../Terminal/Terminal.hpp"
#include "../../../Tools/params.h"
#include "../../../Tools/Threads/Barrier.hpp"

template <typename B, typename R, typename Q, typename QD>
class Simulation_turbo_mt : public Simulation
{
protected:
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
	std::vector<mipp::vector<B>> U_K;  // information vector without frozen bits inserted
	std::vector<mipp::vector<B>> X_N;  // encoded codeword
	std::vector<mipp::vector<R>> Y_N1; // noisy codeword (before quantization)
	std::vector<mipp::vector<Q>> Y_N2; // noisy codeword (after  quantization)
	std::vector<mipp::vector<B>> V_K;  // decoded codeword without frozen bits inserted

	// the trellis representation
	std::vector<mipp::vector<mipp::vector<int>>> trellis;

	// code specifications
	float code_rate;
	float sigma;

	Interleaver<short>* interleaver;

	// communication chain objects
	std::vector<Source<B>*>                    source;
	std::vector<CRC<B>*>                       crc;
	std::vector<Encoder<B>*>                   encoder;
	std::vector<Encoder_RSC_sys<B>*>               sub_encoder;
	std::vector<Modulator<B>*>                 modulator;
	std::vector<Channel<B,R>*>                 channel;
	std::vector<Quantizer<R,Q>*>               quantizer;
	std::vector<Scaling_factor<Q>*>            sf;
	std::vector<Decoder<B,Q>*>                 decoder;
	std::vector<SISO<Q>*>                      siso;
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
	std::vector<std::chrono::nanoseconds> d_quant_total;
	std::vector<std::chrono::nanoseconds> d_load_total;
	std::vector<std::chrono::nanoseconds> d_decod_total;
	std::vector<std::chrono::nanoseconds> d_store_total;
	std::vector<std::chrono::nanoseconds> d_check_total;

	std::chrono::nanoseconds d_sourc_total_red;
	std::chrono::nanoseconds d_encod_total_red;
	std::chrono::nanoseconds d_modul_total_red;
	std::chrono::nanoseconds d_chann_total_red;
	std::chrono::nanoseconds d_quant_total_red;
	std::chrono::nanoseconds d_load_total_red;
	std::chrono::nanoseconds d_decod_total_red;
	std::chrono::nanoseconds d_store_total_red;
	std::chrono::nanoseconds d_check_total_red;

	Barrier barrier;

public:
	Simulation_turbo_mt(const t_simulation_param& simu_params,
	                    const t_code_param&       code_params,
	                    const t_encoder_param&    enco_params,
	                    const t_channel_param&    chan_params,
	                    const t_decoder_param&    deco_params,
	                    const int n_threads = 1);

	virtual ~Simulation_turbo_mt();

	void launch();

protected:
	void snr_loop();
	static void build_communication_chain(Simulation_turbo_mt<B,R,Q,QD> *simu, const R& snr, const int tid = 0);
	static void simulation_Monte_Carlo(Simulation_turbo_mt<B,R,Q,QD> *simu, const R snr, const int tid);

protected:
	static void simulation_loop(Simulation_turbo_mt<B,R,Q,QD> *simu, const int tid = 0);
	static void time_reduction(Simulation_turbo_mt<B,R,Q,QD> *simu);
};

#endif /* SIMULATION_TURBO_MT_HPP_ */