/*!
 * \file
 * \brief Class tools::Event_probability.
 */
#ifndef ERASED_PROBABILITY_HPP_
#define ERASED_PROBABILITY_HPP_

#include <string>

#include "Tools/Noise/Noise.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R = float>
class Event_probability : public Noise<R>
{
public:
	Event_probability() = default;
	explicit Event_probability(R value);
	virtual ~Event_probability() = default;

	virtual std::string get_unity() const;
	virtual Noise_type get_type() const;

protected:
	virtual void check();
};

template <typename R = float>
using EP = Event_probability<R>;

}
}

#endif //ERASED_PROBABILITY_HPP_
