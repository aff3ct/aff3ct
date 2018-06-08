#include "Tools/Exception/exception.hpp"

#include "Monitor.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

Monitor
::Monitor(int n_frames)
: Module(n_frames)
{
	const std::string name = "Monitor";
	this->set_name(name);
	this->set_short_name(name);
}

void Monitor
::clear_callbacks()
{
}

void Monitor
::reset()
{
}

Monitor& Monitor
::operator=(const Monitor& m)
{
	return *this;
}
