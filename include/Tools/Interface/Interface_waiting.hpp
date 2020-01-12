/*!
 * \file
 * \brief Class tools::Interface_waiting.
 */
#ifndef INTERFACE_WAITING_HPP__
#define INTERFACE_WAITING_HPP__

#include "Tools/Interface/Interface_reset.hpp"

namespace aff3ct
{
namespace tools
{
class Interface_waiting : public Interface_reset
{
public:
	virtual void reset() = 0;
	virtual void send_cancel_signal() = 0;
	virtual void wake_up() = 0;
	virtual void cancel_waiting() = 0;
};

}
}

#endif // INTERFACE_WAITING_HPP__