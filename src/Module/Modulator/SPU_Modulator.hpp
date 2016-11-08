#ifndef SPU_MODULATOR_HPP_
#define SPU_MODULATOR_HPP_

#ifdef STARPU
#include <vector>
#include <string>
#include <cassert>

#include <starpu.h>

#include "Tools/Perf/MIPP/mipp.h"

template <typename B, typename R, typename Q>
class SPU_Modulator : public Modulator_i<B,R,Q>
{
private:
	mipp::vector<B> X_N1;
	mipp::vector<R> X_N2, H_N, Y_N1f, Y_N2f;
	mipp::vector<Q> Y_N1, Y_N2, Y_N3;

	static starpu_codelet spu_cl_modulate;
	static starpu_codelet spu_cl_filter;
	static starpu_codelet spu_cl_demodulate;
	static starpu_codelet spu_cl_demodulate_wg;
	static starpu_codelet spu_cl_tdemodulate;
	static starpu_codelet spu_cl_tdemodulate_wg;

public:
	SPU_Modulator(const int N, const int N_mod, const int N_fil, const int n_frames = 1,
	              const std::string name = "SPU_Modulator")
	: Modulator_i<B,R,Q>(N, N_mod, N_fil, n_frames, name) {}

	SPU_Modulator(const int N, const int N_mod, const int n_frames = 1, const std::string name = "SPU_Modulator")
	: Modulator_i<B,R,Q>(N, N_mod, n_frames, name) {}

	SPU_Modulator(const int N, const int n_frames = 1, const std::string name = "SPU_Modulator")
	: Modulator_i<B,R,Q>(N, n_frames, name) {}

	virtual ~SPU_Modulator() {}

	void spu_init()
	{
		if ((int)X_N1 .size() != this->N     * this->n_frames) X_N1 .resize(this->N     * this->n_frames);
		if ((int)X_N2 .size() != this->N_mod * this->n_frames) X_N2 .resize(this->N_mod * this->n_frames);
		if ((int)Y_N1f.size() != this->N_mod * this->n_frames) Y_N1f.resize(this->N_mod * this->n_frames);
		if ((int)Y_N2f.size() != this->N_fil * this->n_frames) Y_N2f.resize(this->N_fil * this->n_frames);
		if ((int)Y_N1 .size() != this->N_fil * this->n_frames) Y_N1 .resize(this->N_fil * this->n_frames);
		if ((int)Y_N2 .size() != this->N     * this->n_frames) Y_N2 .resize(this->N     * this->n_frames);
		if ((int)Y_N3 .size() != this->N     * this->n_frames) Y_N3 .resize(this->N     * this->n_frames);
		if ((int)H_N  .size() != this->N_fil * this->n_frames) H_N  .resize(this->N_fil * this->n_frames);
	}

	static inline starpu_task* spu_task_modulate(SPU_Modulator<B,R,Q> *modulator, starpu_data_handle_t & in_data,
	                                                                              starpu_data_handle_t &out_data)
	{
		auto task = starpu_task_create();

		task->cl          = &SPU_Modulator<B,R,Q>::spu_cl_modulate;
		task->cl_arg      = (void*)(modulator);
		task->cl_arg_size = sizeof(*modulator);
		task->handles[0]  = in_data;
		task->modes  [0]  = STARPU_R;
		task->handles[1]  = out_data;
		task->modes  [1]  = STARPU_W;

		return task;
	}

	static inline starpu_task* spu_task_filter(SPU_Modulator<B,R,Q> *modulator, starpu_data_handle_t & in_data,
	                                                                            starpu_data_handle_t &out_data)
	{
		auto task = starpu_task_create();

		task->cl          = &SPU_Modulator<B,R,Q>::spu_cl_filter;
		task->cl_arg      = (void*)(modulator);
		task->cl_arg_size = sizeof(*modulator);
		task->handles[0]  = in_data;
		task->modes  [0]  = STARPU_R;
		task->handles[1]  = out_data;
		task->modes  [1]  = STARPU_W;

		return task;
	}

	static inline starpu_task* spu_task_demodulate(SPU_Modulator<B,R,Q> *modulator, starpu_data_handle_t & in_data,
	                                                                                starpu_data_handle_t &out_data)
	{
		auto task = starpu_task_create();

		task->cl          = &SPU_Modulator<B,R,Q>::spu_cl_demodulate;
		task->cl_arg      = (void*)(modulator);
		task->cl_arg_size = sizeof(*modulator);
		task->handles[0]  = in_data;
		task->modes  [0]  = STARPU_R;
		task->handles[1]  = out_data;
		task->modes  [1]  = STARPU_W;

		return task;
	}

	static inline starpu_task* spu_task_demodulate_wg(SPU_Modulator<B,R,Q> *modulator, starpu_data_handle_t & in_data1,
	                                                                                   starpu_data_handle_t & in_data2,
	                                                                                   starpu_data_handle_t &out_data)
	{
		auto task = starpu_task_create();

		task->cl          = &SPU_Modulator<B,R,Q>::spu_cl_demodulate_wg;
		task->cl_arg      = (void*)(modulator);
		task->cl_arg_size = sizeof(*modulator);
		task->handles[0]  = in_data1;
		task->modes  [0]  = STARPU_R;
		task->handles[1]  = in_data2;
		task->modes  [1]  = STARPU_R;
		task->handles[2]  = out_data;
		task->modes  [2]  = STARPU_W;

		return task;
	}

	static inline starpu_task* spu_task_tdemodulate(SPU_Modulator<B,R,Q> *modulator, starpu_data_handle_t & in_data1,
	                                                                                 starpu_data_handle_t & in_data2,
	                                                                                 starpu_data_handle_t &out_data)
	{
		auto task = starpu_task_create();

		task->cl          = &SPU_Modulator<B,R,Q>::spu_cl_tdemodulate;
		task->cl_arg      = (void*)(modulator);
		task->cl_arg_size = sizeof(*modulator);
		task->handles[0]  = in_data1;
		task->modes  [0]  = STARPU_R;
		task->handles[1]  = in_data2;
		task->modes  [1]  = STARPU_R;
		task->handles[2]  = out_data;
		task->modes  [2]  = STARPU_W;

		return task;
	}

	static inline starpu_task* spu_task_tdemodulate_wg(SPU_Modulator<B,R,Q> *modulator, starpu_data_handle_t & in_data1,
	                                                                                    starpu_data_handle_t & in_data2,
	                                                                                    starpu_data_handle_t & in_data3,
	                                                                                    starpu_data_handle_t &out_data)
	{
		auto task = starpu_task_create();

		task->cl          = &SPU_Modulator<B,R,Q>::spu_cl_tdemodulate_wg;
		task->cl_arg      = (void*)(modulator);
		task->cl_arg_size = sizeof(*modulator);
		task->handles[0]  = in_data1;
		task->modes  [0]  = STARPU_R;
		task->handles[1]  = in_data2;
		task->modes  [1]  = STARPU_R;
		task->handles[2]  = in_data3;
		task->modes  [2]  = STARPU_R;
		task->handles[3]  = out_data;
		task->modes  [3]  = STARPU_W;

		return task;
	}

private:
	static starpu_codelet spu_init_cl_modulate()
	{
		starpu_codelet cl;

		cl.type              = STARPU_SEQ;
		cl.cpu_funcs     [0] = SPU_Modulator<B,R,Q>::spu_kernel_modulate;
		cl.cpu_funcs_name[0] = "Modulator::modulate";
		cl.nbuffers          = 2;
		cl.modes         [0] = STARPU_R;
		cl.modes         [1] = STARPU_W;

		return cl;
	}

	static starpu_codelet spu_init_cl_filter()
	{
		starpu_codelet cl;

		cl.type              = STARPU_SEQ;
		cl.cpu_funcs     [0] = SPU_Modulator<B,R,Q>::spu_kernel_filter;
		cl.cpu_funcs_name[0] = "Modulator::filter";
		cl.nbuffers          = 2;
		cl.modes         [0] = STARPU_R;
		cl.modes         [1] = STARPU_W;

		return cl;
	}

	static starpu_codelet spu_init_cl_demodulate()
	{
		starpu_codelet cl;

		cl.type              = STARPU_SEQ;
		cl.cpu_funcs     [0] = SPU_Modulator<B,R,Q>::spu_kernel_demodulate;
		cl.cpu_funcs_name[0] = "Modulator::demodulate";
		cl.nbuffers          = 2;
		cl.modes         [0] = STARPU_R;
		cl.modes         [1] = STARPU_W;

		return cl;
	}

	static starpu_codelet spu_init_cl_demodulate_wg()
	{
		starpu_codelet cl;

		cl.type              = STARPU_SEQ;
		cl.cpu_funcs     [0] = SPU_Modulator<B,R,Q>::spu_kernel_demodulate_wg;
		cl.cpu_funcs_name[0] = "Modulator::demodulate_wg";
		cl.nbuffers          = 3;
		cl.modes         [0] = STARPU_R;
		cl.modes         [1] = STARPU_R;
		cl.modes         [2] = STARPU_W;

		return cl;
	}

	static starpu_codelet spu_init_cl_tdemodulate()
	{
		starpu_codelet cl;

		cl.type              = STARPU_SEQ;
		cl.cpu_funcs     [0] = SPU_Modulator<B,R,Q>::spu_kernel_tdemodulate;
		cl.cpu_funcs_name[0] = "Modulator::tdemodulate";
		cl.nbuffers          = 3;
		cl.modes         [0] = STARPU_R;
		cl.modes         [1] = STARPU_R;
		cl.modes         [2] = STARPU_W;

		return cl;
	}

	static starpu_codelet spu_init_cl_tdemodulate_wg()
	{
		starpu_codelet cl;

		cl.type              = STARPU_SEQ;
		cl.cpu_funcs     [0] = SPU_Modulator<B,R,Q>::spu_kernel_tdemodulate_wg;
		cl.cpu_funcs_name[0] = "Modulator::tdemodulate_wg";
		cl.nbuffers          = 4;
		cl.modes         [0] = STARPU_R;
		cl.modes         [1] = STARPU_R;
		cl.modes         [2] = STARPU_R;
		cl.modes         [3] = STARPU_W;

		return cl;
	}

	static void spu_kernel_modulate(void *buffers[], void *cl_arg)
	{
		auto modulator = static_cast<SPU_Modulator<B,R,Q>*>(cl_arg);
		modulator->spu_init();

		assert(STARPU_VECTOR_GET_NX(buffers[0]) == modulator->X_N1.size());
		assert(STARPU_VECTOR_GET_NX(buffers[1]) == modulator->X_N2.size());

		const B* buff_in  = (const B*)STARPU_VECTOR_GET_PTR(buffers[0]);
		      R* buff_out = (      R*)STARPU_VECTOR_GET_PTR(buffers[1]);

		std::copy(buff_in, buff_in + modulator->X_N1.size(), modulator->X_N1.begin());
		modulator->modulate(modulator->X_N1, modulator->X_N2);
		std::copy(modulator->X_N2.begin(), modulator->X_N2.end(), buff_out);
	}

	static void spu_kernel_filter(void *buffers[], void *cl_arg)
	{
		auto modulator = static_cast<SPU_Modulator<B,R,Q>*>(cl_arg);
		modulator->spu_init();

		assert(STARPU_VECTOR_GET_NX(buffers[0]) == modulator->Y_N1f.size());
		assert(STARPU_VECTOR_GET_NX(buffers[1]) == modulator->Y_N2f.size());

		const R* buff_in  = (const R*)STARPU_VECTOR_GET_PTR(buffers[0]);
		      R* buff_out = (      R*)STARPU_VECTOR_GET_PTR(buffers[1]);

		std::copy(buff_in, buff_in + modulator->Y_N1f.size(), modulator->Y_N1f.begin());
		modulator->filter(modulator->Y_N1f, modulator->Y_N2f);
		std::copy(modulator->Y_N2f.begin(), modulator->Y_N2f.end(), buff_out);
	}

	static void spu_kernel_demodulate(void *buffers[], void *cl_arg)
	{
		auto modulator = static_cast<SPU_Modulator<B,R,Q>*>(cl_arg);
		modulator->spu_init();

		assert(STARPU_VECTOR_GET_NX(buffers[0]) == modulator->Y_N1.size());
		assert(STARPU_VECTOR_GET_NX(buffers[1]) == modulator->Y_N2.size());

		const Q* buff_in  = (const Q*)STARPU_VECTOR_GET_PTR(buffers[0]);
		      Q* buff_out = (      Q*)STARPU_VECTOR_GET_PTR(buffers[1]);

		std::copy(buff_in, buff_in + modulator->Y_N1.size(), modulator->Y_N1.begin());
		modulator->demodulate(modulator->Y_N1, modulator->Y_N2);
		std::copy(modulator->Y_N2.begin(), modulator->Y_N2.end(), buff_out);
	}

	static void spu_kernel_demodulate_wg(void *buffers[], void *cl_arg)
	{
		auto modulator = static_cast<SPU_Modulator<B,R,Q>*>(cl_arg);
		modulator->spu_init();

		assert(STARPU_VECTOR_GET_NX(buffers[0]) == modulator->Y_N1.size());
		assert(STARPU_VECTOR_GET_NX(buffers[1]) == modulator->H_N .size());
		assert(STARPU_VECTOR_GET_NX(buffers[2]) == modulator->Y_N2.size());

		const Q* buff_in1 = (const Q*)STARPU_VECTOR_GET_PTR(buffers[0]);
		const R* buff_in2 = (const R*)STARPU_VECTOR_GET_PTR(buffers[1]);
		      Q* buff_out = (      Q*)STARPU_VECTOR_GET_PTR(buffers[2]);

		std::copy(buff_in1, buff_in1 + modulator->Y_N1.size(), modulator->Y_N1.begin());
		std::copy(buff_in2, buff_in2 + modulator->H_N .size(), modulator->H_N .begin());
		modulator->demodulate_with_gains(modulator->Y_N1, modulator->H_N, modulator->Y_N2);
		std::copy(modulator->Y_N2.begin(), modulator->Y_N2.end(), buff_out);
	}

	static void spu_kernel_tdemodulate(void *buffers[], void *cl_arg)
	{
		auto modulator = static_cast<SPU_Modulator<B,R,Q>*>(cl_arg);
		modulator->spu_init();

		assert(STARPU_VECTOR_GET_NX(buffers[0]) == modulator->Y_N1.size());
		assert(STARPU_VECTOR_GET_NX(buffers[1]) == modulator->Y_N2.size());
		assert(STARPU_VECTOR_GET_NX(buffers[2]) == modulator->Y_N3.size());

		const Q* buff_in1 = (const Q*)STARPU_VECTOR_GET_PTR(buffers[0]);
		const Q* buff_in2 = (const Q*)STARPU_VECTOR_GET_PTR(buffers[1]);
		      Q* buff_out = (      Q*)STARPU_VECTOR_GET_PTR(buffers[2]);

		std::copy(buff_in1, buff_in1 + modulator->Y_N1.size(), modulator->Y_N1.begin());
		std::copy(buff_in2, buff_in2 + modulator->Y_N2.size(), modulator->Y_N2.begin());
		modulator->tdemodulate(modulator->Y_N1, modulator->Y_N2, modulator->Y_N3);
		std::copy(modulator->Y_N3.begin(), modulator->Y_N3.end(), buff_out);
	}

	static void spu_kernel_tdemodulate_wg(void *buffers[], void *cl_arg)
	{
		auto modulator = static_cast<SPU_Modulator<B,R,Q>*>(cl_arg);
		modulator->spu_init();

		assert(STARPU_VECTOR_GET_NX(buffers[0]) == modulator->Y_N1.size());
		assert(STARPU_VECTOR_GET_NX(buffers[1]) == modulator->H_N .size());
		assert(STARPU_VECTOR_GET_NX(buffers[2]) == modulator->Y_N2.size());
		assert(STARPU_VECTOR_GET_NX(buffers[3]) == modulator->Y_N3.size());

		const Q* buff_in1 = (const Q*)STARPU_VECTOR_GET_PTR(buffers[0]);
		const R* buff_in2 = (const R*)STARPU_VECTOR_GET_PTR(buffers[1]);
		const Q* buff_in3 = (const Q*)STARPU_VECTOR_GET_PTR(buffers[2]);
		      Q* buff_out = (      Q*)STARPU_VECTOR_GET_PTR(buffers[3]);

		std::copy(buff_in1, buff_in1 + modulator->Y_N1.size(), modulator->Y_N1.begin());
		std::copy(buff_in2, buff_in2 + modulator->H_N .size(), modulator->H_N .begin());
		std::copy(buff_in3, buff_in3 + modulator->Y_N2.size(), modulator->Y_N2.begin());
		modulator->tdemodulate_with_gains(modulator->Y_N1, modulator->H_N, modulator->Y_N2, modulator->Y_N3);
		std::copy(modulator->Y_N3.begin(), modulator->Y_N3.end(), buff_out);
	}
};

template <typename B, typename R, typename Q>
starpu_codelet SPU_Modulator<B,R,Q>::spu_cl_modulate = SPU_Modulator<B,R,Q>::spu_init_cl_modulate();

template <typename B, typename R, typename Q>
starpu_codelet SPU_Modulator<B,R,Q>::spu_cl_filter = SPU_Modulator<B,R,Q>::spu_init_cl_filter();

template <typename B, typename R, typename Q>
starpu_codelet SPU_Modulator<B,R,Q>::spu_cl_demodulate = SPU_Modulator<B,R,Q>::spu_init_cl_demodulate();

template <typename B, typename R, typename Q>
starpu_codelet SPU_Modulator<B,R,Q>::spu_cl_demodulate_wg = SPU_Modulator<B,R,Q>::spu_init_cl_demodulate_wg();

template <typename B, typename R, typename Q>
starpu_codelet SPU_Modulator<B,R,Q>::spu_cl_tdemodulate = SPU_Modulator<B,R,Q>::spu_init_cl_tdemodulate();

template <typename B, typename R, typename Q>
starpu_codelet SPU_Modulator<B,R,Q>::spu_cl_tdemodulate_wg = SPU_Modulator<B,R,Q>::spu_init_cl_tdemodulate_wg();

template <typename B, typename R, typename Q>
using Modulator = SPU_Modulator<B,R,Q>;
#else
template <typename B, typename R, typename Q>
using Modulator = Modulator_i<B,R,Q>;
#endif

#endif /* SPU_MODULATOR_HPP_ */
