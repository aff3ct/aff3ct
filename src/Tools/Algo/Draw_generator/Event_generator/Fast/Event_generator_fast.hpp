#ifndef EVENT_GENERATOR_FAST_HPP
#define EVENT_GENERATOR_FAST_HPP

#include "Tools/Algo/PRNG/PRNG_MT19937.hpp"
#include "Tools/Algo/PRNG/PRNG_MT19937_simd.hpp"

#include "../Event_generator.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R = float>
class Event_generator_fast : public Event_generator<R>
{
private:
	tools::PRNG_MT19937      mt19937;      // Mersenne Twister 19937 (scalar)
	tools::PRNG_MT19937_simd mt19937_simd; // Mersenne Twister 19937 (SIMD)

public:
	explicit Event_generator_fast(const int seed = 0);

	virtual ~Event_generator_fast() = default;

	virtual void set_seed(const int seed);

	virtual void generate(event_type *draw, const unsigned length, const R event_probability);

private:
	inline mipp::Reg<R> get_random_simd();
	inline R            get_random     ();
};

}
}

#endif //EVENT_GENERATOR_FAST_HPP
