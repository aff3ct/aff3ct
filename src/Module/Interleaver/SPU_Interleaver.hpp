#ifndef SPU_INTERLEAVER_HPP_
#define SPU_INTERLEAVER_HPP_

#ifdef STARPU
#include <vector>
#include <string>
#include <cassert>

#include <starpu.h>

#include "Tools/Perf/MIPP/mipp.h"

template <typename T>
class SPU_Interleaver : public Interleaver_i<T>
{
private:
	mipp::vector<char     > natural_vec_1, interleaved_vec_1;
	mipp::vector<short    > natural_vec_2, interleaved_vec_2;
	mipp::vector<int      > natural_vec_4, interleaved_vec_4;
	mipp::vector<long long> natural_vec_8, interleaved_vec_8;

public:
	static starpu_codelet spu_cl_interleave;
	static starpu_codelet spu_cl_deinterleave;

public:
	SPU_Interleaver(const int size, const int n_frames = 1, const std::string name = "SPU_Interleaver")
	: Interleaver_i<T>(size, n_frames, name) {}

	virtual ~SPU_Interleaver() {}

	void spu_init()
	{
		const int size = this->pi.size();
		if ((int)natural_vec_1    .size() != size * this->n_frames) natural_vec_1    .resize(size * this->n_frames);
		if ((int)natural_vec_2    .size() != size * this->n_frames) natural_vec_2    .resize(size * this->n_frames);
		if ((int)natural_vec_4    .size() != size * this->n_frames) natural_vec_4    .resize(size * this->n_frames);
		if ((int)natural_vec_8    .size() != size * this->n_frames) natural_vec_8    .resize(size * this->n_frames);
		if ((int)interleaved_vec_1.size() != size * this->n_frames) interleaved_vec_1.resize(size * this->n_frames);
		if ((int)interleaved_vec_2.size() != size * this->n_frames) interleaved_vec_2.resize(size * this->n_frames);
		if ((int)interleaved_vec_4.size() != size * this->n_frames) interleaved_vec_4.resize(size * this->n_frames);
		if ((int)interleaved_vec_8.size() != size * this->n_frames) interleaved_vec_8.resize(size * this->n_frames);
	}

private:
	static starpu_codelet spu_init_cl_interleave()
	{
		starpu_codelet cl;

		cl.type              = STARPU_SEQ;
		cl.cpu_funcs     [0] = SPU_Interleaver<T>::spu_kernel_interleave;
		cl.cpu_funcs_name[0] = "Interleaver::interleave";
		cl.nbuffers          = 2;
		cl.modes         [0] = STARPU_R;
		cl.modes         [1] = STARPU_W;

		return cl;
	}

	static starpu_codelet spu_init_cl_deinterleave()
	{
		starpu_codelet cl;

		cl.type              = STARPU_SEQ;
		cl.cpu_funcs     [0] = SPU_Interleaver<T>::spu_kernel_deinterleave;
		cl.cpu_funcs_name[0] = "Interleaver::interleave";
		cl.nbuffers          = 2;
		cl.modes         [0] = STARPU_R;
		cl.modes         [1] = STARPU_W;

		return cl;
	}

	static void spu_kernel_interleave(void *buffers[], void *cl_arg)
	{
		auto itl = static_cast<SPU_Interleaver<T>*>(cl_arg);

		switch (STARPU_VARIABLE_GET_ELEMSIZE(buffers[0]))
		{
			case 1: _spu_kernel_interleave<char     >(buffers, itl, itl->natural_vec_1, itl->interleaved_vec_1); break;
			case 2: _spu_kernel_interleave<short    >(buffers, itl, itl->natural_vec_2, itl->interleaved_vec_2); break;
			case 4: _spu_kernel_interleave<int      >(buffers, itl, itl->natural_vec_4, itl->interleaved_vec_4); break;
			case 8: _spu_kernel_interleave<long long>(buffers, itl, itl->natural_vec_8, itl->interleaved_vec_8); break;
			default:
				std::cerr << "(EE) Unrecognized type of data, exiting." << std::endl;
				std::exit(-1);
				break;
		}
	}

	static void spu_kernel_deinterleave(void *buffers[], void *cl_arg)
	{
		auto itl = static_cast<SPU_Interleaver<T>*>(cl_arg);

		switch (STARPU_VARIABLE_GET_ELEMSIZE(buffers[0]))
		{
			case 1: _spu_kernel_deinterleave<char     >(buffers, itl, itl->interleaved_vec_1, itl->natural_vec_1); break;
			case 2: _spu_kernel_deinterleave<short    >(buffers, itl, itl->interleaved_vec_2, itl->natural_vec_2); break;
			case 4: _spu_kernel_deinterleave<int      >(buffers, itl, itl->interleaved_vec_4, itl->natural_vec_4); break;
			case 8: _spu_kernel_deinterleave<long long>(buffers, itl, itl->interleaved_vec_8, itl->natural_vec_8); break;
			default:
				std::cerr << "(EE) Unrecognized type of data, exiting." << std::endl;
				std::exit(-1);
				break;
		}
	}

	template <typename D>
	static void _spu_kernel_interleave(void *buffers[],
	                                   SPU_Interleaver<T>* interleaver,
	                                   mipp::vector<D> &natural_vec,
	                                   mipp::vector<D> &interleaved_vec)
	{
		interleaver->spu_init();

		assert(STARPU_VECTOR_GET_NX(buffers[0]) == natural_vec    .size());
		assert(STARPU_VECTOR_GET_NX(buffers[1]) == interleaved_vec.size());

		const D* buff_in  = (const D*)STARPU_VECTOR_GET_PTR(buffers[0]);
		      D* buff_out = (      D*)STARPU_VECTOR_GET_PTR(buffers[1]);

		std::copy(buff_in, buff_in + natural_vec.size(), natural_vec.begin());
		interleaver->interleave(natural_vec, interleaved_vec);
		std::copy(interleaved_vec.begin(), interleaved_vec.end(), buff_out);
	}

	template <typename D>
	static void _spu_kernel_deinterleave(void *buffers[],
	                                     SPU_Interleaver<T>* interleaver,
	                                     mipp::vector<D> &interleaved_vec,
	                                     mipp::vector<D> &natural_vec)
	{
		interleaver->spu_init();

		assert(STARPU_VECTOR_GET_NX(buffers[0]) == interleaved_vec.size());
		assert(STARPU_VECTOR_GET_NX(buffers[1]) == natural_vec    .size());

		const D* buff_in  = (const D*)STARPU_VECTOR_GET_PTR(buffers[0]);
		      D* buff_out = (      D*)STARPU_VECTOR_GET_PTR(buffers[1]);

		std::copy(buff_in, buff_in + interleaved_vec.size(), interleaved_vec.begin());
		interleaver->deinterleave(interleaved_vec, natural_vec);
		std::copy(natural_vec.begin(), natural_vec.end(), buff_out);
	}
};

template <typename T>
starpu_codelet SPU_Interleaver<T>::spu_cl_interleave = SPU_Interleaver<T>::spu_init_cl_interleave();

template <typename T>
starpu_codelet SPU_Interleaver<T>::spu_cl_deinterleave = SPU_Interleaver<T>::spu_init_cl_deinterleave();

template <typename T>
using Interleaver = SPU_Interleaver<T>;
#else
template <typename T>
using Interleaver = Interleaver_i<T>;
#endif

#endif /* SPU_INTERLEAVER_HPP_ */
