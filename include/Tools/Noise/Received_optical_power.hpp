/*!
 * \file
 * \brief Class tools::Received_optical_power.
 */
#ifndef RECEIVED_OPTICAL_POWER_HPP_
#define RECEIVED_OPTICAL_POWER_HPP_

#include "Tools/Noise/Noise.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R = float>
class Received_optical_power : public Noise<R>
{
public:
	Received_optical_power() = default;
	explicit Received_optical_power(R value);
	virtual ~Received_optical_power() = default;

	virtual std::string get_unity() const;
	virtual Noise_type get_type() const;
};

template <typename R = float>
using ROP = Received_optical_power<R>;

}
}

#endif //RECEIVED_OPTICAL_POWER_HPP_
