#include <utility>

#include "Tools/Exception/exception.hpp"
#include "Module/Monitor/Monitor.hpp"
#include "Factory/Module/Monitor/Monitor.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Monitor_name   = "Monitor";
const std::string aff3ct::factory::Monitor_prefix = "mnt";

Monitor
::Monitor(const std::string &prefix)
: Factory(Monitor_name, Monitor_name, prefix)
{
}

Monitor
::Monitor(const std::string &name, const std::string &prefix)
: Factory(name, Monitor_name, prefix)
{
}

Monitor* Monitor
::clone() const
{
	return new Monitor(*this);
}

void Monitor
::get_description(cli::Argument_map_info &args) const
{
}

void Monitor
::store(const cli::Argument_map_value &vals)
{
}

void Monitor
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
}
