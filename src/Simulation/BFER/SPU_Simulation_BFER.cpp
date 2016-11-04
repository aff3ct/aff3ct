#ifdef STARPU

#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <fstream>

#include <starpu.h>

#include "Tools/Display/bash_tools.h"
#include "Tools/Display/Frame_trace/Frame_trace.hpp"
#include "Tools/Factory/Factory_terminal.hpp"

#include "Simulation_BFER.hpp"

template <typename B, typename R, typename Q>
Simulation_BFER<B,R,Q>
::Simulation_BFER(const parameters& params)
: Simulation_BFER_i<B,R,Q>(params),

  threads(params.simulation.n_threads -1),

  U_K (params.simulation.n_threads),
  X_N1(params.simulation.n_threads),
  X_N2(params.simulation.n_threads),
  X_N3(params.simulation.n_threads),
  H_N (params.simulation.n_threads),
  Y_N1(params.simulation.n_threads),
  Y_N2(params.simulation.n_threads),
  Y_N3(params.simulation.n_threads),
  Y_N4(params.simulation.n_threads),
  Y_N5(params.simulation.n_threads),
  V_K (params.simulation.n_threads),
  V_N (params.simulation.n_threads),

  monitor_red(nullptr),
  terminal   (nullptr),

  d_decod_total_fake(std::chrono::nanoseconds(0))
{
	auto ret = starpu_init(NULL);
	if (ret == -ENODEV)
		std::exit(77);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");
}

template <typename B, typename R, typename Q>
Simulation_BFER<B,R,Q>
::~Simulation_BFER()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::_launch()
{
	// launch a group of slave threads (there is "n_threads -1" slave threads)
	for (auto tid = 1; tid < this->params.simulation.n_threads; tid++)
		threads[tid -1] = std::thread(Simulation_BFER<B,R,Q>::Monte_Carlo_method, this, tid);

	// launch the master thread
	Simulation_BFER<B,R,Q>::Monte_Carlo_method(this, 0);

	// join the slave threads with the master thread
	for (auto tid = 1; tid < this->params.simulation.n_threads; tid++)
		threads[tid -1].join();

	if (!this->params.terminal.disabled && !this->params.simulation.benchs)
	{
		monitor_red->reduce();
		terminal->final_report(std::cout);
	}
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::release_objects()
{
	Simulation_BFER_i<B,R,Q>::release_objects();

	if (monitor_red != nullptr) { delete monitor_red; monitor_red = nullptr; }
	if (terminal    != nullptr) { delete terminal;    terminal    = nullptr; }
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::build_communication_chain(Simulation_BFER<B,R,Q> *simu, const int tid)
{
	Simulation_BFER_i<B,R,Q>::build_communication_chain(simu, tid);

	// get the real number of frames per threads (from the decoder)
	const auto n_fra = simu->decoder[tid]->get_n_frames();

	// resize the buffers
	const auto N      = simu->params.code.N;
	const auto K      = simu->params.code.K;
	const auto tail   = simu->params.code.tail_length;
	const auto N_mod  = simu->modulator[tid]->get_buffer_size_after_modulation(N + tail);
	const auto N_code = simu->params.code.N_code;
	const auto N_fil  = simu->modulator[tid]->get_buffer_size_after_filtering (N + tail);

	if (simu->U_K [tid].size() != (unsigned) ( K              * n_fra)) simu->U_K [tid].resize( K              * n_fra);
	if (simu->X_N1[tid].size() != (unsigned) ((N_code + tail) * n_fra)) simu->X_N1[tid].resize((N_code + tail) * n_fra);
	if (simu->X_N2[tid].size() != (unsigned) ((N      + tail) * n_fra)) simu->X_N2[tid].resize((N      + tail) * n_fra);
	if (simu->X_N3[tid].size() != (unsigned) ( N_mod          * n_fra)) simu->X_N3[tid].resize( N_mod          * n_fra);
	if (simu->Y_N1[tid].size() != (unsigned) ( N_mod          * n_fra)) simu->Y_N1[tid].resize( N_mod          * n_fra);
	if (simu->H_N [tid].size() != (unsigned) ( N_mod          * n_fra)) simu->H_N [tid].resize( N_mod          * n_fra);
	if (simu->Y_N2[tid].size() != (unsigned) ( N_fil          * n_fra)) simu->Y_N2[tid].resize( N_fil          * n_fra);
	if (simu->Y_N3[tid].size() != (unsigned) ((N      + tail) * n_fra)) simu->Y_N3[tid].resize((N      + tail) * n_fra);
	if (simu->Y_N4[tid].size() != (unsigned) ((N      + tail) * n_fra)) simu->Y_N4[tid].resize((N      + tail) * n_fra);
	if (simu->Y_N5[tid].size() != (unsigned) ((N_code + tail) * n_fra)) simu->Y_N5[tid].resize((N_code + tail) * n_fra);
	if (simu->V_K [tid].size() != (unsigned) ( K              * n_fra)) simu->V_K [tid].resize( K              * n_fra);
	if (simu->V_N [tid].size() != (unsigned) ((N_code + tail) * n_fra)) simu->V_N [tid].resize((N_code + tail) * n_fra);

	simu->barrier(tid);
	if (tid == 0)
	{
		// build a monitor to compute BER/FER (reduce the other monitors)
		simu->monitor_red = new Monitor_reduction<B>(simu->params.code.K,
		                                             simu->params.code.N,
		                                             simu->params.monitor.n_frame_errors,
		                                             simu->monitor,
		                                             n_fra);
		// build the terminal to display the BER/FER
		simu->terminal = simu->build_terminal(tid);
		Simulation::check_errors(simu->terminal, "Terminal");
	}
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::Monte_Carlo_method(Simulation_BFER<B,R,Q> *simu, const int tid)
{
	Simulation_BFER<B,R,Q>::build_communication_chain(simu, tid);

	if (tid == 0 && (!simu->params.terminal.disabled && simu->snr == simu->params.simulation.snr_min &&
	    !(simu->params.simulation.debug && simu->params.simulation.n_threads == 1) && !simu->params.simulation.benchs))
		simu->terminal->legend(std::cout);

	if (simu->params.source.type == "AZCW")
	{
		std::fill(simu->U_K [tid].begin(), simu->U_K [tid].end(), (B)0);
		std::fill(simu->X_N1[tid].begin(), simu->X_N1[tid].end(), (B)0);
		std::fill(simu->X_N2[tid].begin(), simu->X_N2[tid].end(), (B)0);
		simu->modulator[tid]->modulate(simu->X_N2[tid], simu->X_N3[tid]);
	}

	simu->barrier(tid);

	Simulation_BFER<B,R,Q>::simulation_loop(simu, tid);
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::simulation_loop(Simulation_BFER<B,R,Q> *simu, const int tid)
{
	using namespace std::chrono;

	int prev_n_fe = 0;

	// simulation loop
	while (!simu->monitor_red->fe_limit_achieved()) // while max frame error count has not been reached
	{
		if (simu->params.source.type != "AZCW")
		{
			simu->source   [tid]->generate(simu->U_K [tid]                 );
			simu->crc      [tid]->build   (simu->U_K [tid]                 );
			simu->encoder  [tid]->encode  (simu->U_K [tid], simu->X_N1[tid]);
			simu->puncturer[tid]->puncture(simu->X_N1[tid], simu->X_N2[tid]);
			simu->modulator[tid]->modulate(simu->X_N2[tid], simu->X_N3[tid]);
		}

		// Rayleigh channel
		if (simu->params.channel.type.find("RAYLEIGH") != std::string::npos)
		{
			simu->channel  [tid]->add_noise            (simu->X_N3[tid], simu->Y_N1[tid], simu->H_N [tid]);
			simu->modulator[tid]->filter               (simu->Y_N1[tid], simu->Y_N2[tid]                 );
			simu->modulator[tid]->demodulate_with_gains(simu->Y_N2[tid], simu->H_N [tid], simu->Y_N3[tid]);
		}
		else // additive channel (AWGN, USER, NO)
		{
			simu->channel  [tid]->add_noise (simu->X_N3[tid], simu->Y_N1[tid]);
			simu->modulator[tid]->filter    (simu->Y_N1[tid], simu->Y_N2[tid]);
			simu->modulator[tid]->demodulate(simu->Y_N2[tid], simu->Y_N3[tid]);
		}

		simu->quantizer[tid]->process   (simu->Y_N3[tid], simu->Y_N4[tid]);
		simu->puncturer[tid]->depuncture(simu->Y_N4[tid], simu->Y_N5[tid]);

		if (simu->params.code.coset)
			simu->coset_real[tid]->apply(simu->X_N1[tid], simu->Y_N5[tid], simu->Y_N5[tid]);

		simu->decoder[tid]->hard_decode(simu->Y_N5[tid], simu->V_K[tid]);

		if (simu->params.code.coset)
			simu->coset_bit[tid]->apply(simu->U_K[tid], simu->V_K[tid], simu->V_K[tid]);

		// check errors in the frame
		simu->monitor[tid]->check_errors(simu->U_K[tid], simu->V_K[tid]);

		// update the total number of frame errors if needed
		if (simu->monitor[tid]->get_n_fe() > prev_n_fe)
		{
			simu->monitor_red->increment_frame_errors(simu->monitor[tid]->get_n_fe() - prev_n_fe);
			prev_n_fe = simu->monitor[tid]->get_n_fe();
		}
	}
}

template <typename B, typename R, typename Q>
Terminal* Simulation_BFER<B,R,Q>
::build_terminal(const int tid)
{
	return Factory_terminal<B,R>::build(this->params, this->snr, monitor_red, this->t_snr, d_decod_total_fake);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_BFER<B_8,R_8,Q_8>;
template class Simulation_BFER<B_16,R_16,Q_16>;
template class Simulation_BFER<B_32,R_32,Q_32>;
template class Simulation_BFER<B_64,R_64,Q_64>;
#else
template class Simulation_BFER<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation

#endif /* STARPU */
