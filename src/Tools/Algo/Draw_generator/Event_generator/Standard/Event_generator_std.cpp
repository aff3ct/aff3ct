#include "Tools/Algo/Draw_generator/Event_generator/Standard/Event_generator_std.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename R, typename E>
Event_generator_std<R,E>
::Event_generator_std(const int seed)
: Event_generator<R,E>()
{
	this->set_seed(seed);
}

template <typename R, typename E>
Event_generator_std<R,E>* Event_generator_std<R,E>
::clone() const
{
	return new Event_generator_std(*this);
}

template <typename R, typename E>
void Event_generator_std<R,E>
::set_seed(const int seed)
{
	rd_engine.seed(seed);
}

template <typename R, typename E>
void Event_generator_std<R,E>
::generate(E *draw, const unsigned length, const R event_probability)
{
	std::bernoulli_distribution bern_dist(event_probability);

	for (unsigned i = 0; i < length; i++)
		draw[i] = (E)bern_dist(this->rd_engine);
}


// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Event_generator_std<R_32>;
template class aff3ct::tools::Event_generator_std<R_64>;
#else
template class aff3ct::tools::Event_generator_std<R>;
#endif
// ==================================================================================== explicit template instantiation
