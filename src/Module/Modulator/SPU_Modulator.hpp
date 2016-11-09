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

	static inline starpu_task* spu_task_modulate(SPU_Modulator<B,R,Q> *modulator, starpu_data_handle_t & in_data,
	                                                                              starpu_data_handle_t &out_data)
	{
		auto task = starpu_task_create();

		task->cl          = &SPU_Modulator<B,R,Q>::spu_cl_modulate;
		task->cl_arg      = (void*)(modulator);
		task->cl_arg_size = sizeof(*modulator);
		task->handles[0]  = in_data;
		task->handles[1]  = out_data;

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
		task->handles[1]  = out_data;

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
		task->handles[1]  = out_data;

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
		task->handles[1]  = in_data2;
		task->handles[2]  = out_data;

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
		task->handles[1]  = in_data2;
		task->handles[2]  = out_data;

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
		task->handles[1]  = in_data2;
		task->handles[2]  = in_data3;
		task->handles[3]  = out_data;

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

		auto X_N1 = static_cast<mipp::vector<B>*>((void*)STARPU_VECTOR_GET_PTR(buffers[0]));
		auto X_N2 = static_cast<mipp::vector<R>*>((void*)STARPU_VECTOR_GET_PTR(buffers[1]));

		modulator->modulate(*X_N1, *X_N2);
	}

	static void spu_kernel_filter(void *buffers[], void *cl_arg)
	{
		auto modulator = static_cast<SPU_Modulator<B,R,Q>*>(cl_arg);

		auto Y_N1 = static_cast<mipp::vector<R>*>((void*)STARPU_VECTOR_GET_PTR(buffers[0]));
		auto Y_N2 = static_cast<mipp::vector<R>*>((void*)STARPU_VECTOR_GET_PTR(buffers[1]));

		modulator->filter(*Y_N1, *Y_N2);
	}

	static void spu_kernel_demodulate(void *buffers[], void *cl_arg)
	{
		auto modulator = static_cast<SPU_Modulator<B,R,Q>*>(cl_arg);

		mipp::vector<Q>* Y_N1 = static_cast<mipp::vector<Q>*>((void*)STARPU_VECTOR_GET_PTR(buffers[0]));
		mipp::vector<Q>* Y_N2 = static_cast<mipp::vector<Q>*>((void*)STARPU_VECTOR_GET_PTR(buffers[1]));

		modulator->demodulate(*Y_N1, *Y_N2);
	}

	static void spu_kernel_demodulate_wg(void *buffers[], void *cl_arg)
	{
		auto modulator = static_cast<SPU_Modulator<B,R,Q>*>(cl_arg);

		mipp::vector<Q>* Y_N1 = static_cast<mipp::vector<Q>*>((void*)STARPU_VECTOR_GET_PTR(buffers[0]));
		mipp::vector<R>* H_N  = static_cast<mipp::vector<R>*>((void*)STARPU_VECTOR_GET_PTR(buffers[1]));
		mipp::vector<Q>* Y_N2 = static_cast<mipp::vector<Q>*>((void*)STARPU_VECTOR_GET_PTR(buffers[2]));

		modulator->demodulate_with_gains(*Y_N1, *H_N, *Y_N2);
	}

	static void spu_kernel_tdemodulate(void *buffers[], void *cl_arg)
	{
		auto modulator = static_cast<SPU_Modulator<B,R,Q>*>(cl_arg);
		modulator->spu_init();

		mipp::vector<Q>* Y_N1 = static_cast<mipp::vector<Q>*>((void*)STARPU_VECTOR_GET_PTR(buffers[0]));
		mipp::vector<Q>* Y_N2 = static_cast<mipp::vector<Q>*>((void*)STARPU_VECTOR_GET_PTR(buffers[1]));
		mipp::vector<Q>* Y_N3 = static_cast<mipp::vector<Q>*>((void*)STARPU_VECTOR_GET_PTR(buffers[2]));

		modulator->tdemodulate(*Y_N1, *Y_N2, *Y_N3);
	}

	static void spu_kernel_tdemodulate_wg(void *buffers[], void *cl_arg)
	{
		auto modulator = static_cast<SPU_Modulator<B,R,Q>*>(cl_arg);
		modulator->spu_init();

		assert(STARPU_VECTOR_GET_NX(buffers[0]) == modulator->Y_N1.size());
		assert(STARPU_VECTOR_GET_NX(buffers[1]) == modulator->H_N .size());
		assert(STARPU_VECTOR_GET_NX(buffers[2]) == modulator->Y_N2.size());
		assert(STARPU_VECTOR_GET_NX(buffers[3]) == modulator->Y_N3.size());

		mipp::vector<Q>* Y_N1 = static_cast<mipp::vector<Q>*>((void*)STARPU_VECTOR_GET_PTR(buffers[0]));
		mipp::vector<R>* H_N  = static_cast<mipp::vector<R>*>((void*)STARPU_VECTOR_GET_PTR(buffers[1]));
		mipp::vector<Q>* Y_N2 = static_cast<mipp::vector<Q>*>((void*)STARPU_VECTOR_GET_PTR(buffers[2]));
		mipp::vector<Q>* Y_N3 = static_cast<mipp::vector<Q>*>((void*)STARPU_VECTOR_GET_PTR(buffers[3]));

		modulator->tdemodulate_with_gains(*Y_N1, *H_N, *Y_N2, *Y_N3);
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
