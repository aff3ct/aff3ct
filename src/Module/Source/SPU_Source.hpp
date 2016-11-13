#ifndef SPU_SOURCE_HPP_
#define SPU_SOURCE_HPP_

#ifdef STARPU
#include <vector>
#include <string>
#include <cassert>

#include <starpu.h>

#include "Tools/Perf/MIPP/mipp.h"

template <typename B>
class SPU_Source : public Source_i<B>
{
private:
	static starpu_codelet spu_cl_generate;

public:
	SPU_Source(const int K, const int n_frames = 1, const std::string name = "SPU_Source")
	: Source_i<B>(K, n_frames, name) {}

	virtual ~SPU_Source() {}

	static inline starpu_task* spu_task_generate(SPU_Source<B> *source, starpu_data_handle_t &out_data)
	{
		auto task = starpu_task_create();

		task->name        = "src::generate";
		task->cl          = &SPU_Source<B>::spu_cl_generate;
		task->cl_arg      = (void*)(source);
		task->cl_arg_size = sizeof(*source);
		task->handles[0]  = out_data;

		return task;
	}

private:
	static starpu_codelet spu_init_cl_generate()
	{
		starpu_codelet cl;

		cl.type              = STARPU_SEQ;
		cl.cpu_funcs     [0] = SPU_Source<B>::spu_kernel_generate;
		cl.cpu_funcs_name[0] = "src::generate::cpu";
		cl.nbuffers          = 1;
		cl.modes         [0] = STARPU_W;

		return cl;
	}

	static void spu_kernel_generate(void *buffers[], void *cl_arg)
	{
		auto source = static_cast<SPU_Source<B>*>(cl_arg);

		auto task = starpu_task_get_current();

		auto udata0 = starpu_data_get_user_data(task->handles[0]); assert(udata0);

		auto U_K = static_cast<mipp::vector<B>*>(udata0);

		source->generate(*U_K);
	}
};

template <typename B>
starpu_codelet SPU_Source<B>::spu_cl_generate = SPU_Source<B>::spu_init_cl_generate();

template <typename B>
using Source = SPU_Source<B>;
#else
template <typename B>
using Source = Source_i<B>;
#endif

#endif /* SPU_SOURCE_HPP_ */
