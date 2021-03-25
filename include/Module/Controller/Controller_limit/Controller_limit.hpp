/*!
 * \file
 * \brief Class module::Controller_limit.
 */
#ifndef CONTROLLER_LIMIT_HPP_
#define CONTROLLER_LIMIT_HPP_

#include <cstdint>

#include "Module/Controller/Controller.hpp"

namespace aff3ct
{
namespace module
{

class Controller_limit : public Controller
{
protected:
	const size_t limit;
	size_t counter;

public:
	Controller_limit(const size_t limit, const size_t init_path = 0);
	virtual ~Controller_limit() = default;
	virtual Controller_limit* clone() const;

	virtual void reset();

protected:
	virtual void _control(int8_t *out, const size_t frame_id);
};
}
}

#endif /* CONTROLLER_LIMIT_HPP_ */
