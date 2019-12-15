#include <utility>
#include <iomanip>
#include <sstream>
#include <ios>

#include "Tools/Display/Reporter/EXIT/Reporter_EXIT.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename R>
Reporter_EXIT<B,R>
::Reporter_EXIT(const M &monitor, const Noise<>& noise_a)
: Rm(monitor),
  noise_a(noise_a)
{
	create_groups();
}

template <typename B, typename R>
void Reporter_EXIT<B,R>
::create_groups()
{
	auto& EXIT_title = this->monitor_group.first;
	auto& EXIT_cols  = this->monitor_group.second;

	EXIT_title = {"EXIT chart depending on", "the channel A noise"};
	EXIT_cols.push_back(std::make_pair("SIG_A",      "(dB)"));
	EXIT_cols.push_back(std::make_pair("FRA",            ""));
	EXIT_cols.push_back(std::make_pair("A_PRIORI",  "(I_A)"));
	EXIT_cols.push_back(std::make_pair("EXTRINSIC", "(I_E)"));

	this->cols_groups.push_back(this->monitor_group);
}

template <typename B, typename R>
typename Reporter_EXIT<B,R>::report_t Reporter_EXIT<B,R>
::report(bool final)
{
	assert(this->cols_groups.size() == 1);

	report_t the_report(this->cols_groups.size());

	auto& EXIT_report = the_report[0];

	const auto fra = this->monitor.get_n_trials();
	const auto I_A = this->monitor.get_I_A();
	const auto I_E = this->monitor.get_I_E();

	std::stringstream str_sig_a, str_fra, str_I_A, str_I_E;

	str_sig_a << std::setprecision(2) << std::fixed;
	try
	{
		str_sig_a << noise_a.get_value();
	}
	catch(tools::runtime_error&)
	{
		str_sig_a << (R)0.;
	}

	str_fra << std::setprecision(2) << std::fixed << fra;
	str_I_A << std::setprecision(6) << std::fixed << I_A;
	str_I_E << std::setprecision(6) << std::fixed << I_E;

	EXIT_report.push_back(str_sig_a.str());
	EXIT_report.push_back(str_fra  .str());
	EXIT_report.push_back(str_I_A  .str());
	EXIT_report.push_back(str_I_E  .str());

	return the_report;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Reporter_EXIT<B_32, R_32>;
template class aff3ct::tools::Reporter_EXIT<B_64, R_64>;
#else
template class aff3ct::tools::Reporter_EXIT<B,R>;
#endif
// ==================================================================================== explicit template instantiation
