#ifndef SIMULATION_BCH_HPP_
#define SIMULATION_BCH_HPP_

#include <vector>
#include "../../../Tools/MIPP/mipp.h"

#include <chrono>

#include "../../Simulation.hpp"

#include "../../../Source/Source.hpp"
#include "../../../Encoder/Encoder.hpp"
#include "../../../Modulator/Modulator_BPSK.hpp"
#include "../../../Channel/Channel.hpp"
#include "../../../Quantizer/Quantizer.hpp"
#include "../../../Decoder/Decoder.hpp"
#include "../../../Error/Error_analyzer.hpp"
#include "../../../Terminal/Terminal.hpp"
#include "../../../Tools/params.h"
#include "../../../Tools/Algebraic/Galois.hpp"

template <typename B, typename R, typename Q>
class Simulation_BCH : public Simulation
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
	R code_rate;
	R sigma;

	// communication chain
	Source<B>           *source;
	Encoder<B>          *encoder;
	Modulator<B>        *modulator;
	Channel<B,R>        *channel;
	Quantizer<R,Q>      *quantizer;
	Decoder<B,Q>        *decoder;
	Error_analyzer<B,R> *analyzer;
	Terminal            *terminal;
	Galois				*GF;

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
	Simulation_BCH(const t_simulation_param& simu_params,
			      const t_code_param& code_params,
			      const t_encoder_param& enco_params,
			      const t_channel_param& chan_params,
			      const t_decoder_param& deco_params);

	virtual ~Simulation_BCH();

	void launch();

protected:
	void build_communication_chain(const R& snr);
	void snr_loop();

protected:
	virtual void simulation_loop();
};

#endif /* SIMULATION_BCH_HPP_ */
