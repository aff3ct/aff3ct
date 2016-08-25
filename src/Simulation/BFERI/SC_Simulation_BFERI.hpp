#ifdef SYSTEMC

#ifndef SC_SIMULATION_BFERI_HPP_
#define SC_SIMULATION_BFERI_HPP_

#include <chrono>
#include <vector>
#include "../../Tools/MIPP/mipp.h"

#include "../Simulation.hpp"

#include "../../Source/Source.hpp"
#include "../../CRC/CRC.hpp"
#include "../../Encoder/Encoder.hpp"
#include "../../Modulator/Modulator.hpp"
#include "../../Channel/Channel.hpp"
#include "../../Quantizer/Quantizer.hpp"
#include "../../Interleaver/Interleaver.hpp"
#include "../../Decoder/SISO.hpp"
#include "../../Decoder/Decoder.hpp"
#include "../../Error/Error_analyzer.hpp"
#include "../../Terminal/Terminal.hpp"

#include "../../Tools/params.h"
#include "../../Tools/Threads/Barrier.hpp"

#include "../../Tools/SystemC/SC_Debug.hpp"
#include "../../Tools/SystemC/SC_Router.hpp"
#include "../../Tools/SystemC/SC_Predicate.hpp"
#include "../../Tools/SystemC/SC_Duplicator.hpp"

template <typename B, typename R, typename Q>
class Simulation_BFERI : public Simulation
{
protected:
	// simulation parameters
	const t_simulation_param &simu_params;
	const t_code_param       &code_params;
	const t_encoder_param    &enco_params;
	const t_mod_param        &mod_params;
	const t_channel_param    &chan_params;
	const t_decoder_param    &deco_params;

	Barrier barrier; // a barrier to synchronize the threads
	int n_frames;    // number of simulated frames per thread

	// code specifications
	float snr;
	float code_rate;
	float sigma;

	// communication chain
	std::vector<SC_Source<B>*>          source;
	std::vector<SC_CRC<B>*>             crc;
	std::vector<SC_Encoder<B>*>         encoder;
	            SC_Interleaver<int>    *interleaver_e;
	std::vector<SC_Modulator<B,R,Q>*>   modulator;
	std::vector<SC_Channel<R>*>         channel;
	std::vector<SC_Quantizer<R,Q>*>     quantizer;
	std::vector<SC_Interleaver<int>*>   interleaver;
	std::vector<SC_SISO<Q>*>            siso;
	std::vector<SC_Decoder<B,Q>*>       decoder;
	std::vector<SC_Error_analyzer<B>*>  analyzer;
	               Terminal            *terminal;

	SC_Duplicator *duplicator1;
	SC_Duplicator *duplicator2;
	SC_Router     *router;
	SC_Predicate  *predicate;

	SC_Debug<B> *dbg_B[5];
	SC_Debug<R> *dbg_R[3];
	SC_Debug<Q> *dbg_Q[5];

	// time points and durations
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_snr;
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_simu;
	std::chrono::nanoseconds d_snr;
	std::chrono::nanoseconds d_simu;

	std::chrono::nanoseconds d_decod_total_fake;

public:
	Simulation_BFERI(const t_simulation_param& simu_params,
	                 const t_code_param&       code_params,
	                 const t_encoder_param&    enco_params,
	                 const t_mod_param&        mod_params,
	                 const t_channel_param&    chan_params,
	                 const t_decoder_param&    deco_params);
	virtual ~Simulation_BFERI();
	void launch();

private:
	void launch_simulation        ();
	void build_communication_chain();
	void bind_sockets             ();
	void bind_sockets_debug       ();

	static void terminal_temp_report(Simulation_BFERI<B,R,Q> *simu);

protected:
	virtual void               release_objects  ();
	virtual void               launch_precompute();
	virtual void               snr_precompute   ();

	virtual Source<B>*         build_source     (                const int tid = 0);
	virtual CRC<B>*            build_crc        (                const int tid = 0);
	virtual Encoder<B>*        build_encoder    (                const int tid = 0) = 0;
	virtual Interleaver<int>*  build_interleaver(                const int tid = 0);
	virtual Modulator<B,R,Q>*  build_modulator  (                const int tid = 0);
	virtual Channel<R>*        build_channel    (const int size, const int tid = 0);
	virtual Quantizer<R,Q>*    build_quantizer  (const int size, const int tid = 0);
	virtual SISO<Q>*           build_siso       (                const int tid = 0) = 0;
	virtual Decoder<B,Q>*      build_decoder    (                const int tid = 0) = 0;
	virtual Error_analyzer<B>* build_analyzer   (                const int tid = 0);
	        Terminal*          build_terminal   (                const int tid = 0);
};

#endif /* SC_SIMULATION_BFERI_HPP_ */

#endif /* SYSTEMC */