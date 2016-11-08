#ifndef SPU_PUNCTURER_HPP_
#define SPU_PUNCTURER_HPP_

#ifdef STARPU
#include <vector>
#include <string>
#include <cassert>

#include <starpu.h>

#include "Tools/Perf/MIPP/mipp.h"

template <typename B, typename Q>
class SPU_Puncturer : public Puncturer_i<B,Q>
{
private:
	mipp::vector<B> X_N1, X_N2;
	mipp::vector<Q> Y_N1, Y_N2;

	static starpu_codelet spu_cl_puncture;
	static starpu_codelet spu_cl_depuncture;

public:
	SPU_Puncturer(const int K, const int N, const int N_code, const int n_frames = 1,
	              const std::string name = "SPU_Puncturer")
	: Puncturer_i<B,Q>(K, N, N_code, n_frames, name) {}

	virtual ~SPU_Puncturer() {}

	void spu_init()
	{
		if ((int)X_N1.size() != this->N_code * this->n_frames) X_N1.resize(this->N_code * this->n_frames);
		if ((int)X_N2.size() != this->N      * this->n_frames) X_N2.resize(this->N      * this->n_frames);
		if ((int)Y_N1.size() != this->N      * this->n_frames) Y_N1.resize(this->N      * this->n_frames);
		if ((int)Y_N2.size() != this->N_code * this->n_frames) Y_N2.resize(this->N_code * this->n_frames);
	}

	static inline starpu_task* spu_task_puncture(SPU_Puncturer<B,Q> *puncturer, starpu_data_handle_t & in_data,
	                                                                            starpu_data_handle_t &out_data)
	{
		auto task = starpu_task_create();

		task->cl          = &SPU_Puncturer<B,Q>::spu_cl_puncture;
		task->cl_arg      = (void*)(puncturer);
		task->cl_arg_size = sizeof(*puncturer);
		task->handles[0]  = in_data;
		task->modes  [0]  = STARPU_R;
		task->handles[1]  = out_data;
		task->modes  [1]  = STARPU_W;

		return task;
	}

	static inline starpu_task* spu_task_depuncture(SPU_Puncturer<B,Q> *puncturer, starpu_data_handle_t & in_data,
	                                                                              starpu_data_handle_t &out_data)
	{
		auto task = starpu_task_create();

		task->cl          = &SPU_Puncturer<B,Q>::spu_cl_depuncture;
		task->cl_arg      = (void*)(puncturer);
		task->cl_arg_size = sizeof(*puncturer);
		task->handles[0]  = in_data;
		task->modes  [0]  = STARPU_R;
		task->handles[1]  = out_data;
		task->modes  [1]  = STARPU_W;

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
		puncturer->spu_init();

		assert(STARPU_VECTOR_GET_NX(buffers[0]) == puncturer->X_N1.size());
		assert(STARPU_VECTOR_GET_NX(buffers[1]) == puncturer->X_N2.size());

		const B* buff_in  = (const B*)STARPU_VECTOR_GET_PTR(buffers[0]);
		      B* buff_out = (      B*)STARPU_VECTOR_GET_PTR(buffers[1]);

		std::copy(buff_in, buff_in + puncturer->X_N1.size(), puncturer->X_N1.begin());
		puncturer->puncture(puncturer->X_N1, puncturer->X_N2);
		std::copy(puncturer->X_N2.begin(), puncturer->X_N2.end(), buff_out);
	}

	static void spu_kernel_depuncture(void *buffers[], void *cl_arg)
	{
		auto puncturer = static_cast<SPU_Puncturer<B,Q>*>(cl_arg);
		puncturer->spu_init();

		assert(STARPU_VECTOR_GET_NX(buffers[0]) == puncturer->Y_N1.size());
		assert(STARPU_VECTOR_GET_NX(buffers[1]) == puncturer->Y_N2.size());

		const B* buff_in  = (const B*)STARPU_VECTOR_GET_PTR(buffers[0]);
		      B* buff_out = (      B*)STARPU_VECTOR_GET_PTR(buffers[1]);

		std::copy(buff_in, buff_in + puncturer->Y_N1.size(), puncturer->Y_N1.begin());
		puncturer->depuncture(puncturer->Y_N1, puncturer->Y_N2);
		std::copy(puncturer->Y_N2.begin(), puncturer->Y_N2.end(), buff_out);
	}
};

template <typename B, typename Q>
starpu_codelet SPU_Puncturer<B,Q>::spu_cl_puncture = SPU_Puncturer<B,Q>::spu_init_cl_puncture();

template <typename B, typename Q>
starpu_codelet SPU_Puncturer<B,Q>::spu_cl_depuncture = SPU_Puncturer<B,Q>::spu_init_cl_depuncture();

template <typename B, typename Q>
using Puncturer = SPU_Puncturer<B,Q>;
#else
template <typename B, typename Q>
using Puncturer = Puncturer_i<B,Q>;
#endif

#endif /* SPU_PUNCTURER_HPP_ */
