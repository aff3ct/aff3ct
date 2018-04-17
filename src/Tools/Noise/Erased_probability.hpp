#ifndef ERASED_PROBABILITY_HPP_
#define ERASED_PROBABILITY_HPP_

#include "Noise.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R = float>
class Erased_probability : public Noise<R>
{
public:
	Erased_probability() = default;
	explicit Erased_probability(R noise);
	virtual ~Erased_probability() = default;

	virtual Noise_type get_type() const;

	virtual Erased_probability<R>* clone() const;

	template <typename T>
	Erased_probability<T>* cast() const
	{
		Erased_probability<T>* ep;

		if (this->is_set())
			ep = new Erased_probability<T>((T)this->_n.first);
		else
			ep = new Erased_probability<T>();

		return ep;
	}

protected:
	virtual void check();
};

template <typename R = float>
using EP = Erased_probability<R>;

}
}

#endif //ERASED_PROBABILITY_HPP_
