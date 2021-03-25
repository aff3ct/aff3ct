/*!
 * \file
 * \brief Class module::Controller_cyclic.
 */
#ifndef CONTROLLER_CYCLIC_HPP_
#define CONTROLLER_CYCLIC_HPP_

#include <cstdint>

#include "Module/Controller/Controller.hpp"

namespace aff3ct
{
namespace module
{

class Controller_cyclic : public Controller
{
protected:
	size_t n_paths;

public:
	Controller_cyclic(const size_t n_paths, const size_t init_path = 0);
	virtual ~Controller_cyclic() = default;
	virtual Controller_cyclic* clone() const;

	virtual void reset();

	size_t get_n_paths() const;
	void set_n_paths(const size_t n_paths);
	void set_path(const size_t path);

protected:
	virtual void _control(int8_t *out, const size_t frame_id);
};
}
}

#endif /* CONTROLLER_CYCLIC_HPP_ */
