#ifdef SYSTEMC

#ifndef SIMULATION_SC_BFER_HPP_
#define SIMULATION_SC_BFER_HPP_

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
#include "../../Terminal/Terminal.hpp"

#include "../../Tools/params.h"
#include "../../Tools/Threads/Barrier.hpp"

#include "../../Tools/SystemC/SC_Debug.hpp"
#include "../../Tools/SystemC/SC_Duplicator.hpp"

template <typename B, typename R, typename Q>
class Simulation_SC_BFER : public Simulation
{
protected:
	// simulation parameters
	const t_simulation_param &simu_params;
	const t_code_param       &code_params;
	const t_encoder_param    &enco_params;
	const t_mod_param        &mod_params;
	const t_channel_param    &chan_params;
	const t_decoder_param    &deco_params;

	int n_frames; // number of simulated frames per thread

	// code specifications
	float snr;
	float code_rate;
	float sigma;

	// communication chain
	SC_Source<B>         *source;
	SC_CRC<B>            *crc;
	SC_Encoder<B>        *encoder;
	SC_Puncturer<B,Q>    *puncturer;
	SC_Modulator<B,R>    *modulator;
	SC_Channel<R>        *channel;
	SC_Quantizer<R,Q>    *quantizer;
	SC_Decoder<B,Q>      *decoder;
	SC_Error_analyzer<B> *analyzer;
	Terminal             *terminal;

	SC_Duplicator<B> *duplicator;
	SC_Debug<B> *dbg_B[5];
	SC_Debug<R> *dbg_R[3];
	SC_Debug<Q> *dbg_Q[2];

	// time points and durations
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_snr;
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_simu;
	std::chrono::nanoseconds d_snr;
	std::chrono::nanoseconds d_simu;

	std::chrono::nanoseconds d_load_total_fake;
	std::chrono::nanoseconds d_decod_total_fake;
	std::chrono::nanoseconds d_store_total_fake;

public:
	Simulation_SC_BFER(const t_simulation_param& simu_params,
	                   const t_code_param&       code_params,
	                   const t_encoder_param&    enco_params,
	                   const t_mod_param&        mod_params,
	                   const t_channel_param&    chan_params,
	                   const t_decoder_param&    deco_params);
	virtual ~Simulation_SC_BFER();
	void launch();

private:
	void launch_simulation        ();
	void build_communication_chain();
	void bind_sockets             ();
	void bind_sockets_debug       ();

	static void terminal_temp_report(Simulation_SC_BFER<B,R,Q> *simu);

protected:
	virtual void               release_objects  ();
	virtual void               launch_precompute();
	virtual void               snr_precompute   ();

	virtual Source<B>*         build_source     ();
	virtual CRC<B>*            build_crc        ();
	virtual Encoder<B>*        build_encoder    () = 0;
	virtual Puncturer<B,Q>*    build_puncturer  ();
	virtual Modulator<B,R>*    build_modulator  ();
	virtual Channel<R>*        build_channel    ();
	virtual Quantizer<R,Q>*    build_quantizer  ();
	virtual Decoder<B,Q>*      build_decoder    () = 0;
	virtual Error_analyzer<B>* build_analyzer   ();
	        Terminal*          build_terminal   ();

};

#endif /* SIMULATION_SC_BFER_HPP_ */

#endif