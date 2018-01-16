#include <algorithm>
#include <iostream>
#include <utility>
#include <sstream>
#include <vector>
#include <map>

#include "Tools/system_functions.h"
#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Arguments/Argument_handler.hpp"

#include "Factory.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Factory_name       = "Factory";
const std::string aff3ct::factory::Factory_short_name = "Factory";
const std::string aff3ct::factory::Factory_prefix     = "fac";

Factory::parameters
::parameters(const std::string &name, const std::string &short_name, const std::string &prefix)
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

std::vector<std::string> Factory::parameters
::get_names() const
{
	std::vector<std::string> n;
	n.push_back(this->name);
	return n;
}
std::vector<std::string> Factory::parameters
::get_short_names() const
{
	std::vector<std::string> sn;
	sn.push_back(this->short_name);
	return sn;
}

std::vector<std::string> Factory::parameters
::get_prefixes() const
{
	std::vector<std::string> p;
	p.push_back(this->prefix);
	return p;
}

std::pair<tools::Argument_map_info, tools::Argument_map_info> Factory
::get_description(const std::vector<Factory::parameters*> &params)
{
	std::pair<tools::Argument_map_info, tools::Argument_map_info> args;
	for (auto *p : params)
		p->get_description(args.first, args.second);

	return args;
}

void Factory
::store(std::vector<Factory::parameters*> &params, const tools::Argument_map_value &vals)
{
	for (auto *p : params)
		p->store(vals);
}

tools::Argument_map_group Factory
::create_groups(const std::vector<Factory::parameters*> &params)
{
	// create groups of arguments
	tools::Argument_map_group grps;
	for (auto *p : params)
	{
		auto prefixes    = p->get_prefixes   ();
		auto short_names = p->get_short_names();

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

bool Factory
::parse_command(int argc, char** argv, std::vector<Factory::parameters*> &params, std::ostream& err_stream)
{
	// build the required and optional arguments for the cmd line
	auto args = aff3ct::factory::Factory::get_description(params);

	// parse the argument from the command line
	tools::Argument_handler ahandler(argc, (const char**)argv);
	std::vector<std::string> warnings, errors;
	auto read_args = ahandler.parse_arguments(args.first, args.second, warnings, errors);

	try
	{
		// write the parameters values in "params" from "read_args"
		Factory::store(params, read_args);
	}
	catch(const aff3ct::tools::exception& e)
	{
		errors.push_back(e.what_no_bt());
	}
	catch(const std::exception& e)
	{
		errors.push_back(tools::addr2line(e.what()));
	}

	// if there is blocking errors
	if (errors.size())
	{
		// create groups of arguments
		auto grps = aff3ct::factory::Factory::create_groups(params);

		// display the command usage and the help (the parameters are ordered by group)
		ahandler.print_help(args.first, args.second, grps);

		// print the warnings
		for (unsigned e = 0; e < warnings.size(); e++)
			err_stream << aff3ct::tools::format_warning(warnings[e]) << std::endl;

		// print the errors
		for (size_t e = 0; e < errors.size(); e++)
			err_stream << aff3ct::tools::format_error(errors[e]) << std::endl;

		// exit the program here
		return 1;
	}

	// print the warnings
	for (size_t w = 0; w < warnings.size(); w++)
		err_stream << aff3ct::tools::format_warning(warnings[w]) << std::endl;

	return 0;
}

void aff3ct::factory::Header::print_parameters(std::string grp_key, std::string grp_name, header_list header,
                                               int max_n_chars, std::ostream& stream)
{
	auto key = tools::split(grp_key, '-');

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

	std::vector<std::string> dup;
	for (auto i = 0; i < (int)header.size(); i++)
	{
		if (std::find(dup.begin(), dup.end(), header[i].first + header[i].second) == dup.end())
		{
			stream << "#    ** " << tools::style(header[i].first, tools::Style::BOLD);
			for (auto j = 0; j < max_n_chars - (int)header[i].first.length(); j++) stream << " ";
			stream << " = " << header[i].second << std::endl;

			dup.push_back(header[i].first + header[i].second);
		}
	}
}

void aff3ct::factory::Header::print_parameters(const std::vector<Factory::parameters*> &params, const bool full,
                                               std::ostream& stream)
{
	int max_n_chars = 0;
	for (auto *p : params)
	{
		std::map<std::string,aff3ct::factory::header_list> headers;
		p->get_headers(headers, full);

		for (auto &h : headers)
			if (full || (!full && h.second.size() && (h.second[0].first != "Type" || h.second[0].second != "NO")))
				aff3ct::factory::Header::compute_max_n_chars(h.second, max_n_chars);
	}

	std::vector<aff3ct::factory::header_list> dup_h;
	std::vector<std::string                 > dup_n;
	for (auto *p : params)
	{
		std::map<std::string,aff3ct::factory::header_list> headers;
		p->get_headers(headers, full);

		auto prefixes = p->get_prefixes();
		auto short_names = p->get_short_names();

		if (prefixes.size() != short_names.size())
		{
			std::stringstream message;
			message << "'prefixes.size()' has to be equal to 'short_names.size()' ('prefixes.size()' = "
			        << prefixes.size() << ", 'short_names.size()' = " << short_names.size() << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		bool print_first_title = false;
		for (size_t i = 1; i < prefixes.size(); i++)
		{
			auto h = headers[prefixes[i]];
			auto key = tools::split(prefixes[i], '-');

			if (key[0] == prefixes[0] && h.size())
			{
				print_first_title = true;
				break;
			}
		}

		for (size_t i = 0; i < prefixes.size(); i++)
		{
			auto h = headers[prefixes[i]];
			auto print_head = (i == 0) ? print_first_title || h.size() : h.size();

			if (full || (!full && h.size() && (h[0].first != "Type" || h[0].second != "NO")))
			{
				auto n = short_names[i];
				if (print_head && (std::find(dup_h.begin(), dup_h.end(), h) == dup_h.end() ||
				                   std::find(dup_n.begin(), dup_n.end(), n) == dup_n.end()))
				{
					aff3ct::factory::Header::print_parameters(prefixes[i], n, h, max_n_chars);

					dup_h.push_back(h);
					dup_n.push_back(n);
				}
			}
		}
	}
}

void aff3ct::factory::Header::compute_max_n_chars(const header_list& header, int& max_n_chars)
{
	for (unsigned i = 0; i < header.size(); i++)
		max_n_chars = std::max(max_n_chars, (int)header[i].first.length());
}
