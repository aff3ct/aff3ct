#ifndef SPU_CHANNELS_HPP_
#define SPU_CHANNELS_HPP_

#ifdef STARPU
#include <vector>
#include <string>
#include <cassert>

#include <starpu.h>

#include "Tools/Perf/MIPP/mipp.h"

template <typename R>
class SPU_Channel : public Channel_i<R>
{
private:
	mipp::vector<R> X_N, Y_N, H_N;

	static starpu_codelet spu_cl_add_noise;
	static starpu_codelet spu_cl_add_noise_wg;

public:
	SPU_Channel(const int N, const int n_frames = 1, const std::string name = "SPU_Channel")
	: Channel_i<R>(N, n_frames, name) {}

	virtual ~SPU_Channel()
	{
	};

	void spu_init()
	{
		if ((int)X_N.size() != this->N * this->n_frames) X_N.resize(this->N * this->n_frames);
		if ((int)Y_N.size() != this->N * this->n_frames) Y_N.resize(this->N * this->n_frames);
		if ((int)H_N.size() != this->N * this->n_frames) H_N.resize(this->N * this->n_frames);
	}

	static inline starpu_task* spu_task_add_noise(SPU_Channel<R> *channel, starpu_data_handle_t & in_data,
	                                                                       starpu_data_handle_t &out_data)
	{
		auto *task = starpu_task_create();

		task->cl          = &SPU_Channel<R>::spu_cl_add_noise;
		task->cl_arg      = (void*)(channel);
		task->cl_arg_size = sizeof(*channel);
		task->handles[0]  = in_data;
		task->handles[1]  = out_data;

		return task;
	}

	static inline starpu_task* spu_task_add_noise_wg(SPU_Channel<R> *channel, starpu_data_handle_t & in_data,
	                                                                          starpu_data_handle_t &out_data1,
	                                                                          starpu_data_handle_t &out_data2)
	{
		auto task = starpu_task_create();

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
		cl.cpu_funcs_name[0] = "Channel::add_noise";
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
		cl.cpu_funcs_name[0] = "Channel::add_noise_wg";
		cl.nbuffers          = 3;
		cl.modes         [0] = STARPU_R;
		cl.modes         [1] = STARPU_W;
		cl.modes         [2] = STARPU_W;

		return cl;
	}

	static void spu_kernel_add_noise(void *buffers[], void *cl_arg)
	{
		auto channel = static_cast<SPU_Channel<R>*>(cl_arg);
		channel->spu_init();

		assert(STARPU_VECTOR_GET_NX(buffers[0]) == channel->X_N.size());
		assert(STARPU_VECTOR_GET_NX(buffers[1]) == channel->Y_N.size());

		const R* buff_in  = (const R*)STARPU_VECTOR_GET_PTR(buffers[0]);
		      R* buff_out = (      R*)STARPU_VECTOR_GET_PTR(buffers[1]);

		std::copy(buff_in, buff_in + channel->X_N.size(), channel->X_N.begin());
		channel->add_noise(channel->X_N, channel->Y_N, channel->H_N);
		std::copy(channel->Y_N.begin(), channel->Y_N.end(), buff_out);
	}

	static void spu_kernel_add_noise_wg(void *buffers[], void *cl_arg)
	{
		auto channel = static_cast<SPU_Channel<R>*>(cl_arg);
		channel->spu_init();

		assert(STARPU_VECTOR_GET_NX(buffers[0]) == channel->X_N.size());
		assert(STARPU_VECTOR_GET_NX(buffers[1]) == channel->Y_N.size());
		assert(STARPU_VECTOR_GET_NX(buffers[2]) == channel->H_N.size());

		const R* buff_in   = (const R*)STARPU_VECTOR_GET_PTR(buffers[0]);
		      R* buff_out1 = (      R*)STARPU_VECTOR_GET_PTR(buffers[1]);
		      R* buff_out2 = (      R*)STARPU_VECTOR_GET_PTR(buffers[2]);

		std::copy(buff_in, buff_in + channel->X_N.size(), channel->X_N.begin());
		channel->add_noise(channel->X_N, channel->Y_N, channel->H_N);
		std::copy(channel->Y_N.begin(), channel->Y_N.end(), buff_out1);
		std::copy(channel->H_N.begin(), channel->H_N.end(), buff_out2);
	}
};

template <typename R>
starpu_codelet SPU_Channel<R>::spu_cl_add_noise = SPU_Channel<R>::spu_init_cl_add_noise();

template <typename R>
starpu_codelet SPU_Channel<R>::spu_cl_add_noise_wg = SPU_Channel<R>::spu_init_cl_add_noise_wg();

template <typename R>
using Channel = SPU_Channel<R>;
#else
template <typename R>
using Channel = Channel_i<R>;
#endif

#endif /* SPU_CHANNELS_HPP_ */
