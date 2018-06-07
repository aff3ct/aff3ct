#include <ios>

#include "Tools/Exception/exception.hpp"
#include "Reporter_MI.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename R>
Reporter_MI<B,R>
::Reporter_MI(const module::Monitor_MI<B,R> &monitor)
: Reporter(),
  monitor(monitor)
{
	auto& MI_title = MI_group.first;
	auto& MI_cols  = MI_group.second;

	MI_title = {"Mutual Information (MI)", ""};
	MI_cols.push_back(std::make_pair("TRIALS", ""));
	MI_cols.push_back(std::make_pair("MI",     ""));
	MI_cols.push_back(std::make_pair("MIN",    ""));
	MI_cols.push_back(std::make_pair("MAX",    ""));

	this->cols_groups.push_back(MI_group);
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

	stream << str_MI.str()     << report_style << Reporter_stream::spaced_scol_separator << rang::style::reset;
	stream << str_MI_min.str() << report_style << Reporter_stream::spaced_scol_separator << rang::style::reset;
	stream << str_MI_max.str() << report_style << Reporter_stream::spaced_dcol_separator << rang::style::reset;

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
