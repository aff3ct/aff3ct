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
	template<typename T>
	explicit Received_optical_power(const Received_optical_power<T>& other);
	virtual ~Received_optical_power() = default;

	virtual std::string get_unity() const;
	virtual Noise_type get_type() const;

	virtual Received_optical_power<R>* clone() const;
};

template <typename R = float>
using ROP = Received_optical_power<R>;

}
}

#endif //RECEIVED_OPTICAL_POWER_HPP_
