#ifndef REPORTER_HPP__
#define REPORTER_HPP__

#include <chrono>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <utility>
#include <string>

#include "Tools/Display/rang_format/rang_format.h"
#include "Reporter_stream.hpp"

namespace aff3ct
{
namespace tools
{

class Reporter
{
public:
	using title_t        = std::pair<std::string, std::string>;
	using group_title_t  = title_t;
	using column_title_t = std::vector<title_t>;
	using group_t        = std::pair<group_title_t, column_title_t>;

	std::vector<group_t> cols_groups;

public:
	Reporter() = default;

	virtual ~Reporter() = default;

	virtual void report(std::ostream &stream = std::cout, bool final = false) = 0;

	static std::string get_time_format(float secondes);
	static unsigned extra_spaces   (const title_t& text, const unsigned group_width);
	static unsigned extra_spaces   (const group_t& group);
	static unsigned get_group_width(const group_t& group);
};
}
}


#endif // REPORTER_HPP__