#ifndef EVENT_GENERATOR_HPP
#define EVENT_GENERATOR_HPP

#include <cstdint>
#include <vector>
#include "../Draw_generator.hpp"

namespace aff3ct
{
namespace tools
{

using event_type = int32_t;

template <typename R = float>
class Event_generator : public Draw_generator<R>
{
public:
	Event_generator() = default;

	virtual ~Event_generator() = default;

	template <class A = std::allocator<event_type>>
	void generate(std::vector<event_type,A> &draw, const R event_probability)
	{
		this->generate(draw.data(), (unsigned)draw.size(), event_probability);
	}

	virtual void generate(event_type *draw, const unsigned length, const R event_probability) = 0;
};

}
}

#endif //EVENT_GENERATOR_HPP
