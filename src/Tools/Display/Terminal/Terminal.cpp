#include <cassert>
#include "Terminal.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;


const char        aff3ct::tools::Terminal::line_separator        = '-';
const std::string aff3ct::tools::Terminal::col_separator         = "|";
const std::string aff3ct::tools::Terminal::group_separator       = "||" ;
const std::string aff3ct::tools::Terminal::spaced_scol_separator = " |" ;
const std::string aff3ct::tools::Terminal::spaced_dcol_separator = " ||";
const std::string aff3ct::tools::Terminal::data_tag              = "  ";
const rang::style aff3ct::tools::Terminal::legend_style          = rang::style::bold;
const rang::style aff3ct::tools::Terminal::report_style          = rang::style::bold;

#ifdef _WIN32
const int aff3ct::tools::Terminal::column_width = 11;
#else
const int aff3ct::tools::Terminal::column_width = 10;
#endif

Terminal
::Terminal()
: stop_terminal(false)
{
}

Terminal
::~Terminal()
{
	stop_temp_report(); // try to join the thread if this is not been done by the user
}

// get extra spaces if text is too long for the given group width
unsigned Terminal
::extra_spaces(const title_t& text, const unsigned group_width)
{
	const unsigned longest_text = std::max(text.first.size(), text.second.size());
	return (longest_text > group_width) ? longest_text - group_width : 0;
}

unsigned Terminal
::extra_spaces(const group_t& group)
{
	return extra_spaces(group.first, get_group_width(group));
}

unsigned Terminal
::get_group_width(const group_t& group)
{
	return group.second.size() * (column_width + 1) -1; // add a col separator between each except for the last
}

void Terminal
::legend(std::ostream &stream)
{
	std::ios::fmtflags f(stream.flags());

	// stream << "# " << "----------------------------------------------||---------------------------------" << std::endl; // line 1
	// stream << "# " << "          cols_groups[0].first.first          ||   cols_groups[1].first.first    " << std::endl; // line 2
	// stream << "# " << "          cols_groups[0].first.second         ||   cols_groups[1].first.second   " << std::endl; // line 3
	// stream << "# " << "----------------------------------------------||---------------------------------" << std::endl; // line 4
	// stream << "# " << "----------|-----------|-----------|-----------||----------|----------|-----------" << std::endl; // line 5
	// stream << "# " << "   (1.1)  |    (2.1)  |    (3.1)  |    (4.1)  ||   (5.1)  |   (6.1)  |   (7.1)   " << std::endl; // line 6
	// stream << "# " << "   (1.2)  |    (2.2)  |    (3.2)  |    (4.2)  ||   (5.2)  |   (6.2)  |   (7.2)   " << std::endl; // line 7
	// stream << "# " << "----------|-----------|-----------|-----------||----------|----------|-----------" << std::endl; // line 8
	// note (1.1) is "cols_groups[0].second[0].first"
	// note (1.2) is "cols_groups[0].second[0].second"
	// note (2.1) is "cols_groups[0].second[1].first"
	// note (2.2) is "cols_groups[0].second[1].second"
	// note (3.1) is "cols_groups[0].second[2].first"
	// note (3.2) is "cols_groups[0].second[2].second"
	// note (4.1) is "cols_groups[0].second[3].first"
	// note (4.2) is "cols_groups[0].second[3].second"
	// note (5.1) is "cols_groups[1].second[0].first"
	// note (5.2) is "cols_groups[1].second[0].second"
	// note (6.1) is "cols_groups[1].second[1].first"
	// note (6.2) is "cols_groups[1].second[1].second"
	// note (7.1) is "cols_groups[1].second[2].first"
	// note (7.2) is "cols_groups[1].second[2].second"

	assert(!cols_groups.empty());

	// print line 1 of the table
	stream << rang::tag::comment;
	for (unsigned i = 0; i < cols_groups.size(); i++)
	{
		assert(!cols_groups[i].second.empty());

		const unsigned group_width = get_group_width(cols_groups[i]);
		const auto n_separators = group_width + extra_spaces(cols_groups[i].first, group_width);

		stream << legend_style << std::string(n_separators, line_separator) << rang::style::reset ;

		if (i < (cols_groups.size() -1)) // print group separator except for last
			stream << legend_style << group_separator << rang::style::reset ;
	}
	stream << std::endl;

	// print line 2 and 3 of the table (group title lines)
	for (auto l = 0; l < 2; l++)
	{
		stream << rang::tag::comment;
		for (unsigned i = 0; i < cols_groups.size(); i++)
		{
			const auto& text = l == 0 ? cols_groups[i].first.first : cols_groups[i].first.second;

			const unsigned group_width = get_group_width(cols_groups[i]);
			int n_spaces = (int)group_width - (int)text.size();

			if (text.size() != std::max(cols_groups[i].first.first.size(), cols_groups[i].first.second.size()))
				n_spaces += extra_spaces(cols_groups[i].first, group_width);


			const unsigned n_spaces_left  = (n_spaces >= 0) ? (unsigned)n_spaces/2 : 0;
			const unsigned n_spaces_right = (n_spaces >= 0) ? n_spaces - n_spaces_left : 0;

			stream << legend_style << std::string(n_spaces_left,  ' ') << rang::style::reset;
			stream << legend_style << text << rang::style::reset;
			stream << legend_style << std::string(n_spaces_right, ' ') << rang::style::reset;

			if (i < (cols_groups.size() -1)) // print group separator except for last
				stream << legend_style << group_separator << rang::style::reset;
		}
		stream << std::endl;
	}

	// print line 4 of the table
	stream << rang::tag::comment;
	for (unsigned i = 0; i < cols_groups.size(); i++)
	{
		const unsigned group_width = get_group_width(cols_groups[i]);
		const auto n_separators = group_width + extra_spaces(cols_groups[i].first, group_width);

		stream << legend_style << std::string(n_separators, line_separator) << rang::style::reset;

		if (i < (cols_groups.size() -1)) // print group separator except for last
			stream << legend_style << group_separator << rang::style::reset;
	}
	stream << std::endl;

	// print line 5 of the table
	stream << rang::tag::comment;
	for (unsigned i = 0; i < cols_groups.size(); i++)
	{
		const unsigned group_width = get_group_width(cols_groups[i]);
		const auto n_extra = extra_spaces(cols_groups[i].first, group_width);

		for (unsigned j = 0; j < cols_groups[i].second.size(); j++)
		{
			auto n_separators = column_width;
			if (j == 0)
				n_separators += n_extra;

			stream << legend_style << std::string(n_separators, line_separator) << rang::style::reset;
			if (j < (cols_groups[i].second.size() -1)) // print column separator except for last
				stream << legend_style << col_separator << rang::style::reset;
		}

		if (i < (cols_groups.size() -1)) // print group separator except for last
			stream << legend_style << group_separator << rang::style::reset;
	}
	stream << std::endl;

	// print line 6 and 7 of the table (column title lines)
	for (auto l = 0; l < 2; l++)
	{
		stream << rang::tag::comment;
		for (unsigned i = 0; i < cols_groups.size(); i++)
		{
			const unsigned group_width = get_group_width(cols_groups[i]);
			const auto n_extra = extra_spaces(cols_groups[i].first, group_width);

			for (unsigned j = 0; j < cols_groups[i].second.size(); j++)
			{
				const auto& text = l == 0 ? cols_groups[i].second[j].first : cols_groups[i].second[j].second;
				int n_spaces = (int)column_width - (int)text.size() -1;

				if (j == 0)
					n_spaces += n_extra;

				if (n_spaces > 0)
					stream << legend_style << std::string(n_spaces,  ' ') << rang::style::reset;

				stream << legend_style << text + " " << rang::style::reset;

				if (j < (cols_groups[i].second.size() -1)) // print column separator except for last
					stream << legend_style << col_separator << rang::style::reset;
			}

			if (i < (cols_groups.size() -1)) // print group separator except for last
				stream << legend_style << group_separator << rang::style::reset;
		}
		stream << std::endl;
	}

	// print line 8 of the table
	stream << rang::tag::comment;
	for (unsigned i = 0; i < cols_groups.size(); i++)
	{
		const unsigned group_width = get_group_width(cols_groups[i]);
		const auto n_extra = extra_spaces(cols_groups[i].first, group_width);

		for (unsigned j = 0; j < cols_groups[i].second.size(); j++)
		{
			auto n_separators = column_width;
			if (j == 0)
				n_separators += n_extra;

			stream << legend_style << std::string(n_separators, line_separator) << rang::style::reset;
			if (j < (cols_groups[i].second.size() -1)) // print column separator except for last
				stream << legend_style << col_separator << rang::style::reset;
		}

		if (i < (cols_groups.size() -1)) // print group separator except for last
			stream << legend_style << group_separator << rang::style::reset;
	}
	stream << std::endl;

	stream.flags(f);
}

void Terminal
::final_report(std::ostream &stream)
{
	this->stop_temp_report();
}

void Terminal
::start_temp_report(const std::chrono::milliseconds freq)
{
	this->stop_temp_report();

	// launch a thread dedicated to the terminal display
	term_thread = std::thread(Terminal::start_thread_terminal, this, freq);
}

void Terminal
::stop_temp_report()
{
	if (term_thread.joinable())
	{
		stop_terminal = true;
		cond_terminal.notify_all();
		// wait the terminal thread to finish
		term_thread.join();
		stop_terminal = false;
	}
}

void Terminal
::start_thread_terminal(Terminal *terminal, const std::chrono::milliseconds freq)
{
	const auto sleep_time = freq - std::chrono::milliseconds(0);
	while (!terminal->stop_terminal)
	{
		std::unique_lock<std::mutex> lock(terminal->mutex_terminal);
		if (terminal->cond_terminal.wait_for(lock, sleep_time) == std::cv_status::timeout)
			terminal->temp_report(std::clog); // display statistics in the terminal
	}
}

std::string Terminal
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

	return std::string(time_format);
}