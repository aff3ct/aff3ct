/*!
 * \file
 * \brief Class module::Controller_static.
 */
#ifndef CONTROLLER_STATIC_HPP_
#define CONTROLLER_STATIC_HPP_

#include <cstdint>

#include "Module/Controller/Controller.hpp"

namespace aff3ct
{
namespace module
{

class Controller_static : public Controller
{
public:
	Controller_static(const size_t init_path = 0);
	virtual ~Controller_static() = default;
	virtual Controller_static* clone() const;

protected:
	virtual void _control(int8_t *out, const size_t frame_id);
};
}
}

#endif /* CONTROLLER_STATIC_HPP_ */
