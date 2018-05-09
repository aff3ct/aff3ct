
#include "Event_generator_std.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename R>
Event_generator_std<R>
::Event_generator_std(const int seed)
: Event_generator<R>(), uni_dist((R)0, (R)1)
{
	this->set_seed(seed);
}

template <typename R>
void Event_generator_std<R>
::set_seed(const int seed)
{
	rd_engine.seed(seed);
}

template <typename R>
void Event_generator_std<R>
::generate(R *draw, const unsigned length, const R event_probability)
{
	for (unsigned i = 0; i < length; i++)
		draw[i] = this->uni_dist(this->rd_engine) <= event_probability;
}


// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Event_generator_std<R_32>;
template class aff3ct::tools::Event_generator_std<R_64>;
#else
template class aff3ct::tools::Event_generator_std<R>;
#endif
// ==================================================================================== explicit template instantiation
