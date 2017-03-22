#include <iostream>
#include <iomanip>
#include <sstream>

#include "Terminal_BFER_legacy.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Terminal_BFER_legacy<B,R>
::Terminal_BFER_legacy(const R& snr,
                       const Monitor<B,R> &monitor,
                       const std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> &t_snr)
: Terminal(), snr(snr), monitor(monitor), t_snr(t_snr), real_time_state(0)
{
}

template <typename B, typename R>
std::string Terminal_BFER_legacy<B,R>::get_time_format(float secondes)
{
	auto ss = (int)secondes % 60;
	auto mm = ((int)(secondes / 60.f) % 60);
	auto hh = (int)(secondes / 3600.f);

	// TODO: this is not a C++ style code
	char time_format[256];
#ifdef _MSC_VER
	sprintf_s(time_format, 32, "%2.2dh%2.2d'%2.2d", hh, mm, ss);
#else
	sprintf(time_format, "%2.2dh%2.2d'%2.2d", hh, mm, ss);
#endif
	std::string time_format2(time_format);

	return time_format2;
}

template <typename B, typename R>
void Terminal_BFER_legacy<B,R>::_report(std::ostream &stream)
{
	using namespace std::chrono;

	auto ber = 0.f, fer = 0.f, be_fe = 0.f;
	if (monitor.get_n_be() != 0)
	{
		ber   = (float)monitor.get_ber();
		fer   = (float)monitor.get_fer();
		be_fe = (float)monitor.get_n_be() / (float)monitor.get_n_fe();
	}
	else
	{
		ber = (1.f) / ((float)monitor.get_n_analyzed_fra()) / monitor.get_K();
		fer = (1.f) / ((float)monitor.get_n_analyzed_fra());
	}

	auto rt   = duration_cast<milliseconds>(steady_clock::now() - t_snr).count() / 1000.f;
	auto fpmn = (60.f * monitor.get_n_analyzed_fra()) / rt;
	auto bps  = (fpmn * (float)monitor.get_N()) / 60.f / 1000.f / 1000.f;

	auto fra = monitor.get_n_analyzed_fra();
	auto fe  = monitor.get_n_fe();
	auto be  = monitor.get_n_be();

	stream << "SNR = "      << std::setprecision(2) << std::fixed      << std::setw(4) << snr << " | ";
	stream << "BER = "      << std::setprecision(2) << std::scientific << std::setw(4) << ber << " | ";
	stream << "FER = "      << std::setprecision(2) << std::scientific << std::setw(4) << fer << " | ";
	stream << "BPS = "      << std::setprecision(2) << std::scientific << std::setw(4) << bps << " | ";
	stream << "MATRICES = " << std::setprecision(0) << std::fixed      << std::setw(7) << fra << " | ";
	stream << "FE = "       << std::setprecision(0) << std::fixed      << std::setw(4) << fe  << " | ";
	stream << "BE = "       << std::setprecision(0) << std::fixed      << std::setw(7) << be  << " | ";
	stream << "BE/FE = "    << std::setprecision(2) << std::fixed      << std::setw(7) << be_fe;
}

template <typename B, typename R>
void Terminal_BFER_legacy<B,R>::temp_report(std::ostream &stream)
{
	using namespace std::chrono;

	_report(stream);

	auto rt        = duration_cast<milliseconds>(steady_clock::now() - t_snr).count() / 1000.f;
	auto et        = rt * ((float)monitor.get_fe_limit() / (float)monitor.get_n_fe()) - rt;
	auto et_format = get_time_format((monitor.get_n_fe() == 0) ? 0 : et);

	stream << " | ESTTIME = " << std::setprecision(0) << std::fixed << std::setw(8) << et_format;

	stream << " ";
	for (auto i = 0; i <     real_time_state; i++) stream << ".";
	for (auto i = 0; i < 3 - real_time_state; i++) stream << " ";
	real_time_state = (real_time_state +1) % 4;
	stream << "\r";

	stream.flush();
}

template <typename B, typename R>
void Terminal_BFER_legacy<B,R>::final_report(std::ostream &stream)
{
	using namespace std::chrono;

	_report(stream);

	auto rt        = duration_cast<milliseconds>(steady_clock::now() - t_snr).count() / 1000.f;
	auto rt_format = get_time_format(rt);

	stream << " | RUNTIME = " << std::setprecision(0) << std::fixed << std::setw(8) << rt_format << "     "
	       << std::endl;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Terminal_BFER_legacy<B_8,R_8>;
template class aff3ct::tools::Terminal_BFER_legacy<B_16,R_16>;
template class aff3ct::tools::Terminal_BFER_legacy<B_32,R_32>;
template class aff3ct::tools::Terminal_BFER_legacy<B_64,R_64>;
#else
template class aff3ct::tools::Terminal_BFER_legacy<B,R>;
#endif
// ==================================================================================== explicit template instantiation
