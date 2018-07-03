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
::Reporter_throughput(std::function<T(void)>  progress_function, const T progress_limit,
                      std::function<T(void)> get_nbits_function, const T nbits_factor)
: Reporter(),
  progress_function(progress_function),
  get_nbits_function(get_nbits_function),
  progress_limit(progress_limit),
  nbits_factor(nbits_factor),
  t_report(std::chrono::steady_clock::now())
{
	auto& throughput_title = throughput_group.first;
	auto& throughput_cols  = throughput_group.second;

	throughput_title = std::make_pair("Global throughput", "and elapsed time");

	throughput_cols.push_back(std::make_pair("SIM_THR", "(Mb/s)"));
	throughput_cols.push_back(std::make_pair("ET/RT", "(hhmmss)"));

	this->cols_groups.push_back(throughput_group);

}

template <typename T>
template <typename B>
Reporter_throughput<T>
::Reporter_throughput(const module::Monitor_BFER<B>& m)
: Reporter_throughput(std::bind(&module::Monitor_BFER<B>::get_n_fe, &m),
	                  (T)m.get_fe_limit(),
	                  std::bind(&module::Monitor_BFER<B>::get_n_analyzed_fra, &m),
	                  (T)m.get_K())
{

}

template <typename T>
template <typename B, typename R>
Reporter_throughput<T>
::Reporter_throughput(const module::Monitor_MI<B,R>& m)
: Reporter_throughput(std::bind(&module::Monitor_MI<B,R>::get_n_trials, &m),
	                  (T)m.get_n_trials_limit(),
	                  std::bind(&module::Monitor_MI<B,R>::get_n_trials, &m),
	                  (T)m.get_N())
{

}

template <typename T>
Reporter::report_t Reporter_throughput<T>
::report(bool final)
{
	assert(this->cols_groups.size() == 1);

	report_t report(this->cols_groups.size());

	auto& thgput_report = report[0];

	T progress = 0, nbits = 0;

	if (progress_function != nullptr)
		progress = progress_function();

	if (get_nbits_function != nullptr)
		nbits = get_nbits_function() * nbits_factor;

	using namespace std::chrono;

	auto simu_time = (double)duration_cast<microseconds>(steady_clock::now() - t_report).count(); // usec
	double displayed_time = simu_time * 1e-6; // sec

	if (!final && progress != 0 && progress_limit != 0)
		displayed_time *= (double)progress_limit / (double)progress - 1.;


	auto str_time = get_time_format(displayed_time) + " ";
	int  n_spaces = (int)Reporter_stream::column_width - (int)str_time.size();
	str_time = std::string((n_spaces >= 0) ? n_spaces : 0, ' ') + str_time;


	auto simu_cthr = (double)nbits / simu_time; // = Mbps

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
	Reporter::init();

	t_report = std::chrono::steady_clock::now();
}

}
}

#endif /* REPORTER_THROUGHPUT_HXX_ */