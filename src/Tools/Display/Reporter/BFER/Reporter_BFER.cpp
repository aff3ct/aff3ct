#include <iomanip>
#include <sstream>
#include <utility>
#include <ios>

#include "Tools/Display/Reporter/BFER/Reporter_BFER.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B>
Reporter_BFER<B>
::Reporter_BFER(const M &monitor)
: Rm(monitor)
{
	create_groups();
}

template <typename B>
void Reporter_BFER<B>
::create_groups()
{
	auto& BFER_title = this->monitor_group.first;
	auto& BFER_cols  = this->monitor_group.second;

	BFER_title = {"Bit Error Rate (BER) and Frame Error Rate (FER)", ""};
	BFER_cols.push_back(std::make_pair("FRA", ""));
	BFER_cols.push_back(std::make_pair("BE", ""));
	BFER_cols.push_back(std::make_pair("FE", ""));
	BFER_cols.push_back(std::make_pair("BER", ""));
	BFER_cols.push_back(std::make_pair("FER", ""));

	this->cols_groups.push_back(this->monitor_group);
}

std::string format(unsigned long long val)
{
	std::stringstream os;

	if (val > (unsigned long long)(1e8 -1))
		os << std::setprecision(2) << std::scientific << (double)val;
	else
		os << std::setprecision(0) << std::fixed      << val;

	return os.str();
}

template <typename B>
typename Reporter_BFER<B>::report_t Reporter_BFER<B>
::report(bool final)
{
	assert(this->cols_groups.size() == 1);

	report_t the_report(this->cols_groups.size());

	auto& bfer_report = the_report[0];

	bfer_report.push_back(format(this->monitor.get_n_analyzed_fra()));
	bfer_report.push_back(format(this->monitor.get_n_be          ()));
	bfer_report.push_back(format(this->monitor.get_n_fe          ()));

	std::stringstream str_ber, str_fer;
	str_ber << std::setprecision(2) << std::scientific << this->monitor.get_ber();
	str_fer << std::setprecision(2) << std::scientific << this->monitor.get_fer();

	bfer_report.push_back(str_ber.str());
	bfer_report.push_back(str_fer.str());

	return the_report;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Reporter_BFER<B_8 >;
template class aff3ct::tools::Reporter_BFER<B_16>;
template class aff3ct::tools::Reporter_BFER<B_32>;
template class aff3ct::tools::Reporter_BFER<B_64>;
#else
template class aff3ct::tools::Reporter_BFER<B>;
#endif
// ==================================================================================== explicit template instantiation
