#include <algorithm>
#include <iostream>
#include <utility>
#include <sstream>
#include <fstream>
#include <vector>
#include <regex>
#include <map>

#include "Tools/Display/rang_format/rang_format.h"
#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"

#include "Factory.hpp"
#include "strings.cpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Factory_name       = "Factory";
const std::string aff3ct::factory::Factory_short_name = "Factory";
const std::string aff3ct::factory::Factory_prefix     = "fac";

std::map<std::string,std::string> Documentation;

tools::Argument_tag extract_tags(const std::string &key, const std::string &prefix)
{
	auto key_cpy = key;
	const std::string pcode = "p+";

	while (key_cpy.find(pcode) != std::string::npos)
		key_cpy.replace(key_cpy.find(pcode), pcode.length(), prefix + "-");
	auto split_key = tools::split(key_cpy, ':');

	if (split_key.size() == 0)
	{
		std::stringstream message;
		message << "'split_key.size()' has to be higher than 0.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	auto tags = tools::split(split_key[split_key.size()-1], ',');

	if (tags.size() == 0)
	{
		std::stringstream message;
		message << "'tags.size()' has to be higher than 0.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	for (size_t t = 0; t < tags.size(); t++)
		if (tags[t].size() != 0 && tags[t][tags[t].size()-1] == '-')
			tags[t].pop_back();

	return tags;
}

std::string sanitize(const std::string &value)
{
	std::string new_value = value;

// trick to compile on the GNU compiler version 4 (where 'std::regex' is unavailable)
#if !defined(__clang__) && !defined(__llvm__) && defined(__GNUC__) && defined(__cplusplus) && __GNUC__ < 5
	return new_value;
#endif

	std::regex e_pipe("\\|([^ ]*)\\|");
	new_value = std::regex_replace (new_value,e_pipe,"$1");

	std::regex e_quote("`([^ ]*)`");
	new_value = std::regex_replace (new_value,e_quote,"$1");

	std::regex e_start2("\\*\\*([^ ]*)\\*\\*");
	new_value = std::regex_replace (new_value,e_start2,"$1");

	std::regex e_start1("\\*([^ ]*)\\*");
	new_value = std::regex_replace (new_value,e_start1,"$1");

	return new_value;
}

void parse_documentation()
{
	const auto lines = tools::split(Strings, '\n');

	std::string line, key, value;
	for (auto line : lines)
	{
		if (line.find(".. |") == 0)
		{
			if (key.length() && value.length())
				Documentation[key] = sanitize(value);

			value.clear();

			auto split_line = tools::split(line, '|');

			if (split_line.size() < 2)
			{
				std::stringstream message;
				message << "'split_line.size()' has to be equal or higher than 2 ('split_line.size()' = "
				        << split_line.size() << ").";
				throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
			}

			key = split_line[1];
		}
		else if (line.find("..") != 0 && !std::all_of(line.begin(),line.end(),isspace))
		{
			if (line.find("   ") == 0) line.replace(0, 3, "");
			if (value.length()) value += " ";
			value += line;
		}
	}

	if (key.length() && value.length())
		Documentation[key] = sanitize(value);
}

std::string extract_doc(const std::string &key)
{
	if (Documentation.empty()) parse_documentation();

	if (Documentation.find(key) == Documentation.end())
		return "This parameter is not documented";
	else
		return Documentation[key];
}

void aff3ct::factory::add_arg(      tools::Argument_map_info &args,
                              const std::string              &prefix,
                              const std::string              &key,
                                    tools::Argument_type     *arg_t,
                              const tools::arg_rank           rank)
{
	const tools::Argument_tag tags = extract_tags(key, prefix);
	const std::string doc = extract_doc(key);
	args.add(tags, arg_t, doc, rank);
}

Factory::parameters
::parameters(const std::string &name, const std::string &short_name, const std::string &prefix)
: name(name), short_name(short_name), prefix(prefix)
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

tools::Argument_map_info Factory
::get_description(const std::vector<Factory::parameters*> &params)
{
	tools::Argument_map_info args;

	get_description(params, args);

	return args;
}

void Factory
::get_description(const std::vector<Factory::parameters*> &params,
                     tools::Argument_map_info &args)
{
	for (auto *p : params)
		p->get_description(args);
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

void aff3ct::factory::Header::print_parameters(std::string grp_key, std::string grp_name, header_list header,
                                               int max_n_chars, std::ostream& stream)
{
	auto key = tools::split(grp_key, '-');

	if (key.size() == 1)
	{
		stream << rang::tag::comment << "* " << rang::style::bold << rang::style::underline << grp_name << rang::style::reset << " ";
		for (auto i = 0; i < 46 - (int)grp_name.length(); i++) std::cout << "-";
		stream << std::endl;
	}
	else if (key.size() > 1)
	{
		stream << rang::tag::comment << "   " << rang::style::bold << rang::style::underline << grp_name << rang::style::reset << " ";
		for (auto i = 0; i < 45 - (int)grp_name.length(); i++) std::cout << "-";
		stream << std::endl;
	}

	std::vector<std::string> dup;
	for (auto i = 0; i < (int)header.size(); i++)
	{
		if (std::find(dup.begin(), dup.end(), header[i].first + header[i].second) == dup.end())
		{
			stream << rang::tag::comment << "   ** " << rang::style::bold << header[i].first << rang::style::reset;
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
