#ifndef SPU_SISO_HPP_
#define SPU_SISO_HPP_

#ifdef STARPU
#include <vector>
#include <string>
#include <cassert>

#include <starpu.h>

#include "Tools/Perf/MIPP/mipp.h"

template <typename R>
class SPU_SISO : public SISO_i<R>
{
private:
	static starpu_codelet spu_cl_soft_decode;

public:
	SPU_SISO(const int K, const int N, const int n_frames, const int simd_inter_frame_level = 1,
	        const std::string name = "SPU_SISO")
	: SISO_i<R>(K, N, n_frames, simd_inter_frame_level, name) {}

	virtual ~SPU_SISO() {}

	static inline starpu_task* spu_task_soft_decode(SPU_SISO<R> *siso, starpu_data_handle_t & in_data,
	                                                                   starpu_data_handle_t &out_data)
	{
		auto task = starpu_task_create();

		task->cl          = &SPU_SISO<R>::spu_cl_soft_decode;
		task->cl_arg      = (void*)(siso);
		task->cl_arg_size = sizeof(*siso);
		task->handles[0]  = in_data;
		task->handles[1]  = out_data;

		return task;
	}

private:
	static starpu_codelet spu_init_cl_soft_decode()
	{
		starpu_codelet cl;

		cl.type              = STARPU_SEQ;
		cl.cpu_funcs     [0] = SPU_SISO<R>::spu_kernel_soft_decode;
		cl.cpu_funcs_name[0] = "SISO::soft_decode";
		cl.nbuffers          = 2;
		cl.modes         [0] = STARPU_R;
		cl.modes         [1] = STARPU_W;

		return cl;
	}

	static void spu_kernel_soft_decode(void *buffers[], void *cl_arg)
	{
		auto decoder = static_cast<SPU_SISO<R>*>(cl_arg);

		auto task = starpu_task_get_current();

		auto udata0 = starpu_data_get_user_data(task->handles[0]); assert(udata0);
		auto udata1 = starpu_data_get_user_data(task->handles[1]); assert(udata1);

		auto Y_N1 = static_cast<mipp::vector<R>*>(udata0);
		auto Y_N2 = static_cast<mipp::vector<R>*>(udata1);

		decoder->soft_decode(*Y_N1, *Y_N2);
	}
};

template <typename R>
starpu_codelet SPU_SISO<R>::spu_cl_soft_decode = SPU_SISO<R>::spu_init_cl_soft_decode();

template <typename R>
using SISO = SPU_SISO<R>;
#else
template <typename R>
using SISO = SISO_i<R>;
#endif

#endif /* SPU_SISO_HPP_ */
