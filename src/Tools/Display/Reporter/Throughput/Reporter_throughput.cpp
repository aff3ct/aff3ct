#include <ios>

#include "Tools/Exception/exception.hpp"
#include "Reporter_throughput.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

Reporter_throughput
::Reporter_throughput(PF_t progress_function, const unsigned long long limit)
: Reporter(),
  progress_function(progress_function),
  limit(limit),
  t_report(std::chrono::steady_clock::now())
{
	auto& throughput_title = throughput_group.first;
	auto& throughput_cols  = throughput_group.second;

	throughput_title = std::make_pair("Global throughput", "and elapsed time");
	throughput_cols.clear();
	throughput_cols.push_back(std::make_pair("SIM_THR", "(Mb/s)"));
	throughput_cols.push_back(std::make_pair("ET/RT", "(hhmmss)"));

	this->cols_groups.push_back(throughput_group);
}

void Reporter_throughput
::report(std::ostream &stream, bool final)
{
	std::ios::fmtflags f(stream.flags());

	const auto report_style = Reporter_stream::report_style;

	auto progress = progress_function();

	auto simu_time = (double)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t_report).count(); // usec
	auto simu_cthr = ((double)progress) / simu_time; // = Mbps


	auto displayed_time = (progress == 0) ? 0.f : simu_time * 1e-6f; // sec

	if (!final)
		displayed_time *= (double)limit / (double)progress - 1.;
	else
		t_report = std::chrono::steady_clock::now();


	auto str_time = get_time_format(displayed_time);

	stream << std::string(extra_spaces(throughput_group), ' ');


	std::stringstream str_cthr;
	str_cthr << std::setprecision(3) << std::fixed << std::setw(Reporter_stream::column_width-1) << simu_cthr;

	stream << str_cthr.str() << report_style << Reporter_stream::spaced_scol_separator << rang::style::reset;
	stream << str_time       << report_style << Reporter_stream::spaced_dcol_separator << rang::style::reset;

	stream.flags(f);
}


void Reporter_throughput
::init()
{
	t_report = std::chrono::steady_clock::now();
}

