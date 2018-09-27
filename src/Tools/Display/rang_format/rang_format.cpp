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

std::ostream& format_comment(std::ostream& os)
{
	os << rang::style::bold << "# " << rang::style::reset;
	return os;
}

std::ostream& format_positive(std::ostream& os)
{
	os << rang::style::bold << rang::fg::green << "(II) " << rang::style::reset;
	return os;
}

std::ostream& rang::operator<<(std::ostream& os, tag f)
{
	switch (f)
	{
		case rang::tag::error:    return format_error   (os);
		case rang::tag::warning:  return format_warning (os);
		case rang::tag::info:     return format_info    (os);
		case rang::tag::comment:  return format_comment (os);
		case rang::tag::positive: return format_positive(os);
	}

	return os;
}

void rang::format_on_each_line(std::ostream& os, const std::string& str, rang::tag f)
{
	size_t pos = 0, old_pos = 0;
	while ((pos = str.find('\n', old_pos)) != std::string::npos)
	{
		os << f << str.substr(old_pos, pos-old_pos) << std::endl;
		old_pos = pos+1;
	}
	if (!str.substr(old_pos, pos-old_pos).empty())
		os << f << str.substr(old_pos, pos-old_pos);
}