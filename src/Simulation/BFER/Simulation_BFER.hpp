#ifdef SYSTEMC
#include "SC_Simulation_BFER.hpp"
#else

#ifndef SIMULATION_BFER_HPP_
#define SIMULATION_BFER_HPP_

#include <thread>
#include <chrono>
#include <vector>
#include "../../Tools/MIPP/mipp.h"

#include "../Simulation.hpp"

#include "../../Source/Source.hpp"
#include "../../CRC/CRC.hpp"
#include "../../Encoder/Encoder.hpp"
#include "../../Puncturer/Puncturer.hpp"
#include "../../Modulator/Modulator.hpp"
#include "../../Channel/Channel.hpp"
#include "../../Quantizer/Quantizer.hpp"
#include "../../Decoder/Decoder.hpp"
#include "../../Error/Error_analyzer.hpp"
#include "../../Error/Standard/Error_analyzer_reduction.hpp"
#include "../../Terminal/Terminal.hpp"

#include "../../Tools/params.h"
#include "../../Tools/Threads/Barrier.hpp"

template <typename B, typename R, typename Q>
class Simulation_BFER : public Simulation
{
protected:
	// simulation parameters
	const t_simulation_param &simu_params;
	const t_code_param       &code_params;
	const t_encoder_param    &enco_params;
	const t_mod_param        &mod_params;
	const t_channel_param    &chan_params;
	const t_decoder_param    &deco_params;

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
	std::vector<mipp::vector<B>> X_N2; // encoded and punctured codeword
	std::vector<mipp::vector<R>> X_N3; // modulate codeword
	std::vector<mipp::vector<R>> Y_N1; // noisy codeword (after the channel noise)
	std::vector<mipp::vector<R>> Y_N2; // noisy codeword (after the filtering)
	std::vector<mipp::vector<R>> Y_N3; // noisy codeword (after the demodulation)
	std::vector<mipp::vector<Q>> Y_N4; // noisy codeword (after quantization)
	std::vector<mipp::vector<Q>> Y_N5; // noisy and depunctured codeword
	std::vector<mipp::vector<B>> V_K;  // decoded codeword 
	std::vector<mipp::vector<B>> V_N;  // decoded codeword (especially for simulation_bench and SC_FAST decoders)

	// communication chain
	std::vector<Source<B>*>           source;
	std::vector<CRC<B>*>              crc;
	std::vector<Encoder<B>*>          encoder;
	std::vector<Puncturer<B,Q>*>      puncturer;
	std::vector<Modulator<B,R,R>*>    modulator;
	std::vector<Channel<R>*>          channel;
	std::vector<Quantizer<R,Q>*>      quantizer;
	std::vector<Decoder<B,Q>*>        decoder;
	std::vector<Error_analyzer<B>*>   analyzer;
	Error_analyzer_reduction<B>      *analyzer_red;
	Terminal                         *terminal;

	// time points and durations
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_snr;
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_simu;
	std::chrono::nanoseconds d_snr;
	std::chrono::nanoseconds d_simu;
	std::vector<std::chrono::nanoseconds> d_sourc_total;
	std::vector<std::chrono::nanoseconds> d_crc_total;
	std::vector<std::chrono::nanoseconds> d_encod_total;
	std::vector<std::chrono::nanoseconds> d_punct_total;
	std::vector<std::chrono::nanoseconds> d_modul_total;
	std::vector<std::chrono::nanoseconds> d_chann_total;
	std::vector<std::chrono::nanoseconds> d_filte_total;
	std::vector<std::chrono::nanoseconds> d_demod_total;
	std::vector<std::chrono::nanoseconds> d_quant_total;
	std::vector<std::chrono::nanoseconds> d_depun_total;
	std::vector<std::chrono::nanoseconds> d_load_total;
	std::vector<std::chrono::nanoseconds> d_decod_total;
	std::vector<std::chrono::nanoseconds> d_store_total;
	std::vector<std::chrono::nanoseconds> d_check_total;

	std::chrono::nanoseconds d_sourc_total_red;
	std::chrono::nanoseconds d_crc_total_red;
	std::chrono::nanoseconds d_encod_total_red;
	std::chrono::nanoseconds d_punct_total_red;
	std::chrono::nanoseconds d_modul_total_red;
	std::chrono::nanoseconds d_chann_total_red;
	std::chrono::nanoseconds d_filte_total_red;
	std::chrono::nanoseconds d_demod_total_red;
	std::chrono::nanoseconds d_quant_total_red;
	std::chrono::nanoseconds d_depun_total_red;
	std::chrono::nanoseconds d_load_total_red;
	std::chrono::nanoseconds d_decod_total_red;
	std::chrono::nanoseconds d_store_total_red;
	std::chrono::nanoseconds d_check_total_red;
	std::chrono::nanoseconds d_decod_all_red;

	std::chrono::nanoseconds d_sourc_total_sum;
	std::chrono::nanoseconds d_crc_total_sum;
	std::chrono::nanoseconds d_encod_total_sum;
	std::chrono::nanoseconds d_punct_total_sum;
	std::chrono::nanoseconds d_modul_total_sum;
	std::chrono::nanoseconds d_chann_total_sum;
	std::chrono::nanoseconds d_filte_total_sum;
	std::chrono::nanoseconds d_demod_total_sum;
	std::chrono::nanoseconds d_quant_total_sum;
	std::chrono::nanoseconds d_depun_total_sum;
	std::chrono::nanoseconds d_load_total_sum;
	std::chrono::nanoseconds d_decod_total_sum;
	std::chrono::nanoseconds d_store_total_sum;
	std::chrono::nanoseconds d_check_total_sum;

public:
	Simulation_BFER(const t_simulation_param& simu_params,
	                const t_code_param&       code_params,
	                const t_encoder_param&    enco_params,
	                const t_mod_param&        mod_params,
	                const t_channel_param&    chan_params,
	                const t_decoder_param&    deco_params);
	virtual ~Simulation_BFER();
	void launch();

private:
	static void Monte_Carlo_method       (Simulation_BFER<B,R,Q> *simu, const int tid = 0);
	static void build_communication_chain(Simulation_BFER<B,R,Q> *simu, const int tid = 0);
	static void simulation_loop          (Simulation_BFER<B,R,Q> *simu, const int tid = 0);
	static void simulation_loop_bench    (Simulation_BFER<B,R,Q> *simu, const int tid = 0);
	static void simulation_loop_debug    (Simulation_BFER<B,R,Q> *simu);
	static void trace                    (Simulation_BFER<B,R,Q> *simu);

	void time_reduction(const bool is_snr_done = false  );
	void time_report   (std::ostream &stream = std::clog);

protected:
	virtual void               release_objects  ();
	virtual void               launch_precompute();
	virtual void               snr_precompute   ();

	virtual Source<B>*         build_source     (                const int tid = 0);
	virtual CRC<B>*            build_crc        (                const int tid = 0);
	virtual Encoder<B>*        build_encoder    (                const int tid = 0) = 0;
	virtual Puncturer<B,Q>*    build_puncturer  (                const int tid = 0);
	virtual Modulator<B,R,R>*  build_modulator  (                const int tid = 0);
	virtual Channel<R>*        build_channel    (const int size, const int tid = 0);
	virtual Quantizer<R,Q>*    build_quantizer  (const int size, const int tid = 0);
	virtual Decoder<B,Q>*      build_decoder    (                const int tid = 0) = 0;
	virtual Error_analyzer<B>* build_analyzer   (                const int tid = 0);
	        Terminal*          build_terminal   (                const int tid = 0);
};

#endif /* SIMULATION_BFER_HPP_ */

#endif /* SYSTEMC */
