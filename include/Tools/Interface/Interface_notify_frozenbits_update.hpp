/*!
 * \file
 * \brief Class tools::Interface_notify_frozenbits_update.
 */
#ifndef INTERFACE_NOTIFY_FROZENBITS_UPDATE_HPP__
#define INTERFACE_NOTIFY_FROZENBITS_UPDATE_HPP__

#include <vector>

namespace aff3ct
{
namespace tools
{
class Interface_notify_frozenbits_update
{
public:
	virtual void notify_frozenbits_update(const std::vector<bool>& frozen_bits) = 0;
	virtual const std::vector<bool>& get_frozen_bits() const = 0;
};

}
}

#endif // INTERFACE_NOTIFY_FROZENBITS_UPDATE_HPP__