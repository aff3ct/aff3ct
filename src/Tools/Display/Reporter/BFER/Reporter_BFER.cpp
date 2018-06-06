#include <iostream>
#include <iomanip>
#include <sstream>
#include <ios>

#include "Tools/Display/rang_format/rang_format.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Noise/noise_utils.h"

#include "Reporter_BFER.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B>
Reporter_BFER<B>
::Reporter_BFER(const module::Monitor_BFER<B> &monitor, const bool display_throughput)
: Reporter(),
  monitor(monitor),
  display_throughput(display_throughput),
  t_report(std::chrono::steady_clock::now())
{
	auto& BFER_title = BFER_group.first;
	auto& BFER_cols  = BFER_group.second;

	BFER_title = {"Bit Error Rate (BER) and Frame Error Rate (FER)", ""};
	BFER_cols.push_back(std::make_pair("BE", ""));
	BFER_cols.push_back(std::make_pair("FE", ""));
	BFER_cols.push_back(std::make_pair("BER", ""));
	BFER_cols.push_back(std::make_pair("FER", ""));

	this->cols_groups.push_back(BFER_group);

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

template <typename B>
void Reporter_BFER<B>
::report(std::ostream &stream, bool final)
{
	std::ios::fmtflags f(stream.flags());

	const auto report_style = Reporter_stream::report_style;

	stream << report_style << Reporter_stream::spaced_scol_separator << std::string(extra_spaces(BFER_group), ' ') << rang::style::reset;


	stream << Reporter_stream::format(monitor.get_n_analyzed_fra()) << report_style << Reporter_stream::spaced_scol_separator << rang::style::reset;
	stream << Reporter_stream::format(monitor.get_n_be()          ) << report_style << Reporter_stream::spaced_scol_separator << rang::style::reset;
	stream << Reporter_stream::format(monitor.get_n_fe()          ) << report_style << Reporter_stream::spaced_scol_separator << rang::style::reset;


	std::stringstream str_ber, str_fer;
	str_ber << std::setprecision(2) << std::scientific << std::setw(Reporter_stream::column_width-1) << monitor.get_ber();
	str_fer << std::setprecision(2) << std::scientific << std::setw(Reporter_stream::column_width-1) << monitor.get_fer();

	stream << str_ber.str() << report_style << Reporter_stream::spaced_scol_separator << rang::style::reset;
	stream << str_fer.str() << report_style << Reporter_stream::spaced_dcol_separator << rang::style::reset;


	if (display_throughput)
	{
		auto simu_time = (float)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t_report).count(); // usec
		auto simu_cthr = ((float)monitor.get_K() * (float)monitor.get_n_analyzed_fra()) / simu_time; // = Mbps

		auto displayed_time = (monitor.get_n_analyzed_fra() == 0) ? 0.f : simu_time * 1e-6f;

		if (!final)
			displayed_time *= (float)monitor.get_n_fe() / (float)monitor.get_n_analyzed_fra() - 1.f;

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
template class aff3ct::tools::Reporter_BFER<B_8 >;
template class aff3ct::tools::Reporter_BFER<B_16>;
template class aff3ct::tools::Reporter_BFER<B_32>;
template class aff3ct::tools::Reporter_BFER<B_64>;
#else
template class aff3ct::tools::Reporter_BFER<B>;
#endif
// ==================================================================================== explicit template instantiation
