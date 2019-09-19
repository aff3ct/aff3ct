#ifndef EVENT_GENERATOR_HPP
#define EVENT_GENERATOR_HPP

#include <memory>
#include <vector>

#include "Tools/types.h"
#include "Tools/Algo/Draw_generator/Draw_generator.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float, typename E = typename tools::matching_types<R>::B>
class Event_generator : public Draw_generator<R>
{
public:
	Event_generator() = default;

	virtual ~Event_generator() = default;

	template <class A = std::allocator<E>>
	void generate(std::vector<E,A> &draw, const R event_probability);

	virtual void generate(E *draw, const unsigned length, const R event_probability) = 0;
};
}
}

#include "Tools/Algo/Draw_generator/Event_generator/Event_generator.hxx"

#endif //EVENT_GENERATOR_HPP
