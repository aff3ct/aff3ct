/*!
 * \file
 * \brief Class tools::Interface_notify_frozenbits_update.
 */
#ifndef INTERFACE_NOTIFY_FROZENBITS_UPDATE_HPP__
#define INTERFACE_NOTIFY_FROZENBITS_UPDATE_HPP__

#include "Tools/Interface/Interface_notify_noise_update.hpp"

namespace aff3ct
{
namespace tools
{
class Interface_notify_frozenbits_update : public Interface_notify_noise_update
{
public:
	virtual void notify_noise_update() = 0;
};

}
}

#endif // INTERFACE_NOTIFY_FROZENBITS_UPDATE_HPP__