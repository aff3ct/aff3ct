#include "Tools/Algo/Draw_generator/Event_generator/Event_generator.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R, typename E>
template <class A>
void Event_generator<R,E>::generate(std::vector<E,A> &draw, const R event_probability)
{
	this->generate(draw.data(), (unsigned)draw.size(), event_probability);
}
}
}
