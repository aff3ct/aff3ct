#include <algorithm>
#include <mipp.h>

#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Draw_generator/Event_generator/Fast/Event_generator_fast.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename R, typename E>
Event_generator_fast<R,E>
::Event_generator_fast(const int seed)
: Event_generator<R,E>()
{
	this->set_seed(seed);
}

template <typename R, typename E>
Event_generator_fast<R,E>* Event_generator_fast<R,E>
::clone() const
{
	return new Event_generator_fast(*this);
}

template <typename R, typename E>
void Event_generator_fast<R,E>
::set_seed(const int seed)
{
	mt19937.seed(seed);

	mipp::vector<int> seeds(mipp::nElReg<int>());
	for (auto i = 0; i < mipp::nElReg<int>(); i++)
		seeds[i] = mt19937.rand();
	mt19937_simd.seed(seeds.data());
}

template <typename R, typename E>
void Event_generator_fast<R,E>
::generate(E *draw, const unsigned length, const R event_probability)
{
	throw runtime_error(__FILE__, __LINE__, __func__, "The MT19937 random generator does not support this type.");
}


#include "Tools/types.h"
namespace aff3ct
{
namespace tools
{
template <>
void Event_generator_fast<R_32,B_32>
::generate(B_32 *draw, const unsigned length, const R_32 event_probability)
{
	if (event_probability == (R_32)0.)
	{
		std::fill(draw, draw + length, (B_32)false);
	}
	else
	{
		const mipp::Reg<R_32> r_ep   = event_probability;
		const mipp::Reg<B_32> r_one  = (B_32)true;
		const mipp::Reg<B_32> r_zero = (B_32)false;

		const unsigned vec_loop_size = (length / mipp::N<R_32>()) * mipp::N<R_32>();

		for (unsigned i = 0; i < vec_loop_size; i += mipp::N<R_32>())
		{
			const auto r_draw  = mt19937_simd.randf_cc();
			const auto r_out   = mipp::blend(r_one, r_zero, r_draw <= r_ep);
			r_out.store(draw + i);
		}

		for (auto i = vec_loop_size; i < length; i++)
			draw[i] = mt19937.randf_cc() <= event_probability;
	}
}

}
}

namespace aff3ct
{
namespace tools
{
template <>
void Event_generator_fast<R_64,B_64>
::generate(B_64 *draw, const unsigned length, const R_64 event_probability)
{
	if (event_probability == (R_64)0.)
	{
		std::fill(draw, draw + length, (B_64)false);
	}
	else
	{
		const mipp::Reg<R_64> r_ep   = event_probability;
		const mipp::Reg<B_64> r_one  = (B_64)true;
		const mipp::Reg<B_64> r_zero = (B_64)false;

		const unsigned vec_loop_size = (length / (2*mipp::N<R_64>())) * (2*mipp::N<R_64>());

		for (unsigned i = 0; i < vec_loop_size; i += 2*mipp::N<R_64>())
		{
			const auto r_draw  = mt19937_simd.randf_cc(); // on simple floats

			const auto draw_low  = mipp::cvt<float,R_64>(r_draw.low ());
			const auto draw_high = mipp::cvt<float,R_64>(r_draw.high());

			const auto r_out_low  = mipp::blend(r_one, r_zero, draw_low  <= r_ep);
			const auto r_out_high = mipp::blend(r_one, r_zero, draw_high <= r_ep);
			r_out_low .store(draw + i);
			r_out_high.store(draw + i + mipp::N<R_64>());
		}

		for (auto i = vec_loop_size; i < length; i++)
			draw[i] = (R_64)mt19937.randf_cc() <= event_probability;
	}
}

}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Event_generator_fast<R_32>;
template class aff3ct::tools::Event_generator_fast<R_64>;
#else
template class aff3ct::tools::Event_generator_fast<R>;
#endif
// ==================================================================================== explicit template instantiation
