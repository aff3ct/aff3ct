#ifndef SPU_COSET_HPP_
#define SPU_COSET_HPP_

#ifdef STARPU
#include <vector>
#include <string>
#include <cassert>

#include <starpu.h>

#include "Tools/Perf/MIPP/mipp.h"

template <typename B, typename D>
class SPU_Coset : public Coset_i<B,D>
{
private:
	static starpu_codelet spu_cl_apply;

public:
	SPU_Coset(const int size, const int n_frames = 1, const std::string name = "SPU_Coset")
	: Coset_i<B,D>(size, n_frames, name) {}

	virtual ~SPU_Coset() {}

	static inline starpu_task* spu_task_apply(SPU_Coset<B,D> *coset, starpu_data_handle_t &in_data1,
	                                                                 starpu_data_handle_t &in_data2,
	                                                                 starpu_data_handle_t &out_data)
	{
		auto task = starpu_task_create();

		task->name        = "cst::apply";
		task->cl          = &SPU_Coset<B,D>::spu_cl_apply;
		task->cl_arg      = (void*)(coset);
		task->cl_arg_size = sizeof(*coset);
		task->handles[0]  = in_data1;
		task->handles[1]  = in_data2;
		task->handles[2]  = out_data;

		return task;
	}

private:
	static starpu_codelet spu_init_cl_apply()
	{
		starpu_codelet cl;

		cl.type              = STARPU_SEQ;
		cl.cpu_funcs     [0] = SPU_Coset<B,D>::spu_kernel_apply;
		cl.cpu_funcs_name[0] = "cst::apply::cpu";
		cl.nbuffers          = 3;
		cl.modes         [0] = STARPU_R;
		cl.modes         [1] = STARPU_R;
		cl.modes         [2] = STARPU_W;

		return cl;
	}

	static void spu_kernel_apply(void *buffers[], void *cl_arg)
	{
		auto coset = static_cast<SPU_Coset<B,D>*>(cl_arg);

		auto task = starpu_task_get_current();

		auto udata0 = starpu_data_get_user_data(task->handles[0]); assert(udata0);
		auto udata1 = starpu_data_get_user_data(task->handles[1]); assert(udata1);
		auto udata2 = starpu_data_get_user_data(task->handles[2]); assert(udata2);

		auto ref      = static_cast<mipp::vector<B>*>(udata0);
		auto in_data  = static_cast<mipp::vector<D>*>(udata1);
		auto out_data = static_cast<mipp::vector<D>*>(udata2);

		coset->apply(*ref, *in_data, *out_data);
	}
};

template <typename B, typename D>
starpu_codelet SPU_Coset<B,D>::spu_cl_apply = SPU_Coset<B,D>::spu_init_cl_apply();

template <typename B, typename D>
using Coset = SPU_Coset<B,D>;
#else
template <typename B, typename D>
using Coset = Coset_i<B,D>;
#endif

#endif /* SPU_COSET_HPP_ */
