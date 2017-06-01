#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include "Terminal_BFER_legacy.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Terminal_BFER_legacy<B>
::Terminal_BFER_legacy(const int K,
                       const int N,
                       const Monitor<B> &monitor,
                       const float &snr)
: Terminal(), K(K), N(N), snr(snr), monitor(monitor), t_snr(std::chrono::steady_clock::now()), real_time_state(0)
{
	if (K <= 0)
		throw std::invalid_argument("aff3ct::tools::Terminal_BFER_legacy: \"K\" has to be greater than 0.");
	if (N <= 0)
		throw std::invalid_argument("aff3ct::tools::Terminal_BFER_legacy: \"N\" has to be greater than 0.");
	if (K > N)
		throw std::invalid_argument("aff3ct::tools::Terminal_BFER_legacy: \"K\" has to be smaller than \"N\".");
}

template <typename B>
std::string Terminal_BFER_legacy<B>::get_time_format(float secondes)
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

template <typename B>
void Terminal_BFER_legacy<B>::_report(std::ostream &stream)
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
		ber = (1.f) / ((float)monitor.get_n_analyzed_fra()) / this->K;
		fer = (1.f) / ((float)monitor.get_n_analyzed_fra());
	}

	auto rt   = duration_cast<milliseconds>(steady_clock::now() - t_snr).count() / 1000.f;
	auto fpmn = (60.f * monitor.get_n_analyzed_fra()) / rt;
	auto bps  = (fpmn * (float)this->N) / 60.f / 1000.f / 1000.f;

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

template <typename B>
void Terminal_BFER_legacy<B>::temp_report(std::ostream &stream)
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

template <typename B>
void Terminal_BFER_legacy<B>::final_report(std::ostream &stream)
{
	using namespace std::chrono;

	_report(stream);

	auto rt        = duration_cast<milliseconds>(steady_clock::now() - t_snr).count() / 1000.f;
	auto rt_format = get_time_format(rt);

	stream << " | RUNTIME = " << std::setprecision(0) << std::fixed << std::setw(8) << rt_format << "     "
	       << std::endl;

	t_snr = std::chrono::steady_clock::now();
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Terminal_BFER_legacy<B_8>;
template class aff3ct::tools::Terminal_BFER_legacy<B_16>;
template class aff3ct::tools::Terminal_BFER_legacy<B_32>;
template class aff3ct::tools::Terminal_BFER_legacy<B_64>;
#else
template class aff3ct::tools::Terminal_BFER_legacy<B>;
#endif
// ==================================================================================== explicit template instantiation
