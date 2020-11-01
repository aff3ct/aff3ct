#include <string>

#include "Module/Monitor/Monitor.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

Monitor
::Monitor()
: Module()
{
	const std::string name = "Monitor";
	this->set_name(name);
	this->set_short_name(name);

	this->tasks_with_nullptr.resize((size_t)mnt::tsk::SIZE);
}

Monitor* Monitor
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

void Monitor
::reset()
{
}

void Monitor
::clear_callbacks()
{
}
