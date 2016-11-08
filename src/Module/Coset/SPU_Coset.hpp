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
	mipp::vector<B> ref;
	mipp::vector<D> in_data;
	mipp::vector<D> out_data;

	static starpu_codelet spu_cl_apply;

public:
	SPU_Coset(const int size, const int n_frames = 1, const std::string name = "SPU_Coset")
	: Coset_i<B,D>(size, n_frames, name) {}

	virtual ~SPU_Coset() {}

	void spu_init()
	{
		if ((int)ref     .size() != this->size * this->n_frames) ref     .resize(this->size * this->n_frames);
		if ((int)in_data .size() != this->size * this->n_frames) in_data .resize(this->size * this->n_frames);
		if ((int)out_data.size() != this->size * this->n_frames) out_data.resize(this->size * this->n_frames);
	}

	static inline starpu_task* spu_task_apply(SPU_Coset<B,D> *coset, starpu_data_handle_t & in_data1,
	                                                                 starpu_data_handle_t & in_data2,
	                                                                 starpu_data_handle_t &out_data)
	{
		auto task = starpu_task_create();

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
		cl.cpu_funcs_name[0] = "Coset::apply";
		cl.nbuffers          = 3;
		cl.modes         [0] = STARPU_R;
		cl.modes         [1] = STARPU_R;
		cl.modes         [2] = STARPU_W;

		return cl;
	}

	static void spu_kernel_apply(void *buffers[], void *cl_arg)
	{
		auto coset = static_cast<SPU_Coset<B,D>*>(cl_arg);
		coset->spu_init();

		assert(STARPU_VECTOR_GET_NX(buffers[0]) == coset->ref     .size());
		assert(STARPU_VECTOR_GET_NX(buffers[1]) == coset->in_data .size());
		assert(STARPU_VECTOR_GET_NX(buffers[2]) == coset->out_data.size());

		const B* buff_in1 = (const B*)STARPU_VECTOR_GET_PTR(buffers[0]);
		const D* buff_in2 = (const D*)STARPU_VECTOR_GET_PTR(buffers[1]);
		      D* buff_out = (      D*)STARPU_VECTOR_GET_PTR(buffers[2]);

		std::copy(buff_in1, buff_in1 + coset->ref    .size(), coset->ref    .begin());
		std::copy(buff_in2, buff_in2 + coset->in_data.size(), coset->in_data.begin());
		coset->apply(coset->ref, coset->in_data, coset->out_data);
		std::copy(coset->out_data.begin(), coset->out_data.end(), buff_out);
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
