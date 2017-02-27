#ifndef SPU_CHANNELS_HPP_
#define SPU_CHANNELS_HPP_

#ifdef STARPU
#include <vector>
#include <string>
#include <cassert>

#include <starpu.h>

#include "Tools/Perf/MIPP/mipp.h"

namespace aff3ct
{
namespace module
{
template <typename R = float>
class SPU_Channel : public Channel_i<R>
{
private:
	static starpu_codelet spu_cl_add_noise;
	static starpu_codelet spu_cl_add_noise_wg;

public:
	SPU_Channel(const int N, const int n_frames = 1, const std::string name = "SPU_Channel")
	: Channel_i<R>(N, n_frames, name) {}

	virtual ~SPU_Channel()
	{
	};

	static inline starpu_task* spu_task_add_noise(SPU_Channel<R> *channel, starpu_data_handle_t &in_data,
	                                                                       starpu_data_handle_t &out_data)
	{
		auto *task = starpu_task_create();

		task->name        = "chn::add_noise";
		task->cl          = &SPU_Channel<R>::spu_cl_add_noise;
		task->cl_arg      = (void*)(channel);
		task->cl_arg_size = sizeof(*channel);
		task->handles[0]  = in_data;
		task->handles[1]  = out_data;

		return task;
	}

	static inline starpu_task* spu_task_add_noise_wg(SPU_Channel<R> *channel, starpu_data_handle_t &in_data,
	                                                                          starpu_data_handle_t &out_data1,
	                                                                          starpu_data_handle_t &out_data2)
	{
		auto task = starpu_task_create();

		task->name        = "chn::add_noise_wg";
		task->cl          = &SPU_Channel<R>::spu_cl_add_noise_wg;
		task->cl_arg      = (void*)(channel);
		task->cl_arg_size = sizeof(*channel);
		task->handles[0]  = in_data;
		task->handles[1]  = out_data1;
		task->handles[2]  = out_data2;

		return task;
	}

private:
	static starpu_codelet spu_init_cl_add_noise()
	{
		starpu_codelet cl;

		cl.type              = STARPU_SEQ;
		cl.cpu_funcs     [0] = SPU_Channel<R>::spu_kernel_add_noise;
		cl.cpu_funcs_name[0] = "chn::add_noise::cpu";
		cl.nbuffers          = 2;
		cl.modes         [0] = STARPU_R;
		cl.modes         [1] = STARPU_W;

		return cl;
	}

	static starpu_codelet spu_init_cl_add_noise_wg()
	{
		starpu_codelet cl;

		cl.type              = STARPU_SEQ;
		cl.cpu_funcs     [0] = SPU_Channel<R>::spu_kernel_add_noise_wg;
		cl.cpu_funcs_name[0] = "chn::add_noise_wg::cpu";
		cl.nbuffers          = 3;
		cl.modes         [0] = STARPU_R;
		cl.modes         [1] = STARPU_W;
		cl.modes         [2] = STARPU_W;

		return cl;
	}

	static void spu_kernel_add_noise(void *buffers[], void *cl_arg)
	{
		auto channel = static_cast<SPU_Channel<R>*>(cl_arg);

		auto task = starpu_task_get_current();

		auto udata0 = starpu_data_get_user_data(task->handles[0]); assert(udata0);
		auto udata1 = starpu_data_get_user_data(task->handles[1]); assert(udata1);

		auto X_N = static_cast<mipp::vector<R>*>(udata0);
		auto Y_N = static_cast<mipp::vector<R>*>(udata1);

		channel->add_noise(*X_N, *Y_N);
	}

	static void spu_kernel_add_noise_wg(void *buffers[], void *cl_arg)
	{
		auto channel = static_cast<SPU_Channel<R>*>(cl_arg);

		auto task = starpu_task_get_current();

		auto udata0 = starpu_data_get_user_data(task->handles[0]); assert(udata0);
		auto udata1 = starpu_data_get_user_data(task->handles[1]); assert(udata1);
		auto udata2 = starpu_data_get_user_data(task->handles[2]); assert(udata2);

		auto X_N = static_cast<mipp::vector<R>*>(udata0);
		auto Y_N = static_cast<mipp::vector<R>*>(udata1);
		auto H_N = static_cast<mipp::vector<R>*>(udata2);

		channel->add_noise(*X_N, *Y_N, *H_N);
	}
};

template <typename R>
starpu_codelet SPU_Channel<R>::spu_cl_add_noise = SPU_Channel<R>::spu_init_cl_add_noise();

template <typename R>
starpu_codelet SPU_Channel<R>::spu_cl_add_noise_wg = SPU_Channel<R>::spu_init_cl_add_noise_wg();

template <typename R>
using Channel = SPU_Channel<R>;
}
}
#else
namespace aff3ct
{
namespace module
{
template <typename R>
using Channel = Channel_i<R>;
}
}
#endif

#endif /* SPU_CHANNELS_HPP_ */
