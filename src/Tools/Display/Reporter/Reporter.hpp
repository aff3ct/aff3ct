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

	using report_t = std::vector<std::vector<std::string>>;

protected:
	std::vector<group_t> cols_groups;

public:
	virtual ~Reporter() = default;

	const std::vector<group_t>& get_groups() const;

	/*
	 * Report a vector as long as the number of groups (cols_groups.size())
	 * with each sub vector as long as the number of columns of the matching group (cols_groups[i].second.size())
	 */
	virtual report_t report(bool final = false) = 0;

	virtual void init(); // do nothing by default
};
}
}


#endif // REPORTER_HPP__