/*!
 * \file
 * \brief Class tools::Event_generator.
 */
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
#if defined(AFF3CT_CHANNEL_GSL) && defined(AFF3CT_CHANNEL_MKL)
enum class Event_generator_implem { STD, FAST, GSL, MKL };
#elif defined(AFF3CT_CHANNEL_GSL)
enum class Event_generator_implem { STD, FAST, GSL };
#elif defined(AFF3CT_CHANNEL_MKL)
enum class Event_generator_implem { STD, FAST, MKL };
#else
enum class Event_generator_implem { STD, FAST };
#endif

template <typename R = float, typename E = typename tools::matching_types<R>::B>
class Event_generator : public Draw_generator<R>
{
public:
	Event_generator() = default;
	virtual ~Event_generator() = default;
	virtual Event_generator<R,E>* clone() const = 0;

	template <class A = std::allocator<E>>
	void generate(std::vector<E,A> &draw, const R event_probability);

	virtual void generate(E *draw, const unsigned length, const R event_probability) = 0;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Algo/Draw_generator/Event_generator/Event_generator.hxx"
#endif

#endif //EVENT_GENERATOR_HPP
