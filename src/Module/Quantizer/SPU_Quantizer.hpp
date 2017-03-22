#ifndef SPU_QUANTIZER_HPP_
#define SPU_QUANTIZER_HPP_

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
template <typename R = float, typename Q = int>
class SPU_Quantizer : public Quantizer_i<R,Q>
{
private:
	static starpu_codelet spu_cl_process;

public:
	SPU_Quantizer(const int N, const int n_frames = 1, const std::string name = "SPU_Quantizer")
	: Quantizer_i<R,Q>(N, n_frames, name) {}

	virtual ~SPU_Quantizer() {};

	static inline starpu_task* spu_task_process(SPU_Quantizer<R,Q> *quantizer, starpu_data_handle_t &in_data,
	                                                                           starpu_data_handle_t &out_data)
	{
		auto task = starpu_task_create();

		task->name        = "qnt::process";
		task->cl          = &SPU_Quantizer<R,Q>::spu_cl_process;
		task->cl_arg      = (void*)(quantizer);
		task->cl_arg_size = sizeof(*quantizer);
		task->handles[0]  = in_data;
		task->handles[1]  = out_data;

		return task;
	}

private:
	static starpu_codelet spu_init_cl_process()
	{
		starpu_codelet cl;

		cl.type              = STARPU_SEQ;
		cl.cpu_funcs     [0] = SPU_Quantizer<R,Q>::spu_kernel_process;
		cl.cpu_funcs_name[0] = "qnt::process::cpu";
		cl.nbuffers          = 2;
		cl.modes         [0] = STARPU_R;
		cl.modes         [1] = STARPU_W;

		return cl;
	}

	static void spu_kernel_process(void *buffers[], void *cl_arg)
	{
		auto quantizer = static_cast<SPU_Quantizer<R,Q>*>(cl_arg);

		auto task = starpu_task_get_current();

		auto udata0 = starpu_data_get_user_data(task->handles[0]); assert(udata0);
		auto udata1 = starpu_data_get_user_data(task->handles[1]); assert(udata1);

		auto Y_N1 = static_cast<mipp::vector<R>*>(udata0);
		auto Y_N2 = static_cast<mipp::vector<Q>*>(udata1);

		quantizer->process(*Y_N1, *Y_N2);
	}
};

template <typename R, typename Q>
starpu_codelet SPU_Quantizer<R,Q>::spu_cl_process = SPU_Quantizer<R,Q>::spu_init_cl_process();

template <typename R, typename Q>
using Quantizer = SPU_Quantizer<R,Q>;
}
}
#else
namespace aff3ct
{
namespace module
{
template <typename R = float, typename Q = int>
using Quantizer = Quantizer_i<R,Q>;
}
}
#endif

#endif /* SPU_QUANTIZER_HPP_ */
