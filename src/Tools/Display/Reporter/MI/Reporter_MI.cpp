#include <utility>
#include <iomanip>
#include <sstream>
#include <ios>

#include "Tools/Display/Reporter/MI/Reporter_MI.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename R>
Reporter_MI<B,R>
::Reporter_MI(const M &monitor)
: Rm(monitor)
{
	create_groups();
}

template <typename B, typename R>
void Reporter_MI<B,R>
::create_groups()
{
	auto& MI_title = this->monitor_group.first;
	auto& MI_cols  = this->monitor_group.second;

	MI_title = {"Mutual Information (MI)", ""};
	MI_cols.push_back(std::make_pair("TRIALS", ""));
	MI_cols.push_back(std::make_pair("MI",     ""));
	MI_cols.push_back(std::make_pair("MIN",    ""));
	MI_cols.push_back(std::make_pair("MAX",    ""));

	this->cols_groups.push_back(this->monitor_group);
}

template <typename B, typename R>
typename Reporter_MI<B,R>::report_t Reporter_MI<B,R>
::report(bool final)
{
	assert(this->cols_groups.size() == 1);

	report_t the_report(this->cols_groups.size());

	auto& mi_report = the_report[0];

	std::stringstream str_trials, str_MI, str_MI_min, str_MI_max;
	auto n_trials = this->monitor.get_n_trials();

	if (n_trials > (unsigned long long)(1e8 -1))
		str_trials << std::setprecision(2) << std::scientific << (double)n_trials;
	else
		str_trials << std::setprecision(0) << std::fixed      << n_trials;

	str_MI     << std::setprecision(4) << this->monitor.get_MI    ();
	str_MI_min << std::setprecision(4) << this->monitor.get_MI_min();
	str_MI_max << std::setprecision(4) << this->monitor.get_MI_max();

	mi_report.push_back(str_trials.str());
	mi_report.push_back(str_MI    .str());
	mi_report.push_back(str_MI_min.str());
	mi_report.push_back(str_MI_max.str());

	return the_report;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Reporter_MI<B_8, R_8>;
template class aff3ct::tools::Reporter_MI<B_16,R_16>;
template class aff3ct::tools::Reporter_MI<B_32,R_32>;
template class aff3ct::tools::Reporter_MI<B_64,R_64>;
#else
template class aff3ct::tools::Reporter_MI<B,R>;
#endif
// ==================================================================================== explicit template instantiation
