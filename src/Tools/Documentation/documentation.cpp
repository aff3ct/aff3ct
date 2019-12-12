#include <algorithm>
#include <fstream>
#include <sstream>
#include <regex>
#include <map>

#ifndef AFF3CT_EXT_STRINGS
#include "Tools/Documentation/strings.cpp"
#endif
#include "Tools/version.h"
#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/system_functions.h"
#include "Tools/Documentation/documentation.h"

using namespace aff3ct;
using namespace aff3ct::tools;

std::map<std::string,std::string> Documentation;

cli::Argument_tag extract_tags(const std::string &key, const std::string &prefix)
{
	auto key_cpy = key;
	const std::string pcode = "p+";

	while (key_cpy.find(pcode) != std::string::npos)
		key_cpy.replace(key_cpy.find(pcode), pcode.length(), prefix + "-");
	auto split_key = split(key_cpy, ':');

	if (split_key.size() == 0)
	{
		std::stringstream message;
		message << "'split_key.size()' has to be higher than 0.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	auto tags = split(split_key[split_key.size()-1], ',');

	if (tags.size() == 0)
	{
		std::stringstream message;
		message << "'tags.size()' has to be higher than 0.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
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
#if !(!defined(__clang__) && !defined(__llvm__) && defined(__GNUC__) && defined(__cplusplus) && __GNUC__ < 5)
	std::vector<std::regex> regexes;

	new_value = std::regex_replace(new_value, std::regex("``([^``]*)``"), "'$1'");
	new_value = std::regex_replace(new_value, std::regex("`([^`]*)`_"), "$1");
	new_value = std::regex_replace(new_value, std::regex(":math:`([^ ]*)`"), ";;$1;;");

	regexes.push_back(std::regex("\\|([^ ]*)\\|"));
	regexes.push_back(std::regex("`([^`]*)`"));
	regexes.push_back(std::regex("\\*\\*([^\\*\\*]*)\\*\\*"));
	regexes.push_back(std::regex("\\*([^\\*]*)\\*"));
	for (auto &re : regexes)
		new_value = std::regex_replace(new_value, re, "$1");

	new_value = std::regex_replace(new_value, std::regex(";;([^;;]*);;"),"`$1`");
#endif

	return new_value;
}

void parse_documentation(const std::vector<std::string> &lines)
{
	std::string line, key, value;
	for (auto line : lines)
	{
		if (line.find(".. |") == 0)
		{
			if (key.length() && value.length())
				Documentation[key] = sanitize(value);

			value.clear();

			auto split_line = split(line, '|');

			if (split_line.size() < 2)
			{
				std::stringstream message;
				message << "'split_line.size()' has to be equal or higher than 2 ('split_line.size()' = "
				        << split_line.size() << ").";
				throw runtime_error(__FILE__, __LINE__, __func__, message.str());
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

std::string extract_documentation(const std::string &key)
{
	if (Documentation.empty())
	{
		std::vector<std::string> lines;

#ifdef AFF3CT_EXT_STRINGS
		std::string binary_path = get_binary_path();
		if (!binary_path.empty())
		{
			std::string basedir, filename;
			split_path(binary_path, basedir, filename);

			std::string aff3ct_version = version();
			if (!aff3ct_version.empty() && aff3ct_version[0] == 'v')
				aff3ct_version.erase(0, 1); // rm the 'v'

			std::vector<std::string> paths = {
				"../../doc/",
				"../../../doc/",
				"../share/aff3ct-" + aff3ct_version + "/doc/strings/",
				"../../share/aff3ct-" + aff3ct_version + "/doc/strings/",
				"/usr/share/aff3ct-" + aff3ct_version + "/doc/strings/",
				"/usr/share/aff3ct-" + aff3ct_version + "/doc/strings/",
				"/usr/local/share/aff3ct-" + aff3ct_version + "/doc/strings/",
				"../share/aff3ct/doc/strings/",
				"../../share/aff3ct/doc/strings/",
				"/usr/share/aff3ct/doc/strings/",
				"/usr/local/share/aff3ct/doc/strings/",
			};

			std::ifstream rst_file;
			for (auto &path : paths)
			{
				std::string full_path = (path[0] != '/') ? basedir + "/" : "";
				full_path += path + "strings.rst";
#if defined(_WIN32) || defined(_WIN64)
				if (path[0] == '/')
					continue;
				std::replace(full_path.begin(), full_path.end(), '/', '\\');
#endif
				rst_file.open(full_path);
				if (rst_file.is_open())
					break;
			}

			std::string line;
			if (rst_file.is_open())
				while (std::getline(rst_file, line) && !rst_file.eof() && !rst_file.fail() && !rst_file.bad())
					lines.push_back(line);
		}
#else
		lines = split(Strings, '\n');
#endif

		parse_documentation(lines);
	}

	if (Documentation.find(key) == Documentation.end())
		return "This parameter is not documented.";
	else
		return Documentation[key];
}

void aff3ct::tools::add_arg(cli::Argument_map_info &args, const std::string &prefix, const std::string &key,
                            cli::Argument_type *arg_t, const cli::arg_rank rank)
{
	const cli::Argument_tag tags = extract_tags(key, prefix);
	const std::string doc = extract_documentation(key);
	args.add(tags, arg_t, doc, rank, key);
}