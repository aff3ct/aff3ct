#include <type_traits>
#include "Tools/Noise/Erased_value.hpp"
#include "Channel_BEC.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R>
Channel_BEC<R>
::Channel_BEC(const int N, const int seed, const tools::Event_probability<R>& noise, const int n_frames)
: Channel<R>(N, noise, n_frames), uni_dist((R)0, (R)1)
{
	const std::string name = "Channel_BEC";
	this->set_name(name);

	mipp::vector<int> seeds(mipp::nElReg<int>());
	for (auto i = 0; i < mipp::nElReg<int>(); i++)
		seeds[i] = mt19937.rand();
	mt19937_simd.seed(seeds.data());

	rd_engine.seed(seed);
}

template <typename R>
mipp::Reg<R> Channel_BEC<R>
::get_random_simd()
{
	throw tools::runtime_error(__FILE__, __LINE__, __func__, "The MT19937 random generator does not support this type.");
}

template <typename R>
R Channel_BEC<R>
::get_random()
{
	return this->uni_dist(this->rd_engine);
}

namespace aff3ct
{
namespace module
{
template <>
mipp::Reg<float> Channel_BEC<float>
::get_random_simd()
{
	// return a vector of numbers between [0,1]
	return mt19937_simd.randf_cc();
}
}
}

namespace aff3ct
{
namespace module
{
template <>
float Channel_BEC<float>
::get_random()
{
	// return a number between [0,1]
	return mt19937.randf_cc();
}
}
}

template <typename R>
void Channel_BEC<R>
::_add_noise(const R *X_N, R *Y_N, const int frame_id)
{
	this->check_noise();

	const auto erasure_probability = this->n->get_noise();

	const mipp::Reg<R> r_erased = tools::Erased_value<R>::symbol;
	const mipp::Reg<R> r_ep     = erasure_probability;

	const auto vec_loop_size = (std::is_same<R,float>::value) ? ((this->N / mipp::nElReg<R>()) * mipp::nElReg<R>()) : 0;

	for (auto i = 0; i < vec_loop_size; i += mipp::nElReg<R>())
	{
		const auto r_in    = mipp::Reg<R>(X_N + i);
		const auto r_draw  = get_random_simd();
		const auto r_out   = mipp::blend(r_erased, r_in, r_draw <= r_ep);
		r_out.store(Y_N + i);
	}

	for (auto i = vec_loop_size; i < this->N; i++)
		Y_N[i] = get_random() <= erasure_probability ? tools::Erased_value<R>::symbol : X_N[i];
}

template<typename R>
void Channel_BEC<R>::check_noise()
{
	Channel<R>::check_noise();

	this->n->is_of_type_throw(tools::Noise_type::EP);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Channel_BEC<R_32>;
template class aff3ct::module::Channel_BEC<R_64>;
#else
template class aff3ct::module::Channel_BEC<R>;
#endif
// ==================================================================================== explicit template instantiation
