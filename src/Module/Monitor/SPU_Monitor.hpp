#ifndef SPU_MONITOR_HPP_
#define SPU_MONITOR_HPP_

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
template <typename B = int, typename R = float>
class SPU_Monitor : public Monitor_i<B,R>
{
private:
	static starpu_codelet spu_cl_check_errors;

public:
	SPU_Monitor(const int K, const int N, const int n_frames = 1, const std::string name = "SPU_Monitor")
	: Monitor_i<B,R>(K, N, n_frames, name) {}

	virtual ~SPU_Monitor() {}

	static inline starpu_task* spu_task_check_errors(SPU_Monitor<B,R> *monitor, starpu_data_handle_t &in_data1,
	                                                                            starpu_data_handle_t &in_data2)
	{
		auto task = starpu_task_create();

		task->name        = "mnt::check_errors";
		task->cl          = &SPU_Monitor<B,R>::spu_cl_check_errors;
		task->cl_arg      = (void*)(monitor);
		task->cl_arg_size = sizeof(*monitor);
		task->handles[0]  = in_data1;
		task->handles[1]  = in_data2;

		return task;
	}

private:
	static starpu_codelet spu_init_cl_check_errors()
	{
		starpu_codelet cl;

		cl.type              = STARPU_SEQ;
		cl.cpu_funcs     [0] = SPU_Monitor<B,R>::spu_kernel_check_errors;
		cl.cpu_funcs_name[0] = "mnt::check_errors::cpu";
		cl.nbuffers          = 2;
		cl.modes         [0] = STARPU_R;
		cl.modes         [1] = STARPU_R;

		return cl;
	}

	static void spu_kernel_check_errors(void *buffers[], void *cl_arg)
	{
		auto monitor = static_cast<SPU_Monitor<B,R>*>(cl_arg);

		auto task = starpu_task_get_current();

		auto udata0 = starpu_data_get_user_data(task->handles[0]); assert(udata0);
		auto udata1 = starpu_data_get_user_data(task->handles[1]); assert(udata1);

		auto U_K = static_cast<mipp::vector<B>*>(udata0);
		auto V_K = static_cast<mipp::vector<B>*>(udata1);

		monitor->check_errors(*U_K, *V_K);
	}
};

template <typename B, typename R>
starpu_codelet SPU_Monitor<B,R>::spu_cl_check_errors = SPU_Monitor<B,R>::spu_init_cl_check_errors();

template <typename B, typename R>
using Monitor = SPU_Monitor<B,R>;
}
}
#else
namespace aff3ct
{
namespace module
{
template <typename B, typename R>
using Monitor = Monitor_i<B,R>;
}
}
#endif

#endif /* SPU_MONITOR_HPP_ */
