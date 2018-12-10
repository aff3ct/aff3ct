#include <fstream>
#include <regex>
#include <map>

#if defined(__linux__) || defined(linux) || defined(__linux) || defined(__FreeBSD__)
#include <unistd.h>
#include <string.h>
#include <errno.h>
#elif defined(__APPLE__) || defined(__MACH__)
#include <mach-o/dyld.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif

#ifndef AFF3CT_EXT_STRINGS
#include "strings.cpp"
#endif

#include "Tools/version.h"
#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"

#include "documentation.h"

using namespace aff3ct;
using namespace aff3ct::tools;

std::map<std::string,std::string> Documentation;

Argument_tag extract_tags(const std::string &key, const std::string &prefix)
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
	std::regex e_pipe("\\|([^ ]*)\\|");
	new_value = std::regex_replace (new_value,e_pipe,"$1");

	std::regex e_quote("`([^ ]*)`");
	new_value = std::regex_replace (new_value,e_quote,"$1");

	std::regex e_start2("\\*\\*([^ ]*)\\*\\*");
	new_value = std::regex_replace (new_value,e_start2,"$1");

	std::regex e_start1("\\*([^ ]*)\\*");
	new_value = std::regex_replace (new_value,e_start1,"$1");
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

std::string get_binary_path()
{
	std::string binary_path;

#if defined(__linux__) || defined(linux) || defined(__linux) || defined(__FreeBSD__)
	constexpr size_t path_size = 8192;
	char path[path_size];
#ifdef __FreeBSD__
	if (readlink("/proc/curproc/file", path, path_size) == -1)
#else
	if (readlink("/proc/self/exe", path, path_size) == -1)
#endif
	{
		std::stringstream message;
		message << "'readlink' failed ('errno' = " << strerror(errno) << ").";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
	binary_path = path;
#elif defined(__APPLE__) || defined(__MACH__)
	constexpr size_t path_size = 8192;
	char path[path_size];
	uint32_t size = sizeof(path);
	if (_NSGetExecutablePath(path, &size) != 0)
	{
		std::stringstream message;
		message << "'_NSGetExecutablePath' path buffer is too small; need 'size' = " << size << "('path_size' = "
		        << path_size << ").";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
	binary_path = path;
#elif defined(_WIN32) || defined(_WIN64)
	constexpr size_t path_size = 8192;
	char path[path_size];
	DWORD copied = 0;
	copied = GetModuleFileName(NULL, path, path_size);
	if (copied >= path_size)
	{
		std::stringstream message;
		message << "'GetModuleFileName' path buffer is too small ('path_size' = " << path_size << ").";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
	binary_path = path;
#endif

	return binary_path;
}

void split_path(const std::string& path, std::string &basedir, std::string &filename)
{
  size_t found;
  found = path.find_last_of("/\\");
  basedir = path.substr(0,found);
  filename = path.substr(found+1);
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

			std::string aff3ct_version = aff3ct::version();
			if (!aff3ct_version.empty() && aff3ct_version[0] == 'v')
				aff3ct_version.erase(0, 1); // rm the 'v'

			std::vector<std::string> paths = {
				"../../doc/sphinx/",
				"../../../doc/sphinx/",
				"../../../doc/sphinx/",
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

void aff3ct::tools::add_arg(Argument_map_info &args, const std::string &prefix, const std::string &key,
                            Argument_type *arg_t, const arg_rank rank)
{
	const Argument_tag tags = extract_tags(key, prefix);
	const std::string doc = extract_documentation(key);
	args.add(tags, arg_t, doc, rank);
}