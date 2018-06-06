#include <ios>

#include "Tools/Exception/exception.hpp"
#include "Reporter_MI.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename R>
Reporter_MI<B,R>
::Reporter_MI(const module::Monitor_MI<B,R> &monitor, const bool display_throughput)
: Reporter(),
  monitor(monitor),
  display_throughput(display_throughput),
  t_report(std::chrono::steady_clock::now())
{
	auto& MI_title = MI_group.first;
	auto& MI_cols  = MI_group.second;

	MI_title = {"Mutual Information (MI)", ""};
	MI_cols.push_back(std::make_pair("FRA", ""));
	MI_cols.push_back(std::make_pair("MIN", ""));
	MI_cols.push_back(std::make_pair("MAX", ""));
	MI_cols.push_back(std::make_pair("AVG", ""));

	this->cols_groups.push_back(MI_group);

	if (display_throughput)
	{
		auto& throughput_title = throughput_group.first;
		auto& throughput_cols  = throughput_group.second;

		throughput_title = std::make_pair("Global throughput", "and elapsed time");
		throughput_cols.clear();
		throughput_cols.push_back(std::make_pair("SIM_THR", "(Mb/s)"));
		throughput_cols.push_back(std::make_pair("ET/RT", "(hhmmss)"));

		this->cols_groups.push_back(throughput_group);
	}
}

template <typename B, typename R>
void Reporter_MI<B,R>
::report(std::ostream &stream, bool final)
{
	std::ios::fmtflags f(stream.flags());

	const auto report_style = Reporter_stream::report_style;

	stream << report_style << Reporter_stream::spaced_scol_separator << std::string(extra_spaces(MI_group), ' ') << rang::style::reset;


	stream << Reporter_stream::format(monitor.get_n_trials_fra()) << report_style << Reporter_stream::spaced_scol_separator << rang::style::reset;


	std::stringstream str_MI, str_MI_min, str_MI_max;
	str_MI     << std::setprecision(3) << std::setw(Reporter_stream::column_width-1) << monitor.get_MI    ();
	str_MI_min << std::setprecision(3) << std::setw(Reporter_stream::column_width-1) << monitor.get_MI_min();
	str_MI_max << std::setprecision(3) << std::setw(Reporter_stream::column_width-1) << monitor.get_MI_max();

	stream << str_MI_min.str() << report_style << Reporter_stream::spaced_scol_separator << rang::style::reset;
	stream << str_MI.str()     << report_style << Reporter_stream::spaced_scol_separator << rang::style::reset;
	stream << str_MI_max.str() << report_style << Reporter_stream::spaced_dcol_separator << rang::style::reset;


	if (display_throughput)
	{
		auto simu_time = (float)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t_report).count(); // usec
		auto simu_cthr = ((float)monitor.get_N() * (float)monitor.get_n_trials_fra()) / simu_time; // = Mbps

		auto displayed_time = (monitor.get_n_trials_fra() == 0) ? 0.f : simu_time * 1e-6f;

		if (!final)
			displayed_time *= (float)monitor.get_n_trials_limit() / (float)monitor.get_n_trials_fra() - 1.f;

		auto str_time = get_time_format(displayed_time);

		stream << std::string(extra_spaces(throughput_group), ' ');


		std::stringstream str_cthr;
		str_cthr << std::setprecision(3) << std::fixed << std::setw(Reporter_stream::column_width-1) << simu_cthr;

		stream << str_cthr.str() << report_style << Reporter_stream::spaced_scol_separator << rang::style::reset;
		stream << str_time       << report_style << Reporter_stream::spaced_dcol_separator << rang::style::reset;
	}

	stream.flags(f);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Reporter_MI<B_8, R_8>;
template class aff3ct::tools::Reporter_MI<B_16,R_16>;
template class aff3ct::tools::Reporter_MI<B_32,R_32>;
template class aff3ct::tools::Reporter_MI<B_64,R_64>;
#else
template class aff3ct::tools::Reporter_MI<B,R>;
#endif
// ==================================================================================== explicit template instantiation
