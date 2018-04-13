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

std::ostream& rang::operator<<(std::ostream& os, const format f)
{
	switch (f)
	{
		case rang::format::error:   return format_error  (os); break;
		case rang::format::warning: return format_warning(os); break;
		case rang::format::info:    return format_info   (os); break;
		case rang::format::reset:   return format_reset  (os); break;
		default:                    return                os ; break;
	}
}

void rang::format_on_each_line(std::ostream& os, const std::string& str, const rang::format f)
{
	size_t pos = 0, old_pos = 0;
	while ((pos = str.find('\n', old_pos)) != str.npos)
	{
		os << f << str.substr(old_pos, pos-old_pos) << format::reset << std::endl;
		old_pos = pos+1;
	}

	if (!(str.substr(old_pos, pos-old_pos).length() == 1 && str.substr(old_pos, pos-old_pos)[0] == '\n'))
		os << f << str.substr(old_pos, pos-old_pos) << format::reset;
}