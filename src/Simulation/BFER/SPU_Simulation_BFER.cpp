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
	/* Initialize StarPU with default configuration */
	auto ret = starpu_init(NULL);
	if (ret == -ENODEV)
		std::exit(77);
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
	this->Monte_Carlo_method(0);

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

	// this condition avoids the double unregistering...
	if (monitor_red != nullptr)
		/* StarPU does not need to manipulate the arrays anymore so we can stop
		 * monitoring them */
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
::build_communication_chain(const int tid)
{
	Simulation_BFER_i<B,R,Q>::build_communication_chain(this, tid);

	this->allocate_data(tid);

	this->barrier(tid);
	if (tid == 0)
	{
		// get the real number of frames per threads (from the decoder)
		const auto n_fra = this->decoder[tid]->get_n_frames();

		// build a monitor to compute BER/FER (reduce the other monitors)
		this->monitor_red = new Monitor_reduction<B>(this->params.code.K,
		                                             this->params.code.N,
		                                             this->params.monitor.n_frame_errors,
		                                             this->monitor,
		                                             n_fra);
		// build the terminal to display the BER/FER
		this->terminal = this->build_terminal(tid);
		Simulation::check_errors(this->terminal, "Terminal");
	}
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::allocate_data(const int tid)
{
	// get the real number of frames per threads (from the decoder)
	const auto n_fra = this->decoder[tid]->get_n_frames();

	// resize the buffers
	const auto N      = this->params.code.N;
	const auto K      = this->params.code.K;
	const auto tail   = this->params.code.tail_length;
	const auto N_mod  = this->modulator[tid]->get_buffer_size_after_modulation(N + tail);
	const auto N_code = this->params.code.N_code;
	const auto N_fil  = this->modulator[tid]->get_buffer_size_after_filtering (N + tail);

	if (this->U_K [tid].size() != (unsigned) ( K              * n_fra)) this->U_K [tid].resize( K              * n_fra);
	if (this->X_N1[tid].size() != (unsigned) ((N_code + tail) * n_fra)) this->X_N1[tid].resize((N_code + tail) * n_fra);
	if (this->X_N2[tid].size() != (unsigned) ((N      + tail) * n_fra)) this->X_N2[tid].resize((N      + tail) * n_fra);
	if (this->X_N3[tid].size() != (unsigned) ( N_mod          * n_fra)) this->X_N3[tid].resize( N_mod          * n_fra);
	if (this->Y_N1[tid].size() != (unsigned) ( N_mod          * n_fra)) this->Y_N1[tid].resize( N_mod          * n_fra);
	if (this->H_N [tid].size() != (unsigned) ( N_mod          * n_fra)) this->H_N [tid].resize( N_mod          * n_fra);
	if (this->Y_N2[tid].size() != (unsigned) ( N_fil          * n_fra)) this->Y_N2[tid].resize( N_fil          * n_fra);
	if (this->Y_N3[tid].size() != (unsigned) ((N      + tail) * n_fra)) this->Y_N3[tid].resize((N      + tail) * n_fra);
	if (this->Y_N4[tid].size() != (unsigned) ((N      + tail) * n_fra)) this->Y_N4[tid].resize((N      + tail) * n_fra);
	if (this->Y_N5[tid].size() != (unsigned) ((N_code + tail) * n_fra)) this->Y_N5[tid].resize((N_code + tail) * n_fra);
	if (this->V_K [tid].size() != (unsigned) ( K              * n_fra)) this->V_K [tid].resize( K              * n_fra);

	/* Tell StaPU to associate the "vector" vector with the "vector_handle"
	 * identifier. When a task needs to access a piece of data, it should
	 * refer to the handle that is associated to it.
	 * In the case of the "vector" data interface:
	 *  - the first argument of the registration method is a pointer to the
	 *    handle that should describe the data
	 *  - the second argument is the memory node where the data (ie. "vector")
	 *    resides initially: STARPU_MAIN_RAM stands for an address in main memory, as
	 *    opposed to an adress on a GPU for instance.
	 *  - the third argument is the adress of the vector in RAM
	 *  - the fourth argument is the number of elements in the vector
	 *  - the fifth argument is the size of each element.
	 */
	starpu_vector_data_register(&spu_U_K [tid], STARPU_MAIN_RAM, (uintptr_t)U_K [tid].data(), U_K [tid].size(), sizeof(B));
	starpu_vector_data_register(&spu_X_N1[tid], STARPU_MAIN_RAM, (uintptr_t)X_N1[tid].data(), X_N1[tid].size(), sizeof(B));
	starpu_vector_data_register(&spu_X_N2[tid], STARPU_MAIN_RAM, (uintptr_t)X_N2[tid].data(), X_N2[tid].size(), sizeof(B));
	starpu_vector_data_register(&spu_X_N3[tid], STARPU_MAIN_RAM, (uintptr_t)X_N3[tid].data(), X_N3[tid].size(), sizeof(R));
	starpu_vector_data_register(&spu_Y_N1[tid], STARPU_MAIN_RAM, (uintptr_t)Y_N1[tid].data(), Y_N1[tid].size(), sizeof(R));
	starpu_vector_data_register(&spu_H_N [tid], STARPU_MAIN_RAM, (uintptr_t)H_N [tid].data(), H_N [tid].size(), sizeof(R));
	starpu_vector_data_register(&spu_Y_N2[tid], STARPU_MAIN_RAM, (uintptr_t)Y_N2[tid].data(), Y_N2[tid].size(), sizeof(R));
	starpu_vector_data_register(&spu_Y_N3[tid], STARPU_MAIN_RAM, (uintptr_t)Y_N3[tid].data(), Y_N3[tid].size(), sizeof(R));
	starpu_vector_data_register(&spu_Y_N4[tid], STARPU_MAIN_RAM, (uintptr_t)Y_N4[tid].data(), Y_N4[tid].size(), sizeof(Q));
	starpu_vector_data_register(&spu_Y_N5[tid], STARPU_MAIN_RAM, (uintptr_t)Y_N5[tid].data(), Y_N5[tid].size(), sizeof(Q));
	starpu_vector_data_register(&spu_V_K [tid], STARPU_MAIN_RAM, (uintptr_t)V_K [tid].data(), V_K [tid].size(), sizeof(B));

	this->source    [tid]->spu_init();
	this->crc       [tid]->spu_init();
	this->encoder   [tid]->spu_init();
	this->puncturer [tid]->spu_init();
	this->modulator [tid]->spu_init();
	this->channel   [tid]->spu_init();
	this->quantizer [tid]->spu_init();
	this->coset_real[tid]->spu_init();
	this->decoder   [tid]->spu_init();
	this->coset_bit [tid]->spu_init();
	this->monitor   [tid]->spu_init();
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::Monte_Carlo_method(const int tid)
{
	Simulation_BFER<B,R,Q>::build_communication_chain(tid);

	if (tid == 0 && (!this->params.terminal.disabled && this->snr == this->params.simulation.snr_min &&
	    !(this->params.simulation.debug && this->params.simulation.n_threads == 1) && !this->params.simulation.benchs))
		this->terminal->legend(std::cout);

	if (this->params.source.type == "AZCW")
	{
		std::fill(this->U_K [tid].begin(), this->U_K [tid].end(), (B)0);
		std::fill(this->X_N1[tid].begin(), this->X_N1[tid].end(), (B)0);
		std::fill(this->X_N2[tid].begin(), this->X_N2[tid].end(), (B)0);
		this->modulator[tid]->modulate(this->X_N2[tid], this->X_N3[tid]);
	}

	this->barrier(tid);

//	Simulation_BFER<B,R,Q>::simulation_loop(tid);
	while (!this->monitor[tid]->fe_limit_achieved())
		Simulation_BFER<B,R,Q>::spu_create_tasks(tid);
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::spu_create_tasks(const int tid)
{
	starpu_task *task_gen_source = starpu_task_create();
	starpu_task *task_build_crc  = starpu_task_create();
	starpu_task *task_encode     = starpu_task_create();
	starpu_task *task_puncture   = starpu_task_create();
	starpu_task *task_modulate   = starpu_task_create();
	starpu_task *task_add_noise  = starpu_task_create();
	starpu_task *task_filter     = starpu_task_create();
	starpu_task *task_demodulate = starpu_task_create();
	starpu_task *task_quantize   = starpu_task_create();
	starpu_task *task_depuncture = starpu_task_create();
	starpu_task *task_decode     = starpu_task_create();
	starpu_task *task_check_err  = starpu_task_create();

	task_gen_source->cl = &Source   <B    >::spu_cl_generate;
	task_build_crc ->cl = &CRC      <B    >::spu_cl_build;
	task_encode    ->cl = &Encoder  <B    >::spu_cl_encode;
	task_puncture  ->cl = &Puncturer<B,  Q>::spu_cl_puncture;
	task_modulate  ->cl = &Modulator<B,R,Q>::spu_cl_modulate;
	task_add_noise ->cl = &Channel  <  R  >::spu_cl_add_noise;
	task_filter    ->cl = &Modulator<B,R,Q>::spu_cl_filter;
	task_demodulate->cl = &Modulator<B,R,Q>::spu_cl_demodulate;
	task_quantize  ->cl = &Quantizer<  R,Q>::spu_cl_process;
	task_depuncture->cl = &Puncturer<B,  Q>::spu_cl_depuncture;
	task_decode    ->cl = &Decoder  <B,  Q>::spu_cl_hard_decode;
	task_check_err ->cl = &Monitor  <B    >::spu_cl_check_errors;

	task_gen_source->cl_arg = (void*)(this->source   [tid]); task_gen_source->cl_arg_size = sizeof(*this->source   [tid]);
	task_build_crc ->cl_arg = (void*)(this->crc      [tid]); task_build_crc ->cl_arg_size = sizeof(*this->crc      [tid]);
	task_encode    ->cl_arg = (void*)(this->encoder  [tid]); task_encode    ->cl_arg_size = sizeof(*this->encoder  [tid]);
	task_puncture  ->cl_arg = (void*)(this->puncturer[tid]); task_puncture  ->cl_arg_size = sizeof(*this->puncturer[tid]);
	task_modulate  ->cl_arg = (void*)(this->modulator[tid]); task_modulate  ->cl_arg_size = sizeof(*this->modulator[tid]);
	task_add_noise ->cl_arg = (void*)(this->channel  [tid]); task_add_noise ->cl_arg_size = sizeof(*this->channel  [tid]);
	task_filter    ->cl_arg = (void*)(this->modulator[tid]); task_filter    ->cl_arg_size = sizeof(*this->modulator[tid]);
	task_demodulate->cl_arg = (void*)(this->modulator[tid]); task_demodulate->cl_arg_size = sizeof(*this->modulator[tid]);
	task_quantize  ->cl_arg = (void*)(this->quantizer[tid]); task_quantize  ->cl_arg_size = sizeof(*this->quantizer[tid]);
	task_depuncture->cl_arg = (void*)(this->puncturer[tid]); task_depuncture->cl_arg_size = sizeof(*this->puncturer[tid]);
	task_decode    ->cl_arg = (void*)(this->decoder  [tid]); task_decode    ->cl_arg_size = sizeof(*this->decoder  [tid]);
	task_check_err ->cl_arg = (void*)(this->monitor  [tid]); task_check_err ->cl_arg_size = sizeof(*this->monitor  [tid]);

	task_gen_source->handles[0] = spu_U_K [tid]; task_gen_source->modes[0] = STARPU_W;
	task_build_crc ->handles[0] = spu_U_K [tid]; task_build_crc ->modes[0] = STARPU_RW;
	task_encode    ->handles[0] = spu_U_K [tid]; task_encode    ->modes[0] = STARPU_R;
	task_encode    ->handles[1] = spu_X_N1[tid]; task_encode    ->modes[1] = STARPU_W;
	task_puncture  ->handles[0] = spu_X_N1[tid]; task_puncture  ->modes[0] = STARPU_R;
	task_puncture  ->handles[1] = spu_X_N2[tid]; task_puncture  ->modes[1] = STARPU_W;
	task_modulate  ->handles[0] = spu_X_N2[tid]; task_modulate  ->modes[0] = STARPU_R;
	task_modulate  ->handles[1] = spu_X_N3[tid]; task_modulate  ->modes[1] = STARPU_W;
	task_add_noise ->handles[0] = spu_X_N3[tid]; task_add_noise ->modes[0] = STARPU_R;
	task_add_noise ->handles[1] = spu_Y_N1[tid]; task_add_noise ->modes[1] = STARPU_W;
	task_filter    ->handles[0] = spu_Y_N1[tid]; task_filter    ->modes[0] = STARPU_R;
	task_filter    ->handles[1] = spu_Y_N2[tid]; task_filter    ->modes[1] = STARPU_W;
	task_demodulate->handles[0] = spu_Y_N2[tid]; task_demodulate->modes[0] = STARPU_R;
	task_demodulate->handles[1] = spu_Y_N3[tid]; task_demodulate->modes[1] = STARPU_W;
	task_quantize  ->handles[0] = spu_Y_N3[tid]; task_quantize  ->modes[0] = STARPU_R;
	task_quantize  ->handles[1] = spu_Y_N4[tid]; task_quantize  ->modes[1] = STARPU_W;
	task_depuncture->handles[0] = spu_Y_N4[tid]; task_depuncture->modes[0] = STARPU_R;
	task_depuncture->handles[1] = spu_Y_N5[tid]; task_depuncture->modes[1] = STARPU_W;
	task_decode    ->handles[0] = spu_Y_N5[tid]; task_decode    ->modes[0] = STARPU_R;
	task_decode    ->handles[1] = spu_V_K [tid]; task_decode    ->modes[1] = STARPU_W;
	task_check_err ->handles[0] = spu_U_K [tid]; task_check_err ->modes[0] = STARPU_R;
	task_check_err ->handles[1] = spu_V_K [tid]; task_check_err ->modes[0] = STARPU_R;

	STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_gen_source), "starpu_task_insert_00");
	STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_build_crc ), "starpu_task_insert_01");
	STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_encode    ), "starpu_task_insert_02");
	STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_puncture  ), "starpu_task_insert_03");
	STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_modulate  ), "starpu_task_insert_04");
	STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_add_noise ), "starpu_task_insert_05");
	STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_filter    ), "starpu_task_insert_06");
	STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_demodulate), "starpu_task_insert_07");
	STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_quantize  ), "starpu_task_insert_08");
	STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_depuncture), "starpu_task_insert_09");
	STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_decode    ), "starpu_task_insert_10");
	STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_check_err ), "starpu_task_insert_11");

	starpu_task_wait_for_all();
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::simulation_loop(const int tid)
{
	using namespace std::chrono;

	int prev_n_fe = 0;

	// simulation loop
	while (!this->monitor_red->fe_limit_achieved()) // while max frame error count has not been reached
	{
		if (this->params.source.type != "AZCW")
		{
			this->source   [tid]->generate(this->U_K [tid]                 );
			this->crc      [tid]->build   (this->U_K [tid]                 );
			this->encoder  [tid]->encode  (this->U_K [tid], this->X_N1[tid]);
			this->puncturer[tid]->puncture(this->X_N1[tid], this->X_N2[tid]);
			this->modulator[tid]->modulate(this->X_N2[tid], this->X_N3[tid]);
		}

		// Rayleigh channel
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos)
		{
			this->channel  [tid]->add_noise            (this->X_N3[tid], this->Y_N1[tid], this->H_N [tid]);
			this->modulator[tid]->filter               (this->Y_N1[tid], this->Y_N2[tid]                 );
			this->modulator[tid]->demodulate_with_gains(this->Y_N2[tid], this->H_N [tid], this->Y_N3[tid]);
		}
		else // additive channel (AWGN, USER, NO)
		{
			this->channel  [tid]->add_noise (this->X_N3[tid], this->Y_N1[tid]);
			this->modulator[tid]->filter    (this->Y_N1[tid], this->Y_N2[tid]);
			this->modulator[tid]->demodulate(this->Y_N2[tid], this->Y_N3[tid]);
		}

		this->quantizer[tid]->process   (this->Y_N3[tid], this->Y_N4[tid]);
		this->puncturer[tid]->depuncture(this->Y_N4[tid], this->Y_N5[tid]);

		if (this->params.code.coset)
			this->coset_real[tid]->apply(this->X_N1[tid], this->Y_N5[tid], this->Y_N5[tid]);

		this->decoder[tid]->hard_decode(this->Y_N5[tid], this->V_K[tid]);

		if (this->params.code.coset)
			this->coset_bit[tid]->apply(this->U_K[tid], this->V_K[tid], this->V_K[tid]);

		// check errors in the frame
		this->monitor[tid]->check_errors(this->U_K[tid], this->V_K[tid]);

		// update the total number of frame errors if needed
		if (this->monitor[tid]->get_n_fe() > prev_n_fe)
		{
			this->monitor_red->increment_frame_errors(this->monitor[tid]->get_n_fe() - prev_n_fe);
			prev_n_fe = this->monitor[tid]->get_n_fe();
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
