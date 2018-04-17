#ifndef RECEIVED_OPTICAL_POWER_HPP_
#define RECEIVED_OPTICAL_POWER_HPP_

#include "Noise.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R = float>
class Received_optical_power : public Noise<R>
{
public:
	Received_optical_power() = default;
	explicit Received_optical_power(R noise);
	virtual ~Received_optical_power() = default;

	virtual Noise_type get_type() const;

	virtual Received_optical_power<R>* clone() const;

	template <typename T>
	Received_optical_power<T>* cast() const
	{
		Received_optical_power<T>* rop;

		if (this->is_set())
			rop = new Received_optical_power<T>((T)this->_n.first);
		else
			rop = new Received_optical_power<T>();

		return rop;
	}
};

template <typename R = float>
using ROP = Received_optical_power<R>;

}
}

#endif //RECEIVED_OPTICAL_POWER_HPP_
