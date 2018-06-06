#ifndef REPORTER_STREAM_HPP__
#define REPORTER_STREAM_HPP__

#include <chrono>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

#include "Tools/Display/rang_format/rang_format.h"

namespace aff3ct
{
namespace tools
{

struct Reporter_stream : public std::stringstream
{
public:
	static const char        line_separator;
	static const std::string col_separator;
	static const std::string group_separator;
	static const std::string spaced_scol_separator;
	static const std::string spaced_dcol_separator;
	static const std::string data_tag;
	static const rang::style legend_style;
	static const rang::style report_style;
	static const int         column_width;

	static std::string format(unsigned long long val)
	{
		std::stringstream os;

		if (val > (unsigned long long)(1e8 -1))
			os << std::setprecision(2) << std::scientific << std::setw(Reporter_stream::column_width-1) << (float)val;
		else
			os << std::setprecision(0) << std::fixed      << std::setw(Reporter_stream::column_width-1) << val;

		return os.str();
	}
};

}
}


#endif // REPORTER_STREAM_HPP__