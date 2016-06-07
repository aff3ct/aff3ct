#ifndef SIMULATION_TURBO_HPP_
#define SIMULATION_TURBO_HPP_

#include <chrono>
#include <vector>
#include "../../../Tools/MIPP/mipp.h"

#include "../../Simulation.hpp"

#include "../../../Interleaver/Interleaver.hpp"
#include "../../../Source/Source.hpp"
#include "../../../CRC/CRC.hpp"
#include "../../../Encoder/Encoder.hpp"
#include "../../../Encoder/Encoder_sys.hpp"
#include "../../../Modulator/Modulator.hpp"
#include "../../../Channel/Channel.hpp"
#include "../../../Quantizer/Quantizer.hpp"
#include "../../../Tools/Turbo/Scaling_factor/Scaling_factor.hpp"
#include "../../../Decoder/Decoder.hpp"
#include "../../../Decoder/SISO.hpp"
#include "../../../Error/Error_analyzer.hpp"
#include "../../../Terminal/Terminal.hpp"

#include "../../../Tools/params.h"

template <typename B, typename R, typename Q, typename QD>
class Simulation_turbo : public Simulation
{
protected:
	// simulation parameters
	const t_simulation_param simu_params;
	const t_code_param       code_params;
	const t_encoder_param    enco_params;
	const t_channel_param    chan_params;
	const t_decoder_param    deco_params;

	// data vector
	mipp::vector<B> U_K;  // information vector without frozen bits inserted
	mipp::vector<B> X_N;  // encoded codeword
	mipp::vector<R> Y_N1; // noisy codeword (before quantization)
	mipp::vector<Q> Y_N2; // noisy codeword (after  quantization)
	mipp::vector<B> V_K;  // decoded codeword without frozen bits inserted

	// code specifications
	float code_rate;
	float sigma;

	Interleaver<short> *interleaver;

	// communication chain
	Source<B>           *source;
	CRC<B>              *crc;
	Encoder<B>          *encoder;
	Encoder_sys<B>      *sub_encoder;
	Modulator<B>        *modulator;
	Channel<B,R>        *channel;
	Quantizer<R,Q>      *quantizer;
	Scaling_factor<Q>   *sf;
	Decoder<B,Q>        *decoder;
	SISO<Q>             *siso;
	Error_analyzer<B,R> *analyzer;
	Terminal            *terminal;

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
	std::chrono::nanoseconds d_quant_total;
	std::chrono::nanoseconds d_load_total;
	std::chrono::nanoseconds d_decod_total;
	std::chrono::nanoseconds d_store_total;
	std::chrono::nanoseconds d_check_total;

public:
	Simulation_turbo(const t_simulation_param& simu_params,
	                 const t_code_param&       code_params,
	                 const t_encoder_param&    enco_params,
	                 const t_channel_param&    chan_params,
	                 const t_decoder_param&    deco_params);

	virtual ~Simulation_turbo();

	void launch();

protected:
	void build_communication_chain(const R& snr);
	void snr_loop();

protected:
	virtual void simulation_loop();
};

#endif /* SIMULATION_TURBO_HPP_ */
