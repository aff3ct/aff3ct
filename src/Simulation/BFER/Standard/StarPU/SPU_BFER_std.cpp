#ifdef STARPU

#include <iostream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/bash_tools.h"

#include "SPU_BFER_std.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
SPU_BFER_std<B,R,Q>
::SPU_BFER_std(const factory::BFER_std::parameters &params, tools::Codec<B,Q> &codec)
: BFER_std<B,R,Q>(params, codec),

  task_names(this->params.n_threads, std::vector<std::string>(15)),
  frame_id(0),

  U_K1(this->params.n_threads, mipp::vector<B>(this->params.src->K     * this->params.src->n_frames)),
  U_K2(this->params.n_threads, mipp::vector<B>(this->params.enc->K     * this->params.enc->n_frames)),
  X_N1(this->params.n_threads, mipp::vector<B>(this->params.enc->N_cw  * this->params.enc->n_frames)),
  X_N2(this->params.n_threads, mipp::vector<B>(this->params.pct->N     * this->params.pct->n_frames)),
  X_N3(this->params.n_threads, mipp::vector<R>(this->params.mdm->N_mod * this->params.mdm->n_frames)),
  H_N (this->params.n_threads, mipp::vector<R>(this->params.chn->N     * this->params.chn->n_frames)),
  Y_N1(this->params.n_threads, mipp::vector<R>(this->params.chn->N     * this->params.chn->n_frames)),
  Y_N2(this->params.n_threads, mipp::vector<R>(this->params.mdm->N_fil * this->params.mdm->n_frames)),
  Y_N3(this->params.n_threads, mipp::vector<R>(this->params.mdm->N     * this->params.mdm->n_frames)),
  Y_N4(this->params.n_threads, mipp::vector<Q>(this->params.qnt->size  * this->params.qnt->n_frames)),
  Y_N5(this->params.n_threads, mipp::vector<Q>(this->params.pct->N_cw  * this->params.pct->n_frames)),
  V_K1(this->params.n_threads, mipp::vector<B>(this->params.dec->K     * this->params.dec->n_frames)),
  V_K2(this->params.n_threads, mipp::vector<B>(this->params.crc->K     * this->params.crc->n_frames)),

  spu_U_K1(this->params.n_threads),
  spu_U_K2(this->params.n_threads),
  spu_X_N1(this->params.n_threads),
  spu_X_N2(this->params.n_threads),
  spu_X_N3(this->params.n_threads),
  spu_H_N (this->params.n_threads),
  spu_Y_N1(this->params.n_threads),
  spu_Y_N2(this->params.n_threads),
  spu_Y_N3(this->params.n_threads),
  spu_Y_N4(this->params.n_threads),
  spu_Y_N5(this->params.n_threads),
  spu_V_K1(this->params.n_threads),
  spu_V_K2(this->params.n_threads)
{
	if (params.debug)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "StarPU simulation does not support the debug "
		                                                            "mode.");

	if (params.benchs)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "StarPU simulation does not support the bench "
		                                                            "mode.");

	if (params.coded_monitoring)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "StarPU simulation does not support the coded "
		                                                            "monitoring.");

	if (params.time_report)
		std::clog << tools::format_warning("The time report is not available in the StarPU simulation.") << std::endl;

	// initialize StarPU with default configuration
	auto ret = starpu_init(NULL);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

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
	for (auto tid = 0; tid < this->params.n_threads; tid++)
	{
		starpu_vector_data_register(&spu_U_K1[tid], STARPU_MAIN_RAM, (uintptr_t)U_K1[tid].data(), U_K1[tid].size(), sizeof(B));
		starpu_vector_data_register(&spu_U_K2[tid], STARPU_MAIN_RAM, (uintptr_t)U_K2[tid].data(), U_K2[tid].size(), sizeof(B));
		starpu_vector_data_register(&spu_X_N1[tid], STARPU_MAIN_RAM, (uintptr_t)X_N1[tid].data(), X_N1[tid].size(), sizeof(B));
		starpu_vector_data_register(&spu_X_N2[tid], STARPU_MAIN_RAM, (uintptr_t)X_N2[tid].data(), X_N2[tid].size(), sizeof(B));
		starpu_vector_data_register(&spu_X_N3[tid], STARPU_MAIN_RAM, (uintptr_t)X_N3[tid].data(), X_N3[tid].size(), sizeof(R));
		starpu_vector_data_register(&spu_Y_N1[tid], STARPU_MAIN_RAM, (uintptr_t)Y_N1[tid].data(), Y_N1[tid].size(), sizeof(R));
		starpu_vector_data_register(&spu_H_N [tid], STARPU_MAIN_RAM, (uintptr_t)H_N [tid].data(), H_N [tid].size(), sizeof(R));
		starpu_vector_data_register(&spu_Y_N2[tid], STARPU_MAIN_RAM, (uintptr_t)Y_N2[tid].data(), Y_N2[tid].size(), sizeof(R));
		starpu_vector_data_register(&spu_Y_N3[tid], STARPU_MAIN_RAM, (uintptr_t)Y_N3[tid].data(), Y_N3[tid].size(), sizeof(R));
		starpu_vector_data_register(&spu_Y_N4[tid], STARPU_MAIN_RAM, (uintptr_t)Y_N4[tid].data(), Y_N4[tid].size(), sizeof(Q));
		starpu_vector_data_register(&spu_Y_N5[tid], STARPU_MAIN_RAM, (uintptr_t)Y_N5[tid].data(), Y_N5[tid].size(), sizeof(Q));
		starpu_vector_data_register(&spu_V_K1[tid], STARPU_MAIN_RAM, (uintptr_t)V_K1[tid].data(), V_K1[tid].size(), sizeof(B));
		starpu_vector_data_register(&spu_V_K2[tid], STARPU_MAIN_RAM, (uintptr_t)V_K2[tid].data(), V_K2[tid].size(), sizeof(B));

		starpu_data_set_user_data(spu_U_K1[tid], (void*)&U_K1[tid]);
		starpu_data_set_user_data(spu_U_K2[tid], (void*)&U_K2[tid]);
		starpu_data_set_user_data(spu_X_N1[tid], (void*)&X_N1[tid]);
		starpu_data_set_user_data(spu_X_N2[tid], (void*)&X_N2[tid]);
		starpu_data_set_user_data(spu_X_N3[tid], (void*)&X_N3[tid]);
		starpu_data_set_user_data(spu_Y_N1[tid], (void*)&Y_N1[tid]);
		starpu_data_set_user_data(spu_H_N [tid], (void*)&H_N [tid]);
		starpu_data_set_user_data(spu_Y_N2[tid], (void*)&Y_N2[tid]);
		starpu_data_set_user_data(spu_Y_N3[tid], (void*)&Y_N3[tid]);
		starpu_data_set_user_data(spu_Y_N4[tid], (void*)&Y_N4[tid]);
		starpu_data_set_user_data(spu_Y_N5[tid], (void*)&Y_N5[tid]);
		starpu_data_set_user_data(spu_V_K1[tid], (void*)&V_K1[tid]);
		starpu_data_set_user_data(spu_V_K2[tid], (void*)&V_K2[tid]);
	}
}

template <typename B, typename R, typename Q>
SPU_BFER_std<B,R,Q>
::~SPU_BFER_std()
{
	// StarPU does not need to manipulate the arrays anymore so we can stop monitoring them
	for (auto tid = 0; tid < this->params.n_threads; tid++)
	{
		starpu_data_unregister(spu_U_K1[tid]);
		starpu_data_unregister(spu_U_K2[tid]);
		starpu_data_unregister(spu_X_N1[tid]);
		starpu_data_unregister(spu_X_N2[tid]);
		starpu_data_unregister(spu_X_N3[tid]);
		starpu_data_unregister(spu_Y_N1[tid]);
		starpu_data_unregister(spu_H_N [tid]);
		starpu_data_unregister(spu_Y_N2[tid]);
		starpu_data_unregister(spu_Y_N3[tid]);
		starpu_data_unregister(spu_Y_N4[tid]);
		starpu_data_unregister(spu_Y_N5[tid]);
		starpu_data_unregister(spu_V_K1[tid]);
		starpu_data_unregister(spu_V_K2[tid]);
	}

	/* terminate StarPU, no task can be submitted after */
	starpu_shutdown();
}

template <typename B, typename R, typename Q>
void SPU_BFER_std<B,R,Q>
::_build_communication_chain(const int tid)
{
	BFER_std<B,R,Q>::_build_communication_chain(tid);

	if (this->params.src->type == "AZCW")
	{
		std::fill(this->U_K1[tid].begin(), this->U_K1[tid].end(), (B)0);
		std::fill(this->U_K2[tid].begin(), this->U_K2[tid].end(), (B)0);
		std::fill(this->X_N1[tid].begin(), this->X_N1[tid].end(), (B)0);
		std::fill(this->X_N2[tid].begin(), this->X_N2[tid].end(), (B)0);
		this->modem[tid]->modulate(this->X_N2[tid], this->X_N3[tid]);
	}

	if (this->params.err_track_enable)
	{
		if (this->params.src->type != "AZCW")
			this->dumper[tid]->register_data(U_K1[tid], "src", false, {});

		if (this->params.coset)
			this->dumper[tid]->register_data(X_N1[tid], "enc", false, {(unsigned)this->params.enc->K});

		this->dumper[tid]->register_data(this->channel[tid]->get_noise(), "chn", true, {});

		if (this->interleaver[tid] != nullptr && this->interleaver[tid]->is_uniform())
			this->dumper[tid]->register_data(this->interleaver[tid]->get_lut(), "itl", false, {});
	}
}

template <typename B, typename R, typename Q>
void SPU_BFER_std<B,R,Q>
::_launch()
{
	// Monte Carlo simulation
	while (!this->monitor_red->fe_limit_achieved() &&
	       (this->monitor_red->get_n_analyzed_fra() < this->max_fra || this->max_fra == 0))
	{
		for (auto tid = 0; tid < this->params.n_threads; tid++)
			this->seq_tasks_submission(tid);

		starpu_task_wait_for_all();
	}
}

template <typename B, typename R, typename Q>
void SPU_BFER_std<B,R,Q>
::seq_tasks_submission(const int tid)
{
	const std::string str_id = std::to_string(frame_id);

	if (this->params.src->type != "AZCW")
	{
		auto task_gen_source = module::Source   <B    >::spu_task_generate(this->source   [tid], spu_U_K1[tid]               );
		auto task_build_crc  = module::CRC      <B    >::spu_task_build   (this->crc      [tid], spu_U_K1[tid], spu_U_K2[tid]);
		auto task_encode     = module::Encoder  <B    >::spu_task_encode  (this->encoder  [tid], spu_U_K2[tid], spu_X_N1[tid]);
		auto task_puncture   = module::Puncturer<B,  Q>::spu_task_puncture(this->puncturer[tid], spu_X_N1[tid], spu_X_N2[tid]);
		auto task_modulate   = module::Modem    <B,R,R>::spu_task_modulate(this->modem    [tid], spu_X_N2[tid], spu_X_N3[tid]);

		task_gen_source->priority = STARPU_MIN_PRIO +0;
		task_build_crc ->priority = STARPU_MIN_PRIO +1;
		task_encode    ->priority = STARPU_MIN_PRIO +2;
		task_puncture  ->priority = STARPU_MIN_PRIO +3;
		task_modulate  ->priority = STARPU_MIN_PRIO +4;

		task_names[tid][0] = "src::generate_" + str_id; task_gen_source->name = task_names[tid][0].c_str();
		task_names[tid][1] = "crc::build_"    + str_id; task_build_crc ->name = task_names[tid][1].c_str();
		task_names[tid][2] = "enc::encode_"   + str_id; task_encode    ->name = task_names[tid][2].c_str();
		task_names[tid][3] = "pct::puncture_" + str_id; task_puncture  ->name = task_names[tid][3].c_str();
		task_names[tid][4] = "mod::modulate_" + str_id; task_modulate  ->name = task_names[tid][4].c_str();

		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_gen_source), "task_submit::src::generate");
		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_build_crc ), "task_submit::crc::build"   );
		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_encode    ), "task_submit::enc::encode"  );
		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_puncture  ), "task_submit::pct::puncture");
		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_modulate  ), "task_submit::mod::modulate");
	}

	// Rayleigh channel
	if (this->params.chn->type.find("RAYLEIGH") != std::string::npos)
	{
		auto task_add_noise_wg  = module::Channel<  R  >::spu_task_add_noise_wg (this->channel[tid], spu_X_N3[tid], spu_Y_N1[tid], spu_H_N [tid]);
		auto task_filter        = module::Modem  <B,R,R>::spu_task_filter       (this->modem  [tid], spu_Y_N1[tid], spu_Y_N2[tid]               );
		auto task_demodulate_wg = module::Modem  <B,R,R>::spu_task_demodulate_wg(this->modem  [tid], spu_Y_N2[tid], spu_H_N [tid], spu_Y_N3[tid]);

		task_add_noise_wg ->priority = STARPU_MIN_PRIO +5;
		task_filter       ->priority = STARPU_MIN_PRIO +6;
		task_demodulate_wg->priority = STARPU_MIN_PRIO +7;

		task_names[tid][5] = "chn::add_noise_wg_"  + str_id; task_add_noise_wg ->name = task_names[tid][5].c_str();
		task_names[tid][6] = "mod::filter_"        + str_id; task_filter       ->name = task_names[tid][6].c_str();
		task_names[tid][7] = "mod::demodulate_wg_" + str_id; task_demodulate_wg->name = task_names[tid][7].c_str();

		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_add_noise_wg ), "task_submit::chn::add_noise_wg" );
		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_filter       ), "task_submit::mod::filter"       );
		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_demodulate_wg), "task_submit::mod::demodulate_wg");
	}
	else // additive channel (AWGN, USER, NO)
	{
		auto task_add_noise  = module::Channel<  R  >::spu_task_add_noise (this->channel[tid], spu_X_N3[tid], spu_Y_N1[tid]);
		auto task_filter     = module::Modem  <B,R,R>::spu_task_filter    (this->modem  [tid], spu_Y_N1[tid], spu_Y_N2[tid]);
		auto task_demodulate = module::Modem  <B,R,R>::spu_task_demodulate(this->modem  [tid], spu_Y_N2[tid], spu_Y_N3[tid]);

		task_add_noise ->priority = STARPU_MIN_PRIO +5;
		task_filter    ->priority = STARPU_MIN_PRIO +6;
		task_demodulate->priority = STARPU_MIN_PRIO +7;

		task_names[tid][5] = "chn::add_noise_"  + str_id; task_add_noise ->name = task_names[tid][5].c_str();
		task_names[tid][6] = "mod::filter_"     + str_id; task_filter    ->name = task_names[tid][6].c_str();
		task_names[tid][7] = "mod::demodulate_" + str_id; task_demodulate->name = task_names[tid][7].c_str();

		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_add_noise ), "task_submit::chn::add_noise" );
		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_filter    ), "task_submit::mod::filter"    );
		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_demodulate), "task_submit::mod::demodulate");
	}

	auto task_quantize   = module::Quantizer<  R,Q>::spu_task_process   (this->quantizer[tid], spu_Y_N3[tid], spu_Y_N4[tid]);
	auto task_depuncture = module::Puncturer<B,  Q>::spu_task_depuncture(this->puncturer[tid], spu_Y_N4[tid], spu_Y_N5[tid]);

	task_quantize  ->priority = STARPU_MIN_PRIO +8;
	task_depuncture->priority = STARPU_MIN_PRIO +9;

	task_names[tid][8] = "qnt::process_"    + str_id; task_quantize  ->name = task_names[tid][8].c_str();
	task_names[tid][9] = "pct::depuncture_" + str_id; task_depuncture->name = task_names[tid][9].c_str();

	STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_quantize  ), "task_submit::qnt::process"   );
	STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_depuncture), "task_submit::pct::depuncture");

	if (this->params.coset)
	{
		auto task_coset_real = module::Coset<B,Q>::spu_task_apply(this->coset_real[tid], spu_X_N1[tid], spu_Y_N5[tid], spu_Y_N5[tid]);
		task_coset_real->priority = STARPU_MIN_PRIO +10;
		task_names[tid][10] = "cst::apply_" + str_id; task_coset_real->name = task_names[tid][10].c_str();
		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_coset_real), "task_submit::cst::apply");
	}

	auto task_decode = module::Decoder_SIHO<B,Q>::spu_task_decode_siho(this->decoder[tid], spu_Y_N5[tid], spu_V_K1[tid]);
	task_decode->priority = STARPU_MIN_PRIO +11;
	task_names[tid][11] = "dec::decode_siho_" + str_id; task_decode->name = task_names[tid][11].c_str();
	STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_decode), "task_submit::dec::decode_siho");

	if (this->params.coset)
	{
		auto task_coset_bit = module::Coset<B,B>::spu_task_apply(this->coset_bit[tid], spu_U_K2[tid], spu_V_K1[tid], spu_V_K1[tid]);
		task_coset_bit->priority = STARPU_MIN_PRIO +12;
		task_names[tid][12] = "cst::apply_" + str_id; task_coset_bit->name = task_names[tid][12].c_str();
		STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_coset_bit), "task_submit::cst::apply");
	}

	auto task_extract_crc = module::CRC<B>::spu_task_extract(this->crc[tid], spu_V_K1[tid], spu_V_K2[tid]);
	task_extract_crc->priority = STARPU_MIN_PRIO +13;
	task_names[tid][13] = "crc::extract_" + str_id; task_extract_crc->name = task_names[tid][13].c_str();
	STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_extract_crc), "task_submit::crc::extract");

	auto task_check_err = module::Monitor<B>::spu_task_check_errors(this->monitor[tid], spu_U_K1[tid], spu_V_K2[tid]);
	task_check_err->priority = STARPU_MIN_PRIO +14;
	task_names[tid][14] = "mnt::check_errors_" + str_id; task_check_err->name = task_names[tid][14].c_str();
	STARPU_CHECK_RETURN_VALUE(starpu_task_submit(task_check_err), "task_submit::mnt::check_errors");

	frame_id++;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::simulation::SPU_BFER_std<B_8,R_8,Q_8>;
template class aff3ct::simulation::SPU_BFER_std<B_16,R_16,Q_16>;
template class aff3ct::simulation::SPU_BFER_std<B_32,R_32,Q_32>;
template class aff3ct::simulation::SPU_BFER_std<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::SPU_BFER_std<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation

#endif /* STARPU */
