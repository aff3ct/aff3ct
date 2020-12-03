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
	virtual void notify_frozenbits_update(const std::vector<bool>&) = 0;
};

}
}

#endif // INTERFACE_NOTIFY_FROZENBITS_UPDATE_HPP__