#ifdef CHANNEL_MKL

#include "Tools/Exception/exception.hpp"

#include "Event_generator_MKL.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename R>
Event_generator_MKL<R>
::Event_generator_MKL(const int seed)
: Event_generator<R>(), stream_state(nullptr), is_stream_alloc(false)
{
	this->set_seed(seed);
}

template <typename R>
Event_generator_MKL<R>
::~Event_generator_MKL()
{
	if (is_stream_alloc)
		vslDeleteStream(&stream_state);
}

template <typename R>
void Event_generator_MKL<R>
::set_seed(const int seed)
{
	if (is_stream_alloc) vslDeleteStream(&stream_state);

	//vslNewStream(&stream_state, VSL_BRNG_MT2203, seed);
	vslNewStream(&stream_state, VSL_BRNG_SFMT19937, seed);

	is_stream_alloc = true;
}

template <typename R>
void Event_generator_MKL<R>
::generate(event_type *draw, const unsigned length, const R event_probability)
{
	viRngBernoulli(VSL_RNG_METHOD_BERNOULLI_ICDF, stream_state, length, draw, event_probability);
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
