#ifndef SPU_PUNCTURER_HPP_
#define SPU_PUNCTURER_HPP_

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
template <typename B, typename Q>
class SPU_Puncturer : public Puncturer_i<B,Q>
{
private:
	static starpu_codelet spu_cl_puncture;
	static starpu_codelet spu_cl_depuncture;

public:
	SPU_Puncturer(const int K, const int N, const int N_code, const int n_frames = 1,
	              const std::string name = "SPU_Puncturer")
	: Puncturer_i<B,Q>(K, N, N_code, n_frames, name) {}

	virtual ~SPU_Puncturer() {}

	static inline starpu_task* spu_task_puncture(SPU_Puncturer<B,Q> *puncturer, starpu_data_handle_t &in_data,
	                                                                            starpu_data_handle_t &out_data)
	{
		auto task = starpu_task_create();

		task->name        = "pct::puncture";
		task->cl          = &SPU_Puncturer<B,Q>::spu_cl_puncture;
		task->cl_arg      = (void*)(puncturer);
		task->cl_arg_size = sizeof(*puncturer);
		task->handles[0]  = in_data;
		task->handles[1]  = out_data;

		return task;
	}

	static inline starpu_task* spu_task_depuncture(SPU_Puncturer<B,Q> *puncturer, starpu_data_handle_t &in_data,
	                                                                              starpu_data_handle_t &out_data)
	{
		auto task = starpu_task_create();

		task->name        = "pct::depuncture::cpu";
		task->cl          = &SPU_Puncturer<B,Q>::spu_cl_depuncture;
		task->cl_arg      = (void*)(puncturer);
		task->cl_arg_size = sizeof(*puncturer);
		task->handles[0]  = in_data;
		task->handles[1]  = out_data;

		return task;
	}

private:
	static starpu_codelet spu_init_cl_puncture()
	{
		starpu_codelet cl;

		cl.type              = STARPU_SEQ;
		cl.cpu_funcs     [0] = SPU_Puncturer<B,Q>::spu_kernel_puncture;
		cl.cpu_funcs_name[0] = "Puncturer::puncture";
		cl.nbuffers          = 2;
		cl.modes         [0] = STARPU_R;
		cl.modes         [1] = STARPU_W;

		return cl;
	}

	static starpu_codelet spu_init_cl_depuncture()
	{
		starpu_codelet cl;

		cl.type              = STARPU_SEQ;
		cl.cpu_funcs     [0] = SPU_Puncturer<B,Q>::spu_kernel_depuncture;
		cl.cpu_funcs_name[0] = "Puncturer::depuncture";
		cl.nbuffers          = 2;
		cl.modes         [0] = STARPU_R;
		cl.modes         [1] = STARPU_W;

		return cl;
	}

	static void spu_kernel_puncture(void *buffers[], void *cl_arg)
	{
		auto puncturer = static_cast<SPU_Puncturer<B,Q>*>(cl_arg);

		auto task = starpu_task_get_current();

		auto udata0 = starpu_data_get_user_data(task->handles[0]); assert(udata0);
		auto udata1 = starpu_data_get_user_data(task->handles[1]); assert(udata1);

		auto X_N1 = static_cast<mipp::vector<B>*>(udata0);
		auto X_N2 = static_cast<mipp::vector<B>*>(udata1);

		puncturer->puncture(*X_N1, *X_N2);
	}

	static void spu_kernel_depuncture(void *buffers[], void *cl_arg)
	{
		auto puncturer = static_cast<SPU_Puncturer<B,Q>*>(cl_arg);

		auto task = starpu_task_get_current();

		auto udata0 = starpu_data_get_user_data(task->handles[0]); assert(udata0);
		auto udata1 = starpu_data_get_user_data(task->handles[1]); assert(udata1);

		auto Y_N1 = static_cast<mipp::vector<Q>*>(udata0);
		auto Y_N2 = static_cast<mipp::vector<Q>*>(udata1);

		puncturer->depuncture(*Y_N1, *Y_N2);
	}
};

template <typename B, typename Q>
starpu_codelet SPU_Puncturer<B,Q>::spu_cl_puncture = SPU_Puncturer<B,Q>::spu_init_cl_puncture();

template <typename B, typename Q>
starpu_codelet SPU_Puncturer<B,Q>::spu_cl_depuncture = SPU_Puncturer<B,Q>::spu_init_cl_depuncture();

template <typename B, typename Q>
using Puncturer = SPU_Puncturer<B,Q>;
}
}
#else
namespace aff3ct
{
namespace module
{
template <typename B, typename Q>
using Puncturer = Puncturer_i<B,Q>;
}
}
#endif

#endif /* SPU_PUNCTURER_HPP_ */
