#ifndef REPORTER_THROUGHPUT_HXX_
#define REPORTER_THROUGHPUT_HXX_

#include <ios>

#include "Tools/Exception/exception.hpp"
#include "Reporter_throughput.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T>
Reporter_throughput<T>
::Reporter_throughput(std::function<T(void)> progress_function, const unsigned long long limit)
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

template <typename T>
Reporter::report_t Reporter_throughput<T>
::report(bool final)
{
	assert(this->cols_groups.size() == 1);

	report_t report(this->cols_groups.size());

	auto& thgput_report = report[0];


	auto progress = progress_function();

	auto simu_time = (double)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t_report).count(); // usec
	auto simu_cthr = ((double)progress) / simu_time; // = Mbps


	auto displayed_time = (progress == 0) ? 0.f : simu_time * 1e-6f; // sec

	if (!final)
		displayed_time *= (double)limit / (double)progress - 1.;
	else
		t_report = std::chrono::steady_clock::now();

	auto str_time = get_time_format(displayed_time) + " ";
	int  n_spaces = (int)Reporter_stream::column_width - (int)str_time.size();
	str_time = std::string((n_spaces >= 0) ? n_spaces : 0, ' ') + get_time_format(displayed_time);


	std::stringstream str_cthr;
	str_cthr << std::setprecision(3) << std::fixed << std::setw(Reporter_stream::column_width-1) << simu_cthr << " ";

	thgput_report.push_back(str_cthr.str());
	thgput_report.push_back(str_time);

	return report;
}

template <typename T>
void Reporter_throughput<T>
::init()
{
	t_report = std::chrono::steady_clock::now();
}

}
}

#endif /* REPORTER_THROUGHPUT_HXX_ */