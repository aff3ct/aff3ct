#ifdef CHANNEL_MKL

#include "Tools/Exception/exception.hpp"

#include "Event_generator_MKL.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename R, typename E>
Event_generator_MKL<R,E>
::Event_generator_MKL(const int seed)
: Event_generator<R,E>(), stream_state(nullptr), is_stream_alloc(false)
{
	this->set_seed(seed);
}

template <typename R, typename E>
Event_generator_MKL<R,E>
::~Event_generator_MKL()
{
	if (is_stream_alloc)
		vslDeleteStream(&stream_state);
}

template <typename R, typename E>
void Event_generator_MKL<R,E>
::set_seed(const int seed)
{
	if (is_stream_alloc) vslDeleteStream(&stream_state);

	//vslNewStream(&stream_state, VSL_BRNG_MT2203, seed);
	vslNewStream(&stream_state, VSL_BRNG_SFMT19937, seed);

	is_stream_alloc = true;
}

template <typename R, typename E>
void Event_generator_MKL<R,E>
::generate(E *draw, const unsigned length, const R event_probability)
{
	throw runtime_error(__FILE__, __LINE__, __func__, "The MKL viRngBernoulli random generator does not support this type.");
}

namespace aff3ct
{
namespace tools
{
template <>
void Event_generator_MKL<R_32,B_32>
::generate(B_32 *draw, const unsigned length, const R_32 event_probability)
{
	viRngBernoulli(VSL_RNG_METHOD_BERNOULLI_ICDF, stream_state, length, draw, event_probability);
}

template <>
void Event_generator_MKL<R_64,B_64>
::generate(B_64 *draw, const unsigned length, const R_64 event_probability)
{
	std::vector<int> draw_i(length);
	viRngBernoulli(VSL_RNG_METHOD_BERNOULLI_ICDF, stream_state, length, draw_i.data(), event_probability);

	std::copy(draw_i.begin(), draw_i.end(), draw);
}
}
}


// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Event_generator_MKL<R_32>;
template class aff3ct::tools::Event_generator_MKL<R_64>;
#else
template class aff3ct::tools::Event_generator_MKL<R>;
#endif
// ==================================================================================== explicit template instantiation

#endif // MKL
