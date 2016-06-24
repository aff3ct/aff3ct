#ifndef SIMULATION_POLAR_HPP_
#define SIMULATION_POLAR_HPP_

#include <chrono>
#include <vector>
#include "../../../Tools/MIPP/mipp.h"

#include "../../Simulation.hpp"

#include "../../../Source/Source.hpp"
#include "../../../CRC/CRC.hpp"
#include "../../../Encoder/Encoder.hpp"
#include "../../../Modulator/Modulator.hpp"
#include "../../../Channel/Channel.hpp"
#include "../../../Tools/Polar/Puncturer/Puncturer_polar.hpp"
#include "../../../Quantizer/Quantizer.hpp"
#include "../../../Decoder/Decoder.hpp"
#include "../../../Error/Error_analyzer.hpp"
#include "../../../Terminal/Terminal.hpp"

#include "../../../Tools/Polar/Frozenbits_generator/Frozenbits_generator.hpp"
#include "../../../Tools/params.h"

template <typename B, typename R, typename Q>
class Simulation_polar : public Simulation
{
protected:
	// if N is     a power of two, N_2 = N
	// if N is not a power of two, N_2 = next power of two from N (required for puncturing)
	const int N_2;
	
	// simulation parameters
	const t_simulation_param simu_params;
	const t_code_param       code_params;
	const t_encoder_param    enco_params;
	const t_channel_param    chan_params;
	const t_decoder_param    deco_params;

	// data vector
	mipp::vector<B> frozen_bits; // known bits (alias frozen bits) are set to true
	mipp::vector<B> U_K;         // information vector without frozen bits inserted
	mipp::vector<B> X_N;         // encoded codeword
	mipp::vector<R> Y_N1;        // noisy codeword (before quantization)
	mipp::vector<Q> Y_N2;        // noisy codeword (after  quantization)
	mipp::vector<B> V_K;         // decoded codeword with frozen bits inserted

	// code specifications
	float code_rate;
	float sigma;

	const bool is_generated_decoder;
	Frozenbits_generator<B> *fb_generator;

	// communication chain
	Source<B>            *source;
	CRC<B>               *crc;
	Encoder<B>           *encoder;
	Modulator<B,R>       *modulator;
	Channel<B,R>         *channel;
	Puncturer_polar<B,Q> *puncturer;
	Quantizer<R,Q>       *quantizer;
	Decoder<B,Q>         *decoder;
	Error_analyzer<B,R>  *analyzer;
	Terminal             *terminal;

	// number of frames to encode/decode in the simulation loop
	int n_frames;

	// time points and durations
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_snr;
	std::chrono::nanoseconds d_snr;
	std::chrono::nanoseconds d_simu;
	std::chrono::nanoseconds d_sourc_total;
	std::chrono::nanoseconds d_encod_total;
	std::chrono::nanoseconds d_modul_total;
	std::chrono::nanoseconds d_chann_total;
	std::chrono::nanoseconds d_punct_total;
	std::chrono::nanoseconds d_quant_total;
	std::chrono::nanoseconds d_load_total;
	std::chrono::nanoseconds d_decod_total;
	std::chrono::nanoseconds d_store_total;
	std::chrono::nanoseconds d_check_total;

public:
	Simulation_polar(const t_simulation_param& simu_params,
	                 const t_code_param&       code_params,
	                 const t_encoder_param&    enco_params,
	                 const t_channel_param&    chan_params,
	                 const t_decoder_param&    deco_params);

	virtual ~Simulation_polar();

	void launch();

protected:
	void build_communication_chain(const R& snr);
	void snr_loop();

protected:
	virtual void simulation_loop();
};

#endif /* SIMULATION_POLAR_HPP_ */
