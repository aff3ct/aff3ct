#ifndef SPU_QUANTIZER_HPP_
#define SPU_QUANTIZER_HPP_

#ifdef STARPU
#include <vector>
#include <string>
#include <cassert>

#include <starpu.h>

#include "Tools/Perf/MIPP/mipp.h"

template <typename R, typename Q>
class SPU_Quantizer : public Quantizer_i<R,Q>
{
private:
	mipp::vector<R> Y_N1;
	mipp::vector<Q> Y_N2;

public:
	static starpu_codelet spu_cl_process;

public:
	SPU_Quantizer(const int N, const int n_frames = 1, const std::string name = "SPU_Quantizer")
	: Quantizer_i<R,Q>(N, n_frames, name) {}

	virtual ~SPU_Quantizer() {};

	void spu_init()
	{
		if ((int)Y_N1.size() != this->N * this->n_frames) Y_N1.resize(this->N * this->n_frames);
		if ((int)Y_N2.size() != this->N * this->n_frames) Y_N2.resize(this->N * this->n_frames);
	}

private:
	static starpu_codelet spu_init_cl_process()
	{
		starpu_codelet cl;

		cl.type              = STARPU_SEQ;
		cl.cpu_funcs     [0] = SPU_Quantizer<R,Q>::spu_kernel_process;
		cl.cpu_funcs_name[0] = "Quantizer::process";
		cl.nbuffers          = 2;
		cl.modes         [0] = STARPU_R;
		cl.modes         [1] = STARPU_W;

		return cl;
	}

	static void spu_kernel_process(void *buffers[], void *cl_arg)
	{
		auto quantizer = static_cast<SPU_Quantizer<R,Q>*>(cl_arg);
		quantizer->spu_init();

		assert(STARPU_VECTOR_GET_NX(buffers[0]) == quantizer->Y_N1.size());
		assert(STARPU_VECTOR_GET_NX(buffers[1]) == quantizer->Y_N2.size());

		const R* buff_in  = (const R*)STARPU_VECTOR_GET_PTR(buffers[0]);
		      Q* buff_out = (      Q*)STARPU_VECTOR_GET_PTR(buffers[1]);

		std::copy(buff_in, buff_in + quantizer->Y_N1.size(), quantizer->Y_N1.begin());
		quantizer->process(quantizer->Y_N1, quantizer->Y_N2);
		std::copy(quantizer->Y_N2.begin(), quantizer->Y_N2.end(), buff_out);
	}
};

template <typename R, typename Q>
starpu_codelet SPU_Quantizer<R,Q>::spu_cl_process = SPU_Quantizer<R,Q>::spu_init_cl_process();

template <typename R, typename Q>
using Quantizer = SPU_Quantizer<R,Q>;
#else
template <typename R, typename Q>
using Quantizer = Quantizer_i<R,Q>;
#endif

#endif /* SPU_QUANTIZER_HPP_ */
