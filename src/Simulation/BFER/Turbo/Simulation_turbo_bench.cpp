#include <map>
#include <chrono>
#include <vector>
#include <string>
#include <utility>
#include <cstdlib>
#include <algorithm>

#include "../../../Tools/Factory/Factory_interleaver.hpp"
#include "../../../Tools/Factory/Factory_scaling_factor.hpp"
#include "../../../Tools/Factory/Factory_decoder_RSC.hpp"
#include "../../../Tools/Factory/Factory_decoder_turbo.hpp"

#include "../../../Tools/params.h"
#include "../../../Tools/bash_tools.h"
#include "../../../Tools/simu_tools.h"

#include "Simulation_turbo_bench.hpp"

template <typename B, typename R, typename Q, typename QD>
Simulation_turbo_bench<B,R,Q,QD>
::Simulation_turbo_bench(const t_simulation_param& simu_params,
                         const t_code_param&       code_params,
                         const t_encoder_param&    enco_params,
                         const t_channel_param&    chan_params,
                         const t_decoder_param&    deco_params,
                         const int n_threads)
: Simulation(),
  simu_params(simu_params),
  code_params(code_params),
  enco_params(enco_params),
  chan_params(chan_params),
  deco_params(deco_params),
  n_threads(n_threads),
  threads(n_threads -1),
  Y_N(n_threads),
  V_K(n_threads),
  interleaver(nullptr),
  sf(n_threads),
  decoder(n_threads),
  siso(n_threads),
  n_frames(1),
  barrier(n_threads)
{
	assert(code_params.N / code_params.K == 3);
	assert(n_threads > 0);

	// build the interleaver for the encoder and the decoder
	interleaver = Factory_interleaver<short>::build(code_params, code_params.K);
	check_errors(interleaver, "Interleaver<short>");

	for (auto tid = 0; tid < n_threads; tid++)
	{
		Y_N[tid].resize(code_params.N + code_params.tail_length);
		V_K[tid].resize(code_params.K);
	}
}

template <typename B, typename R, typename Q, typename QD>
Simulation_turbo_bench<B,R,Q,QD>
::~Simulation_turbo_bench()
{
	for (auto tid = 0; tid < n_threads; tid++) if (sf     [tid] != nullptr) delete sf     [tid];
	for (auto tid = 0; tid < n_threads; tid++) if (decoder[tid] != nullptr) delete decoder[tid];
	for (auto tid = 0; tid < n_threads; tid++) if (siso   [tid] != nullptr) delete siso   [tid];
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_turbo_bench<B,R,Q,QD>
::launch()
{
	this->bench();
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_turbo_bench<B,R,Q,QD>
::bench()
{
	// launch a group of slave threads (there is "n_threads -1" slave threads)
	for (auto tid = 1; tid < n_threads; tid++)
		threads[tid -1] = std::thread(Simulation_turbo_bench<B,R,Q,QD>::simulation_Monte_Carlo, this, tid);

	// launch the master thread
	Simulation_turbo_bench<B,R,Q,QD>::simulation_Monte_Carlo(this, 0);

	// join the slave threads with the master thread
	for (auto tid = 1; tid < n_threads; tid++)
		threads[tid -1].join();
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_turbo_bench<B,R,Q,QD>
::simulation_Monte_Carlo(Simulation_turbo_bench<B,R,Q,QD> *simu, const int tid)
{
	// manually build the trellis
	mipp::vector<mipp::vector<int>> trellis(4);
	trellis[0] = {0,4,5,1,2,6,7,3};
	trellis[1] = {4,0,1,5,6,2,3,7};
	trellis[2] = {0,1,1,0,0,1,1,0};
	trellis[3] = {0,0,1,1,1,1,0,0};

	// build the siso for the decoder
	simu->siso[tid] = Factory_decoder_RSC<B,Q,QD>::build_siso(simu->code_params, simu->enco_params, simu->chan_params,
	                                                          simu->deco_params, trellis);
	check_errors(simu->siso[tid], "SISO<Q>", tid);
	
	// build the scaling factor for the decoder
	simu->sf[tid] = Factory_scaling_factor<Q>::build(simu->code_params, simu->deco_params);
	check_errors(simu->sf[tid], "Scaling_factor<Q>", tid);

	// build the decoder
	simu->decoder[tid] = Factory_decoder_turbo<B,Q>::build(simu->code_params, simu->enco_params, simu->chan_params, 
	                                                       simu->deco_params, simu->interleaver, 
	                                                       simu->siso[tid], simu->siso[tid], simu->sf[tid]);
	check_errors(simu->decoder[tid], "Decoder<B,Q>");
	const auto n_fra = simu->decoder[tid]->get_n_frames();
	if (tid == 0)
	{ 
		simu->n_frames = n_fra;
		simu->interleaver->set_n_frames(n_fra);
	}

	// resize the buffers if needed
	if ((int)simu->Y_N[tid].size() != (simu->code_params.N * n_fra)) 
		simu->Y_N[tid].resize((simu->code_params.N + simu->code_params.tail_length) * n_fra);
	if ((int)simu->V_K [tid].size() != (simu->code_params.K * n_fra)) 
		simu->V_K [tid].resize( simu->code_params.K * n_fra);

	std::fill(simu->Y_N[tid].begin(), simu->Y_N[tid].end(), 0);

	simu->barrier(tid);

	// start time
	auto t_start = std::chrono::steady_clock::now();

	simu->barrier(tid);
	if (simu->simu_params.enable_dec_thr)
		for (auto i = 0; i < simu->simu_params.benchs; i++)
		{
			simu->decoder[tid]->decode();
		}
	else
		for (auto i = 0; i < simu->simu_params.benchs; i++)
		{
			simu->decoder[tid]->load(simu->Y_N[tid]);
			simu->decoder[tid]->decode();
			simu->decoder[tid]->store(simu->V_K[tid]);
		}
	simu->barrier(tid);

	// stop time
	auto t_stop = std::chrono::steady_clock::now();

	auto frames = (float)simu->simu_params.benchs * (float)simu->n_frames * (float)simu->n_threads;
	auto bits = (float)frames * (float)simu->code_params.K;
	auto duration = t_stop - t_start;

	auto bps = (float)bits / (float)(duration.count() * 0.000000001f);
	auto kbps = bps * 0.001f;
	auto mbps = kbps * 0.001f;

	auto latency = (float)duration.count() / (float)simu->simu_params.benchs;
	auto latency_us = latency * 0.001f;

	for (auto i = 0; i < simu->n_threads; i++)
	{
		if (tid == i)
			std::cout << " * information throughput = " << mbps << " Mbps, "
			          << "latency = " << latency_us << " us (tid = " << tid << ")" << std::endl;
		simu->barrier(tid);
	}
}

// ==================================================================================== explicit template instantiation 
#include "../../../Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_turbo_bench<B_8,R_8,Q_8,QD_8>;
template class Simulation_turbo_bench<B_16,R_16,Q_16,QD_16>;
template class Simulation_turbo_bench<B_32,R_32,Q_32,QD_32>;
template class Simulation_turbo_bench<B_64,R_64,Q_64,QD_64>;
#else
template class Simulation_turbo_bench<B,R,Q,QD>;
#endif
// ==================================================================================== explicit template instantiation