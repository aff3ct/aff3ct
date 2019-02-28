#ifdef AFF3CT_CHANNEL_MKL

#ifndef EVENT_GENERATOR_MKL_HPP
#define EVENT_GENERATOR_MKL_HPP

#include "../Event_generator.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float, typename E = typename tools::matching_types<R>::B>
class Event_generator_MKL : public Event_generator<R,E>
{
private:
	void* stream_state; // VSLStreamStatePtr* type
	bool  is_stream_alloc;

public:
	explicit Event_generator_MKL(const int seed = 0);

	virtual ~Event_generator_MKL();

	virtual void set_seed(const int seed);

	virtual void generate(E *draw, const unsigned length, const R event_probability);
};

}
}

#endif //EVENT_GENERATOR_MKL_HPP

#endif // MKL
