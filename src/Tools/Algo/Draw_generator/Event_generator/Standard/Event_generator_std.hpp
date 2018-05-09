#ifndef EVENT_GENERATOR_STD_HPP
#define EVENT_GENERATOR_STD_HPP

#include <random>

#include "../Event_generator.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R = float>
class Event_generator_std : public Event_generator<R>
{
protected:
	std::mt19937                      rd_engine; // Mersenne Twister 19937
	std::uniform_real_distribution<R> uni_dist;

public:
	explicit Event_generator_std(const int seed = 0);

	virtual ~Event_generator_std() = default;

	virtual void set_seed(const int seed);

	virtual void generate(R *draw, const unsigned length, const R event_probability);
};

}
}

#endif //EVENT_GENERATOR_STD_HPP
