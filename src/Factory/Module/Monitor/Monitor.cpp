#include "Tools/Exception/exception.hpp"

#include "Module/Monitor/Monitor.hpp"

#include "Monitor.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Monitor_name   = "Monitor";
const std::string aff3ct::factory::Monitor_prefix = "mnt";

Monitor::parameters
::parameters(const std::string &prefix)
: Factory::parameters(Monitor_name, Monitor_name, prefix)
{
}

Monitor::parameters
::parameters(const std::string &name, const std::string &prefix)
: Factory::parameters(name, Monitor_name, prefix)
{
}

Monitor::parameters
::~parameters()
{
}

Monitor::parameters* Monitor::parameters
::clone() const
{
	return new Monitor::parameters(*this);
}

void Monitor::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
}

void Monitor::parameters
::store(const arg_val_map &vals)
{
}

void Monitor::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
}
