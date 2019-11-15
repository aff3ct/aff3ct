/*!
 * \file
 * \brief Class tools::Event_generator_std.
 */
#ifndef EVENT_GENERATOR_STD_HPP
#define EVENT_GENERATOR_STD_HPP

#include <random>

#include "Tools/types.h"
#include "Tools/Algo/Draw_generator/Event_generator/Event_generator.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R = float, typename E = typename tools::matching_types<R>::B>
class Event_generator_std : public Event_generator<R,E>
{
protected:
	std::mt19937 rd_engine; // Mersenne Twister 19937

public:
	explicit Event_generator_std(const int seed = 0);
	virtual ~Event_generator_std() = default;
	virtual Event_generator_std<R,E>* clone() const;

	virtual void set_seed(const int seed);
	virtual void generate(E *draw, const unsigned length, const R event_probability);
};

}
}

#endif //EVENT_GENERATOR_STD_HPP
