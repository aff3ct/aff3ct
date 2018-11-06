#ifdef AFF3CT_CHANNEL_GSL

#ifndef EVENT_GENERATOR_GSL_HPP
#define EVENT_GENERATOR_GSL_HPP

#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>
#include <memory>

#include "../Event_generator.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R = float, typename E = typename tools::matching_types<R>::B>
class Event_generator_GSL : public Event_generator<R,E>
{
private:
	std::unique_ptr<gsl_rng,decltype(&gsl_rng_free)> rng;

public:
	explicit Event_generator_GSL(const int seed = 0);

	virtual ~Event_generator_GSL() = default;

	virtual void set_seed(const int seed);

	virtual void generate(E *draw, const unsigned length, const R event_probability);
};

}
}

#endif //EVENT_GENERATOR_GSL_HPP

#endif // GSL
