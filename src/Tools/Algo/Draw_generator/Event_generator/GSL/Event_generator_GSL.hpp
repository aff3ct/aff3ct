#ifdef CHANNEL_GSL

#ifndef EVENT_GENERATOR_GSL_HPP
#define EVENT_GENERATOR_GSL_HPP

#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>

#include "../Event_generator.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R = float>
class Event_generator_GSL : public Event_generator<R>
{
private:
	gsl_rng *rng;

public:
	explicit Event_generator_GSL(const int seed = 0);

	virtual ~Event_generator_GSL();

	virtual void set_seed(const int seed);

	virtual void generate(event_type *draw, const unsigned length, const R event_probability);
};

}
}

#endif //EVENT_GENERATOR_GSL_HPP

#endif // GSL
