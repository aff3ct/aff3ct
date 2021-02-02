/*!
 * \file
 * \brief Class tools::Interface_get_set_frozen_bits.
 */
#ifndef Interface_get_set_frozen_bits_HPP__
#define Interface_get_set_frozen_bits_HPP__

#include <vector>

namespace aff3ct
{
namespace tools
{
class Interface_get_set_frozen_bits
{
public:
	virtual void set_frozen_bits(const std::vector<bool>& frozen_bits) = 0;
	virtual const std::vector<bool>& get_frozen_bits() const = 0;
};
}
}

#endif // Interface_get_set_frozen_bits_HPP__