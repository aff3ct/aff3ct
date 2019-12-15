/*!
 * \file
 * \brief Class tools::Event_generator_GSL.
 */
#ifdef AFF3CT_CHANNEL_GSL

#ifndef EVENT_GENERATOR_GSL_HPP
#define EVENT_GENERATOR_GSL_HPP

#include "Tools/types.h"
#include "Tools/Algo/Draw_generator/Event_generator/Event_generator.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R = float, typename E = typename tools::matching_types<R>::B>
class Event_generator_GSL : public Event_generator<R,E>
{
private:
	void* rng; // gsl_rng* type

public:
	explicit Event_generator_GSL(const int seed = 0);
	virtual ~Event_generator_GSL();
	virtual Event_generator_GSL<R,E>* clone() const;

	virtual void set_seed(const int seed);
	virtual void generate(E *draw, const unsigned length, const R event_probability);
};

}
}

#endif //EVENT_GENERATOR_GSL_HPP

#endif // GSL
