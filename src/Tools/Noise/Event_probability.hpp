#ifndef ERASED_PROBABILITY_HPP_
#define ERASED_PROBABILITY_HPP_

#include "Noise.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R = float>
class Event_probability : public Noise<R>
{
public:
	Event_probability() = default;
	explicit Event_probability(R noise);
	template<typename T>
	explicit Event_probability(const Event_probability<T>& other);
	virtual ~Event_probability() = default;

	virtual std::string get_unity() const;
	virtual Noise_type get_type() const;

	virtual Event_probability<R>* clone() const;

protected:
	virtual void check();
};

template <typename R = float>
using EP = Event_probability<R>;

}
}

#endif //ERASED_PROBABILITY_HPP_
