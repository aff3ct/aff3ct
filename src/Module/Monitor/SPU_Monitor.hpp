#ifndef SPU_MONITOR_HPP_
#define SPU_MONITOR_HPP_

#ifdef STARPU
#include <vector>
#include <string>
#include <cassert>

#include <starpu.h>

#include "Tools/Perf/MIPP/mipp.h"

template <typename B>
class SPU_Monitor : public Monitor_i<B>
{
private:
	mipp::vector<B> U_K, V_K;

public:
	static starpu_codelet spu_cl_check_errors;

public:
	SPU_Monitor(const int K, const int N, const int n_frames = 1, const std::string name = "SPU_Monitor")
	: Monitor_i<B>(K, N, n_frames, name) {}

	virtual ~SPU_Monitor() {}

	void spu_init()
	{
		if ((int)U_K.size() != this->K * this->n_frames) U_K.resize(this->K * this->n_frames);
		if ((int)V_K.size() != this->K * this->n_frames) V_K.resize(this->K * this->n_frames);
	}

private:
	static starpu_codelet spu_init_cl_check_errors()
	{
		starpu_codelet cl;

		cl.type              = STARPU_SEQ;
		cl.cpu_funcs     [0] = SPU_Monitor<B>::spu_kernel_check_errors;
		cl.cpu_funcs_name[0] = "Monitor::check_errors";
		cl.nbuffers          = 2;
		cl.modes         [0] = STARPU_R;
		cl.modes         [1] = STARPU_W;

		return cl;
	}

	static void spu_kernel_check_errors(void *buffers[], void *cl_arg)
	{
		auto monitor = static_cast<SPU_Monitor<B>*>(cl_arg);
		monitor->spu_init();

		assert(STARPU_VECTOR_GET_NX(buffers[0]) == monitor->U_K.size());
		assert(STARPU_VECTOR_GET_NX(buffers[1]) == monitor->V_K.size());

		const B* buff_in1 = (const B*)STARPU_VECTOR_GET_PTR(buffers[0]);
		const B* buff_in2 = (      B*)STARPU_VECTOR_GET_PTR(buffers[1]);

		std::copy(buff_in1, buff_in1 + monitor->U_K.size(), monitor->U_K.begin());
		std::copy(buff_in2, buff_in2 + monitor->V_K.size(), monitor->V_K.begin());
		monitor->check_errors(monitor->U_K, monitor->V_K);
	}
};

template <typename B>
starpu_codelet SPU_Monitor<B>::spu_cl_check_errors = SPU_Monitor<B>::spu_init_cl_check_errors();

template <typename B>
using Monitor = SPU_Monitor<B>;
#else
template <typename B>
using Monitor = Monitor_i<B>;
#endif

#endif /* SPU_MONITOR_HPP_ */
