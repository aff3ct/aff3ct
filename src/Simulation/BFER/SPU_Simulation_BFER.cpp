#ifdef STARPU

#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "Tools/Display/bash_tools.h"
#include "Tools/Display/Frame_trace/Frame_trace.hpp"
#include "Tools/Factory/Factory_terminal.hpp"

#include "Simulation_BFER.hpp"

template <typename B, typename R, typename Q>
Simulation_BFER<B,R,Q>
::Simulation_BFER(const parameters& params)
: Simulation_BFER_i<B,R,Q>(params),

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

  spu_U_K (params.simulation.n_threads),
  spu_X_N1(params.simulation.n_threads),
  spu_X_N2(params.simulation.n_threads),
  spu_X_N3(params.simulation.n_threads),
  spu_H_N (params.simulation.n_threads),
  spu_Y_N1(params.simulation.n_threads),
  spu_Y_N2(params.simulation.n_threads),
  spu_Y_N3(params.simulation.n_threads),
  spu_Y_N4(params.simulation.n_threads),
  spu_Y_N5(params.simulation.n_threads),
  spu_V_K (params.simulation.n_threads),

  monitor_red(nullptr),
  terminal   (nullptr),

  d_decod_total_fake(std::chrono::nanoseconds(0))
{
	// initialize StarPU with default configuration
	auto ret = starpu_init(NULL);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");
}

template <typename B, typename R, typename Q>
Simulation_BFER<B,R,Q>
::~Simulation_BFER()
{
	/* terminate StarPU, no task can be submitted after */
	starpu_shutdown();
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::_launch()
{
	// launch the master thread
	this->Monte_Carlo_method();

	if (!this->params.terminal.disabled && !this->params.simulation.benchs)
		terminal->final_report(std::cout);
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::release_objects()
{
	Simulation_BFER_i<B,R,Q>::release_objects();

	// this condition avoids the double unregistering...
	if (monitor_red != nullptr)
		// StarPU does not need to manipulate the arrays anymore so we can stop monitoring them
		for (auto tid = 0; tid < this->params.simulation.n_threads; tid++)
		{
			starpu_data_unregister(spu_U_K [tid]);
			starpu_data_unregister(spu_X_N1[tid]);
			starpu_data_unregister(spu_X_N2[tid]);
			starpu_data_unregister(spu_X_N3[tid]);
			starpu_data_unregister(spu_Y_N1[tid]);
			starpu_data_unregister(spu_H_N [tid]);
			starpu_data_unregister(spu_Y_N2[tid]);
			starpu_data_unregister(spu_Y_N3[tid]);
			starpu_data_unregister(spu_Y_N4[tid]);
			starpu_data_unregister(spu_Y_N5[tid]);
			starpu_data_unregister(spu_V_K [tid]);
		}

	if (monitor_red != nullptr) { delete monitor_red; monitor_red = nullptr; }
	if (terminal    != nullptr) { delete terminal;    terminal    = nullptr; }
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::build_communication_chain(Simulation_BFER<B,R,Q> *simu, const int tid)
{
	Simulation_BFER_i<B,R,Q>::build_communication_chain(simu, tid);
	Simulation_BFER  <B,R,Q>::allocate_data            (simu, tid);

	simu->barrier(tid);
	if (tid == 0)
	{
		// get the real number of frames per threads (from the decoder)
		const auto n_fra = simu->decoder[tid]->get_n_frames();

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
::allocate_data(Simulation_BFER<B,R,Q> *simu, const int tid)
{
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

	// Tell StaPU to associate the "vector" vector with the "vector_handle"
	// identifier. When a task needs to access a piece of data, it should
	// refer to the handle that is associated to it.
	// In the case of the "vector" data interface:
	//  - the first argument of the registration method is a pointer to the
	//    handle that should describe the data
	//  - the second argument is the memory node where the data (ie. "vector")
	//    resides initially: STARPU_MAIN_RAM stands for an address in main memory, as
	//    opposed to an adress on a GPU for instance.
	//  - the third argument is the adress of the vector in RAM
	//  - the fourth argument is the number of elements in the vector
	//  - the fifth argument is the size of each element.
	starpu_vector_data_register(&simu->spu_U_K [tid], STARPU_MAIN_RAM, (uintptr_t)&simu->U_K [tid], 1, sizeof(simu->U_K [tid]));
	starpu_vector_data_register(&simu->spu_X_N1[tid], STARPU_MAIN_RAM, (uintptr_t)&simu->X_N1[tid], 1, sizeof(simu->X_N1[tid]));
	starpu_vector_data_register(&simu->spu_X_N2[tid], STARPU_MAIN_RAM, (uintptr_t)&simu->X_N2[tid], 1, sizeof(simu->X_N2[tid]));
	starpu_vector_data_register(&simu->spu_X_N3[tid], STARPU_MAIN_RAM, (uintptr_t)&simu->X_N3[tid], 1, sizeof(simu->X_N3[tid]));
	starpu_vector_data_register(&simu->spu_Y_N1[tid], STARPU_MAIN_RAM, (uintptr_t)&simu->Y_N1[tid], 1, sizeof(simu->Y_N1[tid]));
	starpu_vector_data_register(&simu->spu_H_N [tid], STARPU_MAIN_RAM, (uintptr_t)&simu->H_N [tid], 1, sizeof(simu->H_N [tid]));
	starpu_vector_data_register(&simu->spu_Y_N2[tid], STARPU_MAIN_RAM, (uintptr_t)&simu->Y_N2[tid], 1, sizeof(simu->Y_N2[tid]));
	starpu_vector_data_register(&simu->spu_Y_N3[tid], STARPU_MAIN_RAM, (uintptr_t)&simu->Y_N3[tid], 1, sizeof(simu->Y_N3[tid]));
	starpu_vector_data_register(&simu->spu_Y_N4[tid], STARPU_MAIN_RAM, (uintptr_t)&simu->Y_N4[tid], 1, sizeof(simu->Y_N4[tid]));
	starpu_vector_data_register(&simu->spu_Y_N5[tid], STARPU_MAIN_RAM, (uintptr_t)&simu->Y_N5[tid], 1, sizeof(simu->Y_N5[tid]));
	starpu_vector_data_register(&simu->spu_V_K [tid], STARPU_MAIN_RAM, (uintptr_t)&simu->V_K [tid], 1, sizeof(simu->V_K [tid]));
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::Monte_Carlo_method()
{
	// launch a group of slave threads (there is "n_threads -1" slave threads)
	std::vector<std::thread> threads(this->params.simulation.n_threads -1);
	for (auto tid = 1; tid < this->params.simulation.n_threads; tid++)
		threads[tid -1] = std::thread(Simulation_BFER<B,R,Q>::build_communication_chain, this, tid);

	// build the communication chain
	Simulation_BFER<B,R,Q>::build_communication_chain(this, 0);

	// join the slave threads with the master thread
	for (auto tid = 1; tid < this->params.simulation.n_threads; tid++)
		threads[tid -1].join();

	if (!this->params.terminal.disabled && this->snr == this->params.simulation.snr_min &&
	    !(this->params.simulation.debug && this->params.simulation.n_threads == 1) && !this->params.simulation.benchs)
		this->terminal->legend(std::cout);

	if (this->params.source.type == "AZCW")
		for (auto tid = 0; tid < this->params.simulation.n_threads; tid++)
		{
			std::fill(this->U_K [tid].begin(), this->U_K [tid].end(), (B)0);
			std::fill(this->X_N1[tid].begin(), this->X_N1[tid].end(), (B)0);
			std::fill(this->X_N2[tid].begin(), this->X_N2[tid].end(), (B)0);
			this->modulator[tid]->modulate(this->X_N2[tid], this->X_N3[tid]);
		}

	std::thread a_thread;
	if (!this->params.terminal.disabled && this->params.terminal.frequency != std::chrono::nanoseconds(0))
		// launch a thread dedicated to the terminal display
		a_thread = std::thread(Simulation_BFER<B,R,Q>::terminal_temp_report, this);

	// Monte Carlo simulation
	while (!this->monitor_red->fe_limit_achieved())
	{
		for (auto tid = 0; tid < this->params.simulation.n_threads; tid++)
			Simulation_BFER<B,R,Q>::spu_create_tasks(tid);

		starpu_task_wait_for_all();
	}

	if (!this->params.terminal.disabled && this->params.terminal.frequency != std::chrono::nanoseconds(0))
		// wait the terminal thread to finish
		a_thread.join();
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::spu_create_tasks(const int tid)
{
	if (this->params.source.type != "AZCW")
	{
		auto task_gen_source = Source   <B    >::spu_task_generate(this->source   [tid], spu_U_K [tid]);
		auto task_build_crc  = CRC      <B    >::spu_task_build   (this->crc      [tid], spu_U_K [tid]);
		auto task_encode     = Encoder  <B    >::spu_task_encode  (this->encoder  [tid], spu_U_K [tid], spu_X_N1[tid]);
		auto task_puncture   = Puncturer<B,  Q>::spu_task_puncture(this->puncturer[tid], spu_X_N1[tid], spu_X_N2[tid]);
		auto task_modulate   = Modulator<B,R,R>::spu_task_modulate(this->modulator[tid], spu_X_N2[tid], spu_X_N3[tid]);

		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_gen_source), "starpu_task_insert_gen_source");
		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_build_crc ), "starpu_task_insert_build_crc");
		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_encode    ), "starpu_task_insert_encode");
		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_puncture  ), "starpu_task_insert_puncture");
		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_modulate  ), "starpu_task_insert_modulate");
	}

	// Rayleigh channel
	if (this->params.channel.type.find("RAYLEIGH") != std::string::npos)
	{
		auto task_add_noise_wg  = Channel  <  R  >::spu_task_add_noise_wg (this->channel  [tid], spu_X_N3[tid], spu_Y_N1[tid], spu_H_N [tid]);
		auto task_filter        = Modulator<B,R,R>::spu_task_filter       (this->modulator[tid], spu_Y_N1[tid], spu_Y_N2[tid]               );
		auto task_demodulate_wg = Modulator<B,R,R>::spu_task_demodulate_wg(this->modulator[tid], spu_Y_N2[tid], spu_H_N [tid], spu_Y_N3[tid]);

		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_add_noise_wg ), "starpu_task_insert_add_noise_wg");
		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_filter       ), "starpu_task_insert_filter");
		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_demodulate_wg), "starpu_task_insert_demodulate_wg");
	}
	else // additive channel (AWGN, USER, NO)
	{
		auto task_add_noise  = Channel  <  R  >::spu_task_add_noise (this->channel  [tid], spu_X_N3[tid], spu_Y_N1[tid]);
		auto task_filter     = Modulator<B,R,R>::spu_task_filter    (this->modulator[tid], spu_Y_N1[tid], spu_Y_N2[tid]);
		auto task_demodulate = Modulator<B,R,R>::spu_task_demodulate(this->modulator[tid], spu_Y_N2[tid], spu_Y_N3[tid]);

		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_add_noise ), "starpu_task_insert_add_noise");
		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_filter    ), "starpu_task_insert_filter");
		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_demodulate), "starpu_task_insert_demodulate");
	}

	auto task_quantize   = Quantizer<  R,Q>::spu_task_process   (this->quantizer[tid], spu_Y_N3[tid], spu_Y_N4[tid]);
	auto task_depuncture = Puncturer<B,  Q>::spu_task_depuncture(this->puncturer[tid], spu_Y_N4[tid], spu_Y_N5[tid]);

	STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_quantize  ), "starpu_task_insert_quantize");
	STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_depuncture), "starpu_task_insert_depuncture");

	if (this->params.code.coset)
	{
		auto task_coset_real = Coset<B,Q>::spu_task_apply(this->coset_real[tid], spu_X_N1[tid], spu_Y_N5[tid], spu_Y_N5[tid]);
		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_coset_real), "starpu_task_insert_coset_real");
	}

	auto task_decode = Decoder<B,Q>::spu_task_hard_decode(this->decoder[tid], spu_Y_N5[tid], spu_V_K[tid]);
	STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_decode), "starpu_task_insert_decode");

	if (this->params.code.coset)
	{
		auto task_coset_bit = Coset<B,B>::spu_task_apply(this->coset_bit[tid], spu_U_K[tid], spu_V_K[tid], spu_V_K[tid]);
		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_coset_bit), "starpu_task_insert_coset_bit");
	}

	auto task_check_err = Monitor<B>::spu_task_check_errors(this->monitor[tid], spu_U_K[tid], spu_V_K[tid]);
	STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_check_err ), "starpu_task_insert_check_err");
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::terminal_temp_report(Simulation_BFER<B,R,Q> *simu)
{
	while (!simu->monitor_red->fe_limit_achieved() && !simu->monitor_red->is_interrupt())
	{
		const auto sleep_time = simu->params.terminal.frequency - std::chrono::milliseconds(0);
		std::this_thread::sleep_for(sleep_time);

		// display statistics in terminal
		simu->terminal->temp_report(std::clog);
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
