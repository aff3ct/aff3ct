#ifndef ERASED_PROBABILITY_HPP_
#define ERASED_PROBABILITY_HPP_

#include "Noise.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R = float>
class Erasure_probability : public Noise<R>
{
public:
	Erasure_probability() = default;
	explicit Erasure_probability(R noise);
	virtual ~Erasure_probability() = default;

	virtual Noise_type get_type() const;

	virtual Erasure_probability<R>* clone() const;

	template <typename T>
	Erasure_probability<T>* cast() const
	{
		Erasure_probability<T>* ep;

		if (this->is_set())
			ep = new Erasure_probability<T>((T)this->_n.first);
		else
			ep = new Erasure_probability<T>();

		return ep;
	}

protected:
	virtual void check();
};

template <typename R = float>
using EP = Erasure_probability<R>;

}
}

#endif //ERASED_PROBABILITY_HPP_
