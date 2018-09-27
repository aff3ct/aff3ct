#ifdef CHANNEL_GSL

#include "Tools/Exception/exception.hpp"

#include "Event_generator_GSL.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename R, typename E>
Event_generator_GSL<R,E>
::Event_generator_GSL(const int seed)
: Event_generator<R,E>(), rng(gsl_rng_alloc(gsl_rng_mt19937), gsl_rng_free)
{
	this->set_seed(seed);
}

template <typename R, typename E>
void Event_generator_GSL<R,E>
::set_seed(const int seed)
{
	gsl_rng_set(rng.get(), seed);
}

template <typename R, typename E>
void Event_generator_GSL<R,E>
::generate(E *draw, const unsigned length, const R event_probability)
{
	for (unsigned i = 0; i < length; i++)
		draw[i] = (E)gsl_ran_bernoulli(rng.get(), (double)event_probability);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Event_generator_GSL<R_32>;
template class aff3ct::tools::Event_generator_GSL<R_64>;
#else
template class aff3ct::tools::Event_generator_GSL<R>;
#endif
// ==================================================================================== explicit template instantiation

#endif // GSL
