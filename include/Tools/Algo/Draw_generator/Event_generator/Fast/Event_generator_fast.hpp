/*!
 * \file
 * \brief Class tools::Event_generator_fast.
 */
#ifndef EVENT_GENERATOR_FAST_HPP
#define EVENT_GENERATOR_FAST_HPP

#include "Tools/types.h"
#include "Tools/Algo/PRNG/PRNG_MT19937.hpp"
#include "Tools/Algo/PRNG/PRNG_MT19937_simd.hpp"
#include "Tools/Algo/Draw_generator/Event_generator/Event_generator.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R = float, typename E = typename tools::matching_types<R>::B>
class Event_generator_fast : public Event_generator<R,E>
{
	static_assert(sizeof(R) == sizeof(E), "R and E have to represent the same number of bits.");

private:
	tools::PRNG_MT19937      mt19937;      // Mersenne Twister 19937 (scalar)
	tools::PRNG_MT19937_simd mt19937_simd; // Mersenne Twister 19937 (SIMD)

public:
	explicit Event_generator_fast(const int seed = 0);
	virtual ~Event_generator_fast() = default;
	virtual Event_generator_fast<R,E>* clone() const;

	virtual void set_seed(const int seed);
	virtual void generate(E *draw, const unsigned length, const R event_probability);
};

}
}

#endif //EVENT_GENERATOR_FAST_HPP
