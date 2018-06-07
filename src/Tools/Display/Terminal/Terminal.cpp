#include <cassert>
#include <csignal>

#include "Tools/Exception/exception.hpp"
#include "Terminal.hpp"

bool aff3ct::tools::Terminal::interrupt       = false;
bool aff3ct::tools::Terminal::first_interrupt = true;
int  aff3ct::tools::Terminal::interrupt_cnt   = 0;
bool aff3ct::tools::Terminal::over            = false;

std::chrono::time_point<std::chrono::steady_clock> aff3ct::tools::Terminal::t_last_interrupt;


using namespace aff3ct;
using namespace aff3ct::tools;

Terminal
::Terminal(std::vector<Reporter*>& reporters)
: stop_terminal(false),
  reporters(reporters),
  t_term(std::chrono::steady_clock::now()),
  real_time_state(0)
{
	Terminal::interrupt = false;

#ifndef ENABLE_MPI
	// Install a signal handler
	std::signal(SIGINT, Terminal::signal_interrupt_handler);
#endif
}

Terminal
::~Terminal()
{
	stop_temp_report(); // try to join the thread if this is not been done by the user
}

void Terminal
::legend(std::ostream &stream) const
{
	std::vector<const Reporter::group_t*> cols_groups;

	for(auto& r : this->reporters)
		if (r != nullptr)
			for (auto& g : r->get_groups())
				cols_groups.push_back(&g);
		else
			throw tools::runtime_error(__FILE__, __LINE__, __func__, "'this->reporters' contains null pointer.");


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
		assert(!cols_groups[i]->second.empty());

		const unsigned group_width = Reporter::get_group_width(*cols_groups[i]);
		const auto n_separators = group_width + Reporter::extra_spaces(cols_groups[i]->first, group_width);

		stream << Reporter_stream::legend_style << std::string(n_separators, Reporter_stream::line_separator) << rang::style::reset ;

		if (i < (cols_groups.size() -1)) // print group separator except for last
			stream << Reporter_stream::legend_style << Reporter_stream::group_separator << rang::style::reset ;
	}
	stream << std::endl;

	// print line 2 and 3 of the table (group title lines)
	for (auto l = 0; l < 2; l++)
	{
		stream << rang::tag::comment;
		for (unsigned i = 0; i < cols_groups.size(); i++)
		{
			const auto& text = l == 0 ? cols_groups[i]->first.first : cols_groups[i]->first.second;

			const unsigned group_width = Reporter::get_group_width(*cols_groups[i]);
			int n_spaces = (int)group_width - (int)text.size();

			if (text.size() != std::max(cols_groups[i]->first.first.size(), cols_groups[i]->first.second.size()))
				n_spaces += Reporter::extra_spaces(cols_groups[i]->first, group_width);


			const unsigned n_spaces_left  = (n_spaces >= 0) ? (unsigned)n_spaces/2 : 0;
			const unsigned n_spaces_right = (n_spaces >= 0) ? n_spaces - n_spaces_left : 0;

			stream << Reporter_stream::legend_style << std::string(n_spaces_left,  ' ') << rang::style::reset;
			stream << Reporter_stream::legend_style << text << rang::style::reset;
			stream << Reporter_stream::legend_style << std::string(n_spaces_right, ' ') << rang::style::reset;

			if (i < (cols_groups.size() -1)) // print group separator except for last
				stream << Reporter_stream::legend_style << Reporter_stream::group_separator << rang::style::reset;
		}
		stream << std::endl;
	}

	// print line 4 of the table
	stream << rang::tag::comment;
	for (unsigned i = 0; i < cols_groups.size(); i++)
	{
		const unsigned group_width = Reporter::get_group_width(*cols_groups[i]);
		const auto n_separators = group_width + Reporter::extra_spaces(cols_groups[i]->first, group_width);

		stream << Reporter_stream::legend_style << std::string(n_separators, Reporter_stream::line_separator) << rang::style::reset;

		if (i < (cols_groups.size() -1)) // print group separator except for last
			stream << Reporter_stream::legend_style << Reporter_stream::group_separator << rang::style::reset;
	}
	stream << std::endl;

	// print line 5 of the table
	stream << rang::tag::comment;
	for (unsigned i = 0; i < cols_groups.size(); i++)
	{
		const unsigned group_width = Reporter::get_group_width(*cols_groups[i]);
		const auto n_extra = Reporter::extra_spaces(cols_groups[i]->first, group_width);

		for (unsigned j = 0; j < cols_groups[i]->second.size(); j++)
		{
			auto n_separators = Reporter_stream::Reporter_stream::column_width;
			if (j == 0)
				n_separators += n_extra;

			stream << Reporter_stream::legend_style << std::string(n_separators, Reporter_stream::line_separator) << rang::style::reset;
			if (j < (cols_groups[i]->second.size() -1)) // print column separator except for last
				stream << Reporter_stream::legend_style << Reporter_stream::Reporter_stream::col_separator << rang::style::reset;
		}

		if (i < (cols_groups.size() -1)) // print group separator except for last
			stream << Reporter_stream::legend_style << Reporter_stream::group_separator << rang::style::reset;
	}
	stream << std::endl;

	// print line 6 and 7 of the table (column title lines)
	for (auto l = 0; l < 2; l++)
	{
		stream << rang::tag::comment;
		for (unsigned i = 0; i < cols_groups.size(); i++)
		{
			const unsigned group_width = Reporter::get_group_width(*cols_groups[i]);
			const auto n_extra = Reporter::extra_spaces(cols_groups[i]->first, group_width);

			for (unsigned j = 0; j < cols_groups[i]->second.size(); j++)
			{
				const auto& text = l == 0 ? cols_groups[i]->second[j].first : cols_groups[i]->second[j].second;
				int n_spaces = (int)Reporter_stream::column_width - (int)text.size() -1;

				if (j == 0)
					n_spaces += n_extra;

				if (n_spaces > 0)
					stream << Reporter_stream::legend_style << std::string(n_spaces,  ' ') << rang::style::reset;

				stream << Reporter_stream::legend_style << text + " " << rang::style::reset;

				if (j < (cols_groups[i]->second.size() -1)) // print column separator except for last
					stream << Reporter_stream::legend_style << Reporter_stream::col_separator << rang::style::reset;
			}

			if (i < (cols_groups.size() -1)) // print group separator except for last
				stream << Reporter_stream::legend_style << Reporter_stream::group_separator << rang::style::reset;
		}
		stream << std::endl;
	}

	// print line 8 of the table
	stream << rang::tag::comment;
	for (unsigned i = 0; i < cols_groups.size(); i++)
	{
		const unsigned group_width = Reporter::get_group_width(*cols_groups[i]);
		const auto n_extra = Reporter::extra_spaces(cols_groups[i]->first, group_width);

		for (unsigned j = 0; j < cols_groups[i]->second.size(); j++)
		{
			auto n_separators = Reporter_stream::column_width;
			if (j == 0)
				n_separators += n_extra;

			stream << Reporter_stream::legend_style << std::string(n_separators, Reporter_stream::line_separator) << rang::style::reset;
			if (j < (cols_groups[i]->second.size() -1)) // print column separator except for last
				stream << Reporter_stream::legend_style << Reporter_stream::col_separator << rang::style::reset;
		}

		if (i < (cols_groups.size() -1)) // print group separator except for last
			stream << Reporter_stream::legend_style << Reporter_stream::group_separator << rang::style::reset;
	}
	stream << std::endl;

	stream.flags(f);
}

void Terminal
::temp_report(std::ostream &stream)
{
	this->report(stream, false);
}

void Terminal
::final_report(std::ostream &stream)
{
	this->stop_temp_report();

	auto et = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - t_term).count();

	if (!Terminal::is_over() || et >= 1.f)
		this->report(stream, true);

	t_term = std::chrono::steady_clock::now();
}

void Terminal
::start_temp_report(const std::chrono::milliseconds freq)
{
	this->stop_temp_report();

	// launch a thread dedicated to the terminal display
	term_thread = std::thread(Terminal::start_thread_terminal, this, freq);

	t_term = std::chrono::steady_clock::now();
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

	real_time_state = 0;
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

void Terminal
::report(std::ostream &stream, bool final)
{
	std::ios::fmtflags f(stream.flags());

	const auto report_style = Reporter_stream::report_style;

	stream << "\r" << Reporter_stream::data_tag;


	for (unsigned r = 0; r < this->reporters.size(); r++)
		if (this->reporters[r] != nullptr)
		{
			auto  report = this->reporters[r]->report(final);
			auto& groups = this->reporters[r]->get_groups();

			assert(report.size() == groups.size());

			for (unsigned g = 0; g < groups.size(); g++)
			{
				assert(report[g].size() == groups[g].second.size());

				stream << report_style << std::string(Reporter::extra_spaces(groups[g]), ' ') << rang::style::reset;

				for (unsigned c = 0; c < report[g].size(); c++)
				{
					stream << report[g][c];

					if (c != (report[g].size() - 1))
						stream << report_style << Reporter_stream::col_separator << rang::style::reset;
				}

				if (g != (groups.size() - 1))
					stream << report_style << Reporter_stream::group_separator << rang::style::reset;
			}

			if (r != (this->reporters.size() - 1))
				stream << report_style << Reporter_stream::group_separator << rang::style::reset;
		}
		else
			throw tools::runtime_error(__FILE__, __LINE__, __func__, "'this->reporters' contains null pointer.");

	if (final)
	{
		stream << (Terminal::is_interrupt() ? " x" : "  ") << std::endl;
	}
	else
	{
		stream << rang::style::bold << rang::fg::green << (real_time_state++ < 2 ? " *" : "  ") << rang::style::reset << "\r";
		real_time_state %= (uint8_t)4;
	}

	stream.flags(f);
	stream.flush();
}

void Terminal
::reset()
{
	Terminal::interrupt = false;
	Terminal::interrupt_cnt = 0;
}


bool Terminal
::is_interrupt()
{
	return Terminal::interrupt;
}

bool Terminal
::is_over()
{
	return Terminal::over;
}

void Terminal
::stop()
{
	Terminal::interrupt = true;
	Terminal::over      = true;
}

void Terminal
::signal_interrupt_handler(int signal)
{
	Terminal::interrupt_cnt++;

	auto t_now = std::chrono::steady_clock::now();
	if (!Terminal::first_interrupt)
	{
		auto d_delta_interrupt = t_now - Terminal::t_last_interrupt;
		if (d_delta_interrupt < std::chrono::milliseconds(500))
			Terminal::stop();

		if (d_delta_interrupt < std::chrono::milliseconds(2100))
		{
			if (Terminal::interrupt_cnt >= 4)
			{
				std::cerr << "\r# Killed by user interruption!"
				             "                                                                                         "
				          << std::endl;
				std::exit(EXIT_FAILURE);
			}
		}
		else
			Terminal::interrupt_cnt = 1;
	}
	Terminal::t_last_interrupt  = t_now;

	Terminal::first_interrupt = false;
	Terminal::interrupt       = true;
}