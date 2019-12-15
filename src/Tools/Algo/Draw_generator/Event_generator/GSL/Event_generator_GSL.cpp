#ifdef AFF3CT_CHANNEL_GSL

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include "Tools/Algo/Draw_generator/Event_generator/GSL/Event_generator_GSL.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename R, typename E>
Event_generator_GSL<R,E>
::Event_generator_GSL(const int seed)
: Event_generator<R,E>(), rng((void*)gsl_rng_alloc(gsl_rng_mt19937))
{
	this->set_seed(seed);
}

template <typename R, typename E>
Event_generator_GSL<R,E>
::~Event_generator_GSL()
{
	gsl_rng_free((gsl_rng*)rng);
}

template <typename R, typename E>
Event_generator_GSL<R,E>* Event_generator_GSL<R,E>
::clone() const
{
	Event_generator_GSL<R,E>* eg = new Event_generator_GSL(*this);
	eg->rng = (void*)gsl_rng_alloc(gsl_rng_mt19937);
	eg->set_seed((int)gsl_rng_get((gsl_rng*)this->rng));
	return eg;
}

template <typename R, typename E>
void Event_generator_GSL<R,E>
::set_seed(const int seed)
{
	gsl_rng_set((gsl_rng*)rng, seed);
}

template <typename R, typename E>
void Event_generator_GSL<R,E>
::generate(E *draw, const unsigned length, const R event_probability)
{
	for (unsigned i = 0; i < length; i++)
		draw[i] = (E)gsl_ran_bernoulli((gsl_rng*)rng, (double)event_probability);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Event_generator_GSL<R_32>;
template class aff3ct::tools::Event_generator_GSL<R_64>;
#else
template class aff3ct::tools::Event_generator_GSL<R>;
#endif
// ==================================================================================== explicit template instantiation

#endif // GSL
