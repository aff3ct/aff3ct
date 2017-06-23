#ifndef SPU_CRC_HPP_
#define SPU_CRC_HPP_

#ifdef STARPU
#include <vector>
#include <string>
#include <cassert>
#include <mipp.h>

#include <starpu.h>

namespace aff3ct
{
namespace module
{
template <typename B = int>
class SPU_CRC : public CRC_i<B>
{
private:
	static starpu_codelet spu_cl_build;
	static starpu_codelet spu_cl_extract;

public:
	SPU_CRC(const int K, const int n_frames = 1, const std::string name = "SPU_CRC")
	: CRC_i<B>(K, n_frames, name) {}

	virtual ~SPU_CRC() {}

	static inline starpu_task* spu_task_build(SPU_CRC<B> *crc, starpu_data_handle_t &in_data,
	                                                           starpu_data_handle_t &out_data)
	{
		auto task = starpu_task_create();

		task->name        = "crc::build";
		task->cl          = &SPU_CRC<B>::spu_cl_build;
		task->cl_arg      = (void*)(crc);
		task->cl_arg_size = sizeof(*crc);
		task->handles[0]  = in_data;
		task->handles[1]  = out_data;

		return task;
	}

	static inline starpu_task* spu_task_extract(SPU_CRC<B> *crc, starpu_data_handle_t &in_data,
	                                                             starpu_data_handle_t &out_data)
	{
		auto task = starpu_task_create();

		task->name        = "crc::extract";
		task->cl          = &SPU_CRC<B>::spu_cl_extract;
		task->cl_arg      = (void*)(crc);
		task->cl_arg_size = sizeof(*crc);
		task->handles[0]  = in_data;
		task->handles[1]  = out_data;

		return task;
	}

private:
	static starpu_codelet spu_init_cl_build()
	{
		starpu_codelet cl;

		cl.type              = STARPU_SEQ;
		cl.cpu_funcs     [0] = SPU_CRC<B>::spu_kernel_build;
		cl.cpu_funcs_name[0] = "crc::build::cpu";
		cl.nbuffers          = 2;
		cl.modes         [0] = STARPU_R;
		cl.modes         [1] = STARPU_W;

		return cl;
	}

	static starpu_codelet spu_init_cl_extract()
	{
		starpu_codelet cl;

		cl.type              = STARPU_SEQ;
		cl.cpu_funcs     [0] = SPU_CRC<B>::spu_kernel_extract;
		cl.cpu_funcs_name[0] = "crc::extract::cpu";
		cl.nbuffers          = 2;
		cl.modes         [0] = STARPU_R;
		cl.modes         [1] = STARPU_W;

		return cl;
	}

	static void spu_kernel_build(void *buffers[], void *cl_arg)
	{
		auto crc = static_cast<SPU_CRC<B>*>(cl_arg);

		auto task = starpu_task_get_current();

		auto udata0 = starpu_data_get_user_data(task->handles[0]); assert(udata0);
		auto udata1 = starpu_data_get_user_data(task->handles[1]); assert(udata1);

		auto U_K1 = static_cast<mipp::vector<B>*>(udata0);
		auto U_K2 = static_cast<mipp::vector<B>*>(udata1);

		crc->build(*U_K1, *U_K2);
	}

	static void spu_kernel_extract(void *buffers[], void *cl_arg)
	{
		auto crc = static_cast<SPU_CRC<B>*>(cl_arg);

		auto task = starpu_task_get_current();

		auto udata0 = starpu_data_get_user_data(task->handles[0]); assert(udata0);
		auto udata1 = starpu_data_get_user_data(task->handles[1]); assert(udata1);

		auto V_K1 = static_cast<mipp::vector<B>*>(udata0);
		auto V_K2 = static_cast<mipp::vector<B>*>(udata1);

		crc->extract(*V_K1, *V_K2);
	}
};

template <typename B>
starpu_codelet SPU_CRC<B>::spu_cl_build = SPU_CRC<B>::spu_init_cl_build();

template <typename B>
starpu_codelet SPU_CRC<B>::spu_cl_extract = SPU_CRC<B>::spu_init_cl_extract();

template <typename B>
using CRC = SPU_CRC<B>;
}
}
#else
namespace aff3ct
{
namespace module
{
template <typename B = int>
using CRC = CRC_i<B>;
}
}
#endif

#endif /* SPU_CRC_HPP_ */
