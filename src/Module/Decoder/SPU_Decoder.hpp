#ifndef SPU_DECODER_HPP_
#define SPU_DECODER_HPP_

#ifdef STARPU
#include <vector>
#include <string>
#include <cassert>

#include <starpu.h>

#include "Tools/Perf/MIPP/mipp.h"

template <typename B, typename R>
class SPU_Decoder : public Decoder_i<B,R>
{
private:
	static starpu_codelet spu_cl_hard_decode;

public:
	SPU_Decoder(const int K, const int N, const int n_frames = 1, const int simd_inter_frame_level = 1,
	            const std::string name = "SPU_Decoder")
	: Decoder_i<B,R>(K, N, n_frames, simd_inter_frame_level, name) {}

	virtual ~SPU_Decoder() {}

	static inline starpu_task* spu_task_hard_decode(SPU_Decoder<B,R> *decoder, starpu_data_handle_t & in_data,
	                                                                           starpu_data_handle_t &out_data)
	{
		auto task = starpu_task_create();

		task->cl          = &SPU_Decoder<B,R>::spu_cl_hard_decode;
		task->cl_arg      = (void*)(decoder);
		task->cl_arg_size = sizeof(*decoder);
		task->handles[0]  = in_data;
		task->handles[1]  = out_data;

		return task;
	}

private:
	static starpu_codelet spu_init_cl_hard_decode()
	{
		starpu_codelet cl;

		cl.type              = STARPU_SEQ;
		cl.cpu_funcs     [0] = SPU_Decoder<B,R>::spu_kernel_hard_decode;
		cl.cpu_funcs_name[0] = "Decoder::hard_decode";
		cl.nbuffers          = 2;
		cl.modes         [0] = STARPU_R;
		cl.modes         [1] = STARPU_W;

		return cl;
	}

	static void spu_kernel_hard_decode(void *buffers[], void *cl_arg)
	{
		auto decoder = static_cast<SPU_Decoder<B,R>*>(cl_arg);

		auto Y_N = static_cast<mipp::vector<R>*>((void*)STARPU_VECTOR_GET_PTR(buffers[0]));
		auto V_K = static_cast<mipp::vector<B>*>((void*)STARPU_VECTOR_GET_PTR(buffers[1]));

		decoder->hard_decode(*Y_N, *V_K);
	}
};

template <typename B, typename R>
starpu_codelet SPU_Decoder<B,R>::spu_cl_hard_decode = SPU_Decoder<B,R>::spu_init_cl_hard_decode();

template <typename B, typename R>
using Decoder = SPU_Decoder<B,R>;
#else
template <typename B, typename R>
using Decoder = Decoder_i<B,R>;
#endif

#endif /* DECODER_HPP_ */
