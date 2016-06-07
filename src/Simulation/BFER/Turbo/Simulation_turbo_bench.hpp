#ifndef SIMULATION_TURBO_BENCH_HPP_
#define SIMULATION_TURBO_BENCH_HPP_

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
#include "../../../Tools/Turbo/Scaling_factor/Scaling_factor.hpp"
#include "../../../Decoder/Decoder.hpp"
#include "../../../Decoder/SISO.hpp"
#include "../../../Tools/Threads/Barrier.hpp"
#include "../../../Tools/params.h"

template <typename B, typename R, typename Q, typename QD>
class Simulation_turbo_bench : public Simulation
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
	std::vector<mipp::vector<Q>> Y_N; // noisy codeword
	std::vector<mipp::vector<B>> V_K; // decoded codeword

	Interleaver<short>* interleaver;

	// communication chain objects
	std::vector<Scaling_factor<Q>*> sf;
	std::vector<Decoder<B,Q>*>      decoder;
	std::vector<SISO<Q>*>           siso;

	// number of frames to encode/decode per encoder/decoder instance in the simulation loop
	int n_frames;

	Barrier barrier;

public:
	Simulation_turbo_bench(const t_simulation_param& simu_params,
	                       const t_code_param&       code_params,
	                       const t_encoder_param&    enco_params,
	                       const t_channel_param&    chan_params,
	                       const t_decoder_param&    deco_params,
	                       const int n_threads = 1);

	virtual ~Simulation_turbo_bench();

	void launch();

protected:
	void bench();
	static void simulation_Monte_Carlo(Simulation_turbo_bench<B,R,Q,QD> *simu, const int tid);
};

#endif /* SIMULATION_TURBO_BENCH_HPP_ */