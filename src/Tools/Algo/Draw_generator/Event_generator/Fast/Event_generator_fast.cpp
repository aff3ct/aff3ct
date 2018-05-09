#include <cmath>
#include <mipp.h>

#include "Tools/Exception/exception.hpp"

#include "Event_generator_fast.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename R>
Event_generator_fast<R>
::Event_generator_fast(const int seed)
: Event_generator<R>()
{
	this->set_seed(seed);
}

template <typename R>
void Event_generator_fast<R>
::set_seed(const int seed)
{
	mt19937.seed(seed);

	mipp::vector<int> seeds(mipp::nElReg<int>());
	for (auto i = 0; i < mipp::nElReg<int>(); i++)
		seeds[i] = mt19937.rand();
	mt19937_simd.seed(seeds.data());
}

template <typename R>
mipp::Reg<R> Event_generator_fast<R>
::get_random_simd()
{
	throw runtime_error(__FILE__, __LINE__, __func__, "The MT19937 random generator does not support this type.");
}

template <typename R>
R Event_generator_fast<R>
::get_random()
{
	throw runtime_error(__FILE__, __LINE__, __func__, "The MT19937 random generator does not support this type.");
}

namespace aff3ct
{
namespace tools
{
template <>
mipp::Reg<float> Event_generator_fast<float>
::get_random_simd()
{
	// return a vector of numbers between [0,1]
	return mt19937_simd.randf_cc();
}
}
}

namespace aff3ct
{
namespace tools
{
template <>
float Event_generator_fast<float>
::get_random()
{
	// return a number between  [0,1]
	return mt19937.randf_cc();
}
}
}

template <typename R>
void Event_generator_fast<R>
::generate(event_type *draw, const unsigned length, const R event_probability)
{
	throw runtime_error(__FILE__, __LINE__, __func__, "The MT19937 random generator does not support this type.");
}


namespace aff3ct
{
namespace tools
{
template <>
void Event_generator_fast<float>
::generate(int32_t *draw, const unsigned length, const float event_probability)
{
	const mipp::Reg<float  > r_ep   = event_probability;
	const mipp::Reg<int32_t> r_one  = true;
	const mipp::Reg<int32_t> r_zero = false;

	const unsigned vec_loop_size = (length / mipp::N<float>()) * mipp::N<float>();

	for (unsigned i = 0; i < vec_loop_size; i += mipp::N<float>())
	{
		const auto r_draw  = get_random_simd();
		const auto r_out   = mipp::blend(r_one, r_zero, r_draw <= r_ep);
		r_out.store(draw + i);
	}

	for (auto i = vec_loop_size; i < length; i++)
		draw[i] = get_random() <= event_probability;
}

}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Event_generator_fast<R_32>;
template class aff3ct::tools::Event_generator_fast<R_64>;
#else
template class aff3ct::tools::Event_generator_fast<R>;
#endif
// ==================================================================================== explicit template instantiation
