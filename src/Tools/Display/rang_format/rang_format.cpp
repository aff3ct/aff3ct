#include <numeric>
#include <sstream>

#include "rang_format.h"

std::ostream& format_error(std::ostream& os)
{
	os << rang::style::bold << rang::fg::red << "(EE) " << rang::style::reset;
	return os;
}

std::ostream& format_warning(std::ostream& os)
{
	os << rang::style::bold << rang::fg::yellow << "(WW) " << rang::style::reset;
	return os;
}

std::ostream& format_info(std::ostream& os)
{
	os << rang::style::bold << rang::fg::blue << "(II) " << rang::style::reset;
	return os;
}

std::ostream& format_reset(std::ostream& os)
{
	os << rang::style::reset;
	return os;
}

std::ostream& format_comment(std::ostream& os)
{
	os << rang::style::bold << "# " << rang::style::reset;
	return os;
}

std::ostream& rang::operator<<(std::ostream& os, format f)
{
	switch (f)
	{
		case rang::format::error:   return format_error  (os);
		case rang::format::warning: return format_warning(os);
		case rang::format::info:    return format_info   (os);
		case rang::format::reset:   return format_reset  (os);
		case rang::format::comment: return format_comment(os);
	}
}

void rang::format_on_each_line(std::ostream& os, const std::string& str, rang::format f)
{
	size_t pos = 0, old_pos = 0;
	while ((pos = str.find('\n', old_pos)) != std::string::npos)
	{
		os << f << str.substr(old_pos, pos-old_pos) << format::reset << std::endl;
		old_pos = pos+1;
	}
	if (!str.substr(old_pos, pos-old_pos).empty())
		os << f << str.substr(old_pos, pos-old_pos) << format::reset;
}