#ifndef EVENT_GENERATOR_HPP
#define EVENT_GENERATOR_HPP

#include "../Draw_generator.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R = float>
class Event_generator : public Draw_generator<R>
{
public:
	Event_generator() = default;

	virtual ~Event_generator() = default;

	virtual void generate(R *draw, const unsigned length, const R event_probability) = 0;
};

}
}

#endif //EVENT_GENERATOR_HPP
