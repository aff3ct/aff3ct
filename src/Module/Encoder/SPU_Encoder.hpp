#ifndef SPU_ENCODER_HPP_
#define SPU_ENCODER_HPP_

#ifdef STARPU
#include <vector>
#include <string>
#include <cassert>

#include <starpu.h>

#include "Tools/Perf/MIPP/mipp.h"

template <typename B>
class SPU_Encoder : public Encoder_i<B>
{
private:
	mipp::vector<B> U_K;
	mipp::vector<B> X_N;

	static starpu_codelet spu_cl_encode;

public:
	SPU_Encoder(const int K, const int N, const int n_frames = 1, const std::string name = "SPU_Encoder")
	: Encoder_i<B>(K, N, n_frames, name) {}

	virtual ~SPU_Encoder() {}

	void spu_init()
	{
		if ((int)U_K.size() != this->K * this->n_frames) U_K.resize(this->K * this->n_frames);
		if ((int)X_N.size() != this->N * this->n_frames) X_N.resize(this->N * this->n_frames);
	}

	static inline starpu_task* spu_task_encode(SPU_Encoder<B> *encoder, starpu_data_handle_t & in_data,
	                                                                    starpu_data_handle_t &out_data)
	{
		auto task = starpu_task_create();

		task->cl          = &SPU_Encoder<B>::spu_cl_encode;
		task->cl_arg      = (void*)(encoder);
		task->cl_arg_size = sizeof(*encoder);
		task->handles[0]  = in_data;
		task->handles[1]  = out_data;

		return task;
	}

private:
	static starpu_codelet spu_init_cl_encode()
	{
		starpu_codelet cl;

		cl.type              = STARPU_SEQ;
		cl.cpu_funcs     [0] = SPU_Encoder<B>::spu_kernel_encode;
		cl.cpu_funcs_name[0] = "Encoder::encode";
		cl.nbuffers          = 2;
		cl.modes         [0] = STARPU_R;
		cl.modes         [1] = STARPU_W;

		return cl;
	}

	static void spu_kernel_encode(void *buffers[], void *cl_arg)
	{
		auto encoder = static_cast<SPU_Encoder<B>*>(cl_arg);
		encoder->spu_init();

		assert(STARPU_VECTOR_GET_NX(buffers[0]) == encoder->U_K.size());
		assert(STARPU_VECTOR_GET_NX(buffers[1]) == encoder->X_N.size());

		const B* buff_in  = (const B*)STARPU_VECTOR_GET_PTR(buffers[0]);
		      B* buff_out = (      B*)STARPU_VECTOR_GET_PTR(buffers[1]);

		std::copy(buff_in, buff_in + encoder->U_K.size(), encoder->U_K.begin());
		encoder->encode(encoder->U_K, encoder->X_N);
		std::copy(encoder->X_N.begin(), encoder->X_N.end(), buff_out);
	}
};

template <typename B>
starpu_codelet SPU_Encoder<B>::spu_cl_encode = SPU_Encoder<B>::spu_init_cl_encode();

template <typename B>
using Encoder = SPU_Encoder<B>;
#else
template <typename B>
using Encoder = Encoder_i<B>;
#endif

#endif /* SPU_ENCODER_HPP_ */
