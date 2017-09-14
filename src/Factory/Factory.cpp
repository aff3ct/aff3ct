#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
#include <map>

#include "Tools/general_utils.h"

#include "Factory.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

bool aff3ct::factory::exist(const arg_val_map &vals, const std::vector<std::string> &tags)
{
	return (vals.find(tags) != vals.end());
}

void aff3ct::factory::Header::print_parameters(std::string grp_key, std::string grp_name, header_list header,
                                               int max_n_chars, std::ostream& stream)
{
	auto key = tools::string_split(grp_key, '-');

	if (key.size() == 1)
	{
		stream << "# * " << tools::format(grp_name, tools::Style::BOLD | tools::Style::UNDERLINED) << " ";
		for (auto i = 0; i < 46 - (int)grp_name.length(); i++) std::cout << "-";
		stream << std::endl;
	}
	else if (key.size() > 1)
	{
		stream << "#    " << tools::format(grp_name, tools::Style::BOLD | tools::Style::UNDERLINED) << " ";
		for (auto i = 0; i < 45 - (int)grp_name.length(); i++) std::cout << "-";
		stream << std::endl;
	}

	for (auto i = 0; i < (int)header.size(); i++)
	{
		stream << "#    ** " << tools::style(header[i].first, tools::Style::BOLD);
		for (auto j = 0; j < max_n_chars - (int)header[i].first.length(); j++) stream << " ";
		stream << " = " << header[i].second << std::endl;
	}
}

void aff3ct::factory::Header::compute_max_n_chars(const header_list& header, int& max_n_chars)
{
	for (unsigned i = 0; i < header.size(); i++)
		max_n_chars = std::max(max_n_chars, (int)header[i].first.length());
}

Factory::parameters
::parameters(const std::string name, const std::string short_name, const std::string prefix)
: name(name), short_name(short_name), prefix(prefix)
{
}

Factory::parameters
::~parameters()
{
}

std::string Factory::parameters
::get_name() const
{
	return this->name;
}
std::string Factory::parameters
::get_short_name() const
{
	return this->short_name;
}

std::string Factory::parameters
::get_prefix() const
{
	return this->prefix;
}
