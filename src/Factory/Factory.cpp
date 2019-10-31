#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Factory/Factory.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Factory_name       = "Factory";
const std::string aff3ct::factory::Factory_short_name = "Factory";
const std::string aff3ct::factory::Factory_prefix     = "fac";

Factory
::Factory(const std::string &name, const std::string &short_name, const std::string &prefix)
: name(name), short_name(short_name), prefix(prefix)
{
}

std::string Factory
::get_name() const
{
	return this->name;
}

std::string Factory
::get_short_name() const
{
	return this->short_name;
}

std::string Factory
::get_prefix() const
{
	return this->prefix;
}

std::vector<std::string> Factory
::get_names() const
{
	std::vector<std::string> n;
	n.push_back(this->name);
	return n;
}
std::vector<std::string> Factory
::get_short_names() const
{
	std::vector<std::string> sn;
	sn.push_back(this->short_name);
	return sn;
}

std::vector<std::string> Factory
::get_prefixes() const
{
	std::vector<std::string> p;
	p.push_back(this->prefix);
	return p;
}

cli::Argument_map_info Factory
::get_description(const std::vector<Factory*> &factories)
{
	cli::Argument_map_info args;

	get_description(factories, args);

	return args;
}

void Factory
::get_description(const std::vector<Factory*> &factories, cli::Argument_map_info &args)
{
	for (auto *f : factories)
		f->get_description(args);
}

void Factory
::store(std::vector<Factory*> &factories, const cli::Argument_map_value &vals)
{
	for (auto *f : factories)
		f->store(vals);
}

cli::Argument_map_group Factory
::create_groups(const std::vector<Factory*> &factories)
{
	// create groups of arguments
	cli::Argument_map_group grps;
	for (auto *f : factories)
	{
		auto prefixes    = f->get_prefixes   ();
		auto short_names = f->get_short_names();

		if (prefixes.size() != short_names.size())
		{
			std::stringstream message;
			message << "'prefixes.size()' has to be equal to 'short_names.size()' ('prefixes.size()' = "
			        << prefixes.size() << ", 'short_names.size()' = " << short_names.size() << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		for (size_t i = 0; i < prefixes.size(); i++)
			grps[prefixes[i]] = short_names[i] + " parameter(s)";
	}

	return grps;
}