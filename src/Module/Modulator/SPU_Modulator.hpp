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

	static inline starpu_task* spu_task_modulate(      SPU_Modulator<B,R,Q> *modulator,
	                                                   starpu_data_handle_t &in_data,
	                                                   starpu_data_handle_t &out_data,
	                                             const int                   priority = STARPU_DEFAULT_PRIO,
	                                             const char                 *name     = "Modulator::modulate")
	{
		auto task = starpu_task_create();

		task->name        = "mod::modulate";
		task->cl          = &SPU_Modulator<B,R,Q>::spu_cl_modulate;
		task->cl_arg      = (void*)(modulator);
		task->cl_arg_size = sizeof(*modulator);
		task->handles[0]  = in_data;
		task->handles[1]  = out_data;

		return task;
	}

	static inline starpu_task* spu_task_filter(SPU_Modulator<B,R,Q> *modulator, starpu_data_handle_t &in_data,
	                                                                            starpu_data_handle_t &out_data)
	{
		auto task = starpu_task_create();

		task->name        = "mod::filter";
		task->cl          = &SPU_Modulator<B,R,Q>::spu_cl_filter;
		task->cl_arg      = (void*)(modulator);
		task->cl_arg_size = sizeof(*modulator);
		task->handles[0]  = in_data;
		task->handles[1]  = out_data;

		return task;
	}

	static inline starpu_task* spu_task_demodulate(SPU_Modulator<B,R,Q> *modulator, starpu_data_handle_t &in_data,
	                                                                                starpu_data_handle_t &out_data)
	{
		auto task = starpu_task_create();

		task->name        = "mod::demodulate";
		task->cl          = &SPU_Modulator<B,R,Q>::spu_cl_demodulate;
		task->cl_arg      = (void*)(modulator);
		task->cl_arg_size = sizeof(*modulator);
		task->handles[0]  = in_data;
		task->handles[1]  = out_data;

		return task;
	}

	static inline starpu_task* spu_task_demodulate_wg(SPU_Modulator<B,R,Q> *modulator, starpu_data_handle_t &in_data1,
	                                                                                   starpu_data_handle_t &in_data2,
	                                                                                   starpu_data_handle_t &out_data)
	{
		auto task = starpu_task_create();

		task->name        = "mod::demodulate_wg";
		task->cl          = &SPU_Modulator<B,R,Q>::spu_cl_demodulate_wg;
		task->cl_arg      = (void*)(modulator);
		task->cl_arg_size = sizeof(*modulator);
		task->handles[0]  = in_data1;
		task->handles[1]  = in_data2;
		task->handles[2]  = out_data;

		return task;
	}

	static inline starpu_task* spu_task_tdemodulate(SPU_Modulator<B,R,Q> *modulator, starpu_data_handle_t &in_data1,
	                                                                                 starpu_data_handle_t &in_data2,
	                                                                                 starpu_data_handle_t &out_data)
	{
		auto task = starpu_task_create();

		task->name        = "mod::tdemodulate";
		task->cl          = &SPU_Modulator<B,R,Q>::spu_cl_tdemodulate;
		task->cl_arg      = (void*)(modulator);
		task->cl_arg_size = sizeof(*modulator);
		task->handles[0]  = in_data1;
		task->handles[1]  = in_data2;
		task->handles[2]  = out_data;

		return task;
	}

	static inline starpu_task* spu_task_tdemodulate_wg(SPU_Modulator<B,R,Q> *modulator, starpu_data_handle_t &in_data1,
	                                                                                    starpu_data_handle_t &in_data2,
	                                                                                    starpu_data_handle_t &in_data3,
	                                                                                    starpu_data_handle_t &out_data)
	{
		auto task = starpu_task_create();

		task->name        = "mod::tdemodulate_wg";
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
		cl.cpu_funcs_name[0] = "mod::modulate::cpu";
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
		cl.cpu_funcs_name[0] = "mod::filter::cpu";
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
		cl.cpu_funcs_name[0] = "mod::demodulate::cpu";
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
		cl.cpu_funcs_name[0] = "mod::demodulate_wg::cpu";
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
		cl.cpu_funcs_name[0] = "mod::tdemodulate::cpu";
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
		cl.cpu_funcs_name[0] = "mod::tdemodulate_wg::cpu";
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

		auto task = starpu_task_get_current();

		auto udata0 = starpu_data_get_user_data(task->handles[0]); assert(udata0);
		auto udata1 = starpu_data_get_user_data(task->handles[1]); assert(udata1);

		auto X_N1 = static_cast<mipp::vector<B>*>(udata0);
		auto X_N2 = static_cast<mipp::vector<R>*>(udata1);

		modulator->modulate(*X_N1, *X_N2);
	}

	static void spu_kernel_filter(void *buffers[], void *cl_arg)
	{
		auto modulator = static_cast<SPU_Modulator<B,R,Q>*>(cl_arg);

		auto task = starpu_task_get_current();

		auto udata0 = starpu_data_get_user_data(task->handles[0]); assert(udata0);
		auto udata1 = starpu_data_get_user_data(task->handles[1]); assert(udata1);

		auto Y_N1 = static_cast<mipp::vector<R>*>(udata0);
		auto Y_N2 = static_cast<mipp::vector<R>*>(udata1);

		modulator->filter(*Y_N1, *Y_N2);
	}

	static void spu_kernel_demodulate(void *buffers[], void *cl_arg)
	{
		auto modulator = static_cast<SPU_Modulator<B,R,Q>*>(cl_arg);

		auto task = starpu_task_get_current();

		auto udata0 = starpu_data_get_user_data(task->handles[0]); assert(udata0);
		auto udata1 = starpu_data_get_user_data(task->handles[1]); assert(udata1);

		mipp::vector<Q>* Y_N1 = static_cast<mipp::vector<Q>*>(udata0);
		mipp::vector<Q>* Y_N2 = static_cast<mipp::vector<Q>*>(udata1);

		modulator->demodulate(*Y_N1, *Y_N2);
	}

	static void spu_kernel_demodulate_wg(void *buffers[], void *cl_arg)
	{
		auto modulator = static_cast<SPU_Modulator<B,R,Q>*>(cl_arg);

		auto task = starpu_task_get_current();

		auto udata0 = starpu_data_get_user_data(task->handles[0]); assert(udata0);
		auto udata1 = starpu_data_get_user_data(task->handles[1]); assert(udata1);
		auto udata2 = starpu_data_get_user_data(task->handles[2]); assert(udata2);

		mipp::vector<Q>* Y_N1 = static_cast<mipp::vector<Q>*>(udata0);
		mipp::vector<R>* H_N  = static_cast<mipp::vector<R>*>(udata1);
		mipp::vector<Q>* Y_N2 = static_cast<mipp::vector<Q>*>(udata2);

		modulator->demodulate_with_gains(*Y_N1, *H_N, *Y_N2);
	}

	static void spu_kernel_tdemodulate(void *buffers[], void *cl_arg)
	{
		auto modulator = static_cast<SPU_Modulator<B,R,Q>*>(cl_arg);

		auto task = starpu_task_get_current();

		auto udata0 = starpu_data_get_user_data(task->handles[0]); assert(udata0);
		auto udata1 = starpu_data_get_user_data(task->handles[1]); assert(udata1);
		auto udata2 = starpu_data_get_user_data(task->handles[2]); assert(udata2);

		mipp::vector<Q>* Y_N1 = static_cast<mipp::vector<Q>*>(udata0);
		mipp::vector<Q>* Y_N2 = static_cast<mipp::vector<Q>*>(udata1);
		mipp::vector<Q>* Y_N3 = static_cast<mipp::vector<Q>*>(udata2);

		modulator->tdemodulate(*Y_N1, *Y_N2, *Y_N3);
	}

	static void spu_kernel_tdemodulate_wg(void *buffers[], void *cl_arg)
	{
		auto modulator = static_cast<SPU_Modulator<B,R,Q>*>(cl_arg);

		auto task = starpu_task_get_current();

		auto udata0 = starpu_data_get_user_data(task->handles[0]); assert(udata0);
		auto udata1 = starpu_data_get_user_data(task->handles[1]); assert(udata1);
		auto udata2 = starpu_data_get_user_data(task->handles[2]); assert(udata2);
		auto udata3 = starpu_data_get_user_data(task->handles[3]); assert(udata3);

		mipp::vector<Q>* Y_N1 = static_cast<mipp::vector<Q>*>(udata0);
		mipp::vector<R>* H_N  = static_cast<mipp::vector<R>*>(udata1);
		mipp::vector<Q>* Y_N2 = static_cast<mipp::vector<Q>*>(udata2);
		mipp::vector<Q>* Y_N3 = static_cast<mipp::vector<Q>*>(udata3);

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
