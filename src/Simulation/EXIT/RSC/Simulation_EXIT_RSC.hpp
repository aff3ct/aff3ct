#ifndef SIMULATION_EXIT_RSC_HPP_
#define SIMULATION_EXIT_RSC_HPP_

#include <chrono>
#include <vector>
#include "../../../Tools/MIPP/mipp.h"

#include "../Simulation_EXIT.hpp"

#include "../../../Source/Source.hpp"
#include "../../../Encoder/Encoder.hpp"
#include "../../../Modulator/Modulator.hpp"
#include "../../../Channel/Channel.hpp"
#include "../../../Quantizer/Quantizer.hpp"
#include "../../../Decoder/SISO.hpp"
#include "../../../Terminal/Terminal_EXIT.hpp"
#include "../../../Tools/params.h"

template <typename B, typename R, typename Q, typename QD>
class Simulation_EXIT_RSC : public Simulation_EXIT<B,Q>
{
protected:
	const int& N;
	const int& K;

	// simulation parameters
	const t_simulation_param simu_params;
	const t_code_param       code_params;
	const t_encoder_param    enco_params;
	const t_channel_param    chan_params;
	const t_decoder_param    deco_params;
	
	// data vectors
	mipp::vector<B> B_K, B_N, X_K, X_N;	
	mipp::vector<R> Y_N, Y_K;
	mipp::vector<R> La_K1;
	mipp::vector<R> Lch_N1;
	mipp::vector<Q> La_K2;
	mipp::vector<Q> Lch_N2;
	mipp::vector<Q> Le_K;
	mipp::vector<Q> sys, par;
	mipp::vector<B> B_buff;
	mipp::vector<Q> Le_buff, La_buff;

	//EXIT simu parameters
	const int n_trials;
	int cur_trial;
	double I_A, I_E;

	// code specifications
	float sig_a;
	float code_rate;
	float sigma;

	// time points and durations
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_snr;

	// communication chain
	Source<B>          *source;
	Encoder<B>         *encoder;
	Modulator<B>       *modulator;	
	Channel<B,R>       *channel;
	Channel<B,R>       *channel_a;
	Quantizer<R,Q>     *quantizer;
	SISO<Q>            *siso;
	Terminal_EXIT<B,R> *terminal;
	
public:
	Simulation_EXIT_RSC(const t_simulation_param& simu_params,
	                    const t_code_param&       code_params,
	                    const t_encoder_param&    enco_params,
	                    const t_channel_param&    chan_params,
	                    const t_decoder_param&    deco_params);

	virtual ~Simulation_EXIT_RSC();

	void launch();

protected:
	void build_communication_chain(const R& snr);
	void snr_loop();

protected:
	virtual void simulation_loop();
	
private:
	void store_results();
	void analyze_results();
};

#endif /* SIMULATION_EXIT_RSC_HPP_ */