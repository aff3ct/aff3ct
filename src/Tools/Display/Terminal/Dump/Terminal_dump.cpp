#include <algorithm>
#include <cassert>
#include <iomanip>
#include <cmath>
#include <ios>

#include "Tools/Display/rang_format/rang_format.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Display/Terminal/Dump/Terminal_dump.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

Terminal_dump
::Terminal_dump(const std::vector<tools::Reporter*>& reporters)
: Terminal_std(reporters)
{
}

Terminal_dump
::Terminal_dump(const std::vector<std::unique_ptr<tools::Reporter>>& reporters)
: Terminal_std(reporters)
{
}

void Terminal_dump
::report(std::ostream &stream, bool final)
{
	std::ios::fmtflags f(stream.flags());

	std::vector<Reporter::report_t> report_list;
	for (unsigned r = 0; r < this->reporters.size(); r++)
		report_list.push_back(this->reporters[r]->report(final));

	bool is_not_over = true;
	while (is_not_over)
	{
		stream << data_tag;

		is_not_over = false;
		for (unsigned r = 0; r < this->reporters.size(); r++)
			if (this->reporters[r] != nullptr)
			{
				auto& report = report_list[r];
				auto& groups = this->reporters[r]->get_groups();

				assert(report.size() == groups.size());

				for (unsigned g = 0; g < groups.size(); g++)
				{
					if (report[g].size())
						assert(report[g].size() % groups[g].second.size() == 0);

					stream << report_style << std::string(extra_spaces(groups[g]), ' ') << rang::style::reset;

					for (unsigned c = 0; c < groups[g].second.size(); c++)
					{
						auto text = report[g].size() ? report[g][c] : "-";

						if (text.size() < (size_t)column_width)
						{
							text += " ";
							text.insert(0, column_width - text.size(), ' ');
						}

						stream << text;

						if (c != (groups[g].second.size() - 1))
							stream << report_style << col_separator << rang::style::reset;
					}
					if (report[g].size())
						report[g].erase(report[g].begin(), report[g].begin() + groups[g].second.size());
					is_not_over = is_not_over ? is_not_over : report[g].size() > 0;

					if (g != (groups.size() - 1))
						stream << report_style << group_separator << rang::style::reset;
				}

				if (r != (this->reporters.size() - 1))
					stream << report_style << group_separator << rang::style::reset;
			}
			else
				throw tools::runtime_error(__FILE__, __LINE__, __func__, "'this->reporters' contains null pointer.");

		stream << std::endl;
	}

	stream.flags(f);
	stream.flush();
}
