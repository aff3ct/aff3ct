#include <sstream>
#include <rang.hpp>

#include "bash_tools.h"

std::string aff3ct::tools::format_error(const std::string &str)
{
	std::stringstream stream;
	stream << rang::style::bold << rang::fg::red << "(EE) " << rang::style::reset << str;
	return stream.str();
}

std::string aff3ct::tools::format_warning(const std::string &str)
{
	std::stringstream stream;
	stream << rang::style::bold << rang::fg::yellow << "(WW) " << rang::style::reset << str;
	return stream.str();
}

std::string aff3ct::tools::format_info(const std::string &str)
{
	std::stringstream stream;
	stream << rang::style::bold << rang::fg::blue << "(II) " << rang::style::reset << str;
	return stream.str();
}

std::string aff3ct::tools::apply_on_each_line(const std::string& str, const format_function fptr)
{
	std::string formated;

	size_t pos = 0, old_pos = 0;
	while((pos = str.find('\n', old_pos)) != str.npos)
	{
		formated += fptr(str.substr(old_pos, pos-old_pos)) + "\n";

		old_pos = pos+1;
	}

	formated += fptr(str.substr(old_pos, pos-old_pos));

	return formated;
}