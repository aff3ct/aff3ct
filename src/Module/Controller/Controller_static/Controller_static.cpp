#include <string>

#include "Module/Controller/Controller_static/Controller_static.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

Controller_static
::Controller_static(const size_t init_path)
: Controller(init_path)
{
	const std::string name = "Controller_static";
	this->set_name(name);
}

Controller_static* Controller_static
::clone() const
{
	auto m = new Controller_static(*this);
	m->deep_copy(*this);
	return m;
}

void Controller_static
::_control(int8_t *out, const size_t frame_id)
{
	out[0] = this->path;
}
