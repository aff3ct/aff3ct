#include <cassert>
#include <ios>


#include "Reporter.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

// get extra spaces if text is too long for the given group width
unsigned Reporter
::extra_spaces(const title_t& text, const unsigned group_width)
{
	const unsigned longest_text = (unsigned)std::max(text.first.size(), text.second.size());
	return (longest_text > group_width) ? longest_text - group_width : 0;
}

unsigned Reporter
::extra_spaces(const group_t& group)
{
	return extra_spaces(group.first, get_group_width(group));
}

unsigned Reporter
::get_group_width(const group_t& group)
{
	return (unsigned)(group.second.size() * (Reporter_stream::Reporter_stream::column_width + 1) -1); // add a col separator between each except for the last
}

std::string Reporter
::get_time_format(float secondes)
{
	auto ss = (int)secondes % 60;
	auto mm = ((int)(secondes / 60.f) % 60);
	auto hh = (int)(secondes / 3600.f);

	// TODO: this is not a C++ style code
	char time_format[256];
#ifdef _MSC_VER
	sprintf_s(time_format, 32, "%2.2dh%2.2d'%2.2d", hh, mm, ss);
#else
	sprintf(time_format, "%2.2dh%2.2d'%2.2d", hh, mm, ss);
#endif
	std::string time_format2(time_format);

	return time_format2;
}


void Reporter
::init()
{

}
