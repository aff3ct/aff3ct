#ifndef SPU_CRC_HPP_
#define SPU_CRC_HPP_

#ifdef STARPU
#include <vector>
#include <string>
#include <cassert>

#include <starpu.h>

#include "Tools/Perf/MIPP/mipp.h"

template <typename B>
class SPU_CRC : public CRC_i<B>
{
private:
	mipp::vector<B> U_K;

	static starpu_codelet spu_cl_build;

public:
	SPU_CRC(const int K, const int n_frames = 1, const std::string name = "SPU_CRC")
	: CRC_i<B>(K, n_frames, name) {}

	virtual ~SPU_CRC() {}

	void spu_init()
	{
		if ((int)U_K.size() != this->K * this->n_frames) U_K.resize(this->K * this->n_frames);
	}

	static inline starpu_task* spu_task_build(SPU_CRC<B> *crc, starpu_data_handle_t &in_out_data)
	{
		auto task = starpu_task_create();

		task->cl          = &SPU_CRC<B>::spu_cl_build;
		task->cl_arg      = (void*)(crc);
		task->cl_arg_size = sizeof(*crc);
		task->handles[0]  = in_out_data;
		task->modes  [0]  = STARPU_RW;

		return task;
	}

private:
	static starpu_codelet spu_init_cl_build()
	{
		starpu_codelet cl;

		cl.type              = STARPU_SEQ;
		cl.cpu_funcs     [0] = SPU_CRC<B>::spu_kernel_build;
		cl.cpu_funcs_name[0] = "CRC::build";
		cl.nbuffers          = 1;
		cl.modes         [0] = STARPU_RW;

		return cl;
	}

	static void spu_kernel_build(void *buffers[], void *cl_arg)
	{
		auto crc = static_cast<SPU_CRC<B>*>(cl_arg);
		crc->spu_init();

		assert(STARPU_VECTOR_GET_NX(buffers[0]) == crc->U_K.size());

		B* buff_in_out = (B*)STARPU_VECTOR_GET_PTR(buffers[0]);

		std::copy(buff_in_out, buff_in_out + crc->U_K.size(), crc->U_K.begin());
		crc->build(crc->U_K);
		std::copy(crc->U_K.begin(), crc->U_K.end(), buff_in_out);
	}
};

template <typename B>
starpu_codelet SPU_CRC<B>::spu_cl_build = SPU_CRC<B>::spu_init_cl_build();

template <typename B>
using CRC = SPU_CRC<B>;
#else
template <typename B>
using CRC = CRC_i<B>;
#endif

#endif /* SPU_CRC_HPP_ */
