#ifndef SPU_SISO_HPP_
#define SPU_SISO_HPP_

#ifdef STARPU
#include <vector>
#include <string>
#include <cassert>

#include <starpu.h>

#include "Tools/Perf/MIPP/mipp.h"

template <typename R>
class SPU_SISO : public SISO_i<R>
{
private:
	mipp::vector<R> Y_N1, Y_N2;

public:
	static starpu_codelet spu_cl_soft_decode;

public:
	SPU_SISO(const int K, const int N, const int n_frames, const int simd_inter_frame_level = 1,
	        const std::string name = "SPU_SISO")
	: SISO_i<R>(K, N, n_frames, simd_inter_frame_level, name) {}

	virtual ~SPU_SISO() {}

	void spu_init()
	{
		if ((int)Y_N1.size() != this->N_siso * this->n_frames) Y_N1.resize(this->N_siso * this->n_frames);
		if ((int)Y_N2.size() != this->N_siso * this->n_frames) Y_N2.resize(this->N_siso * this->n_frames);
	}

private:
	static starpu_codelet spu_init_cl_soft_decode()
	{
		starpu_codelet cl;

		cl.type              = STARPU_SEQ;
		cl.cpu_funcs     [0] = SPU_SISO<R>::spu_kernel_soft_decode;
		cl.cpu_funcs_name[0] = "SISO::soft_decode";
		cl.nbuffers          = 2;
		cl.modes         [0] = STARPU_R;
		cl.modes         [1] = STARPU_W;

		return cl;
	}

	static void spu_kernel_soft_decode(void *buffers[], void *cl_arg)
	{
		auto decoder = static_cast<SPU_SISO<R>*>(cl_arg);
		decoder->spu_init();

		assert(STARPU_VECTOR_GET_NX(buffers[0]) == decoder->Y_N1.size());
		assert(STARPU_VECTOR_GET_NX(buffers[1]) == decoder->Y_N2.size());

		const R* buff_in  = (const R*)STARPU_VECTOR_GET_PTR(buffers[0]);
		      R* buff_out = (      R*)STARPU_VECTOR_GET_PTR(buffers[1]);

		std::copy(buff_in, buff_in + decoder->Y_N1.size(), decoder->Y_N1.begin());
		decoder->soft_decode(decoder->Y_N1, decoder->Y_N2);
		std::copy(decoder->Y_N2.begin(), decoder->Y_N2.end(), buff_out);
	}
};

template <typename R>
starpu_codelet SPU_SISO<R>::spu_cl_soft_decode = SPU_SISO<R>::spu_init_cl_soft_decode();

template <typename R>
using SISO = SPU_SISO<R>;
#else
template <typename R>
using SISO = SISO_i<R>;
#endif

#endif /* SPU_SISO_HPP_ */
