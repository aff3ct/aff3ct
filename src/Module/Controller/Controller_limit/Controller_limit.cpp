#include <string>

#include "Module/Controller/Controller_limit/Controller_limit.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

Controller_limit
::Controller_limit(const size_t limit, const size_t init_path)
: Controller(init_path), limit(limit), counter(0)
{
	const std::string name = "Controller_limit";
	this->set_name(name);
}

Controller_limit* Controller_limit
::clone() const
{
	auto m = new Controller_limit(*this);
	m->deep_copy(*this);
	return m;
}

void Controller_limit
::reset()
{
	Controller::reset();
	this->counter = 0;
}

void Controller_limit
::_control(int8_t *out, const size_t frame_id)
{
	if (this->counter >= this->limit)
		this->path = !init_path;
	out[0] = this->path;
	counter++;
}
