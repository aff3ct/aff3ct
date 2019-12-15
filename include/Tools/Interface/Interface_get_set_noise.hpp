/*!
 * \file
 * \brief Class tools::Interface_get_set_noise.
 */
#ifndef INTERFACE_GET_SET_NOISE_HPP__
#define INTERFACE_GET_SET_NOISE_HPP__

#include "Tools/Noise/Noise.hpp"

namespace aff3ct
{
namespace tools
{
class Interface_get_set_noise
{
public:
	virtual const tools::Noise<>& get_noise() const = 0;
	virtual void set_noise(const tools::Noise<>& noise) = 0;
};

}
}

#endif // INTERFACE_GET_SET_NOISE_HPP__