#include <iostream>
#include <iomanip>
#include <sstream>

#include "Tools/Display/bash_tools.h"

#include "Terminal_EXIT.hpp"

template <typename B, typename R>
Terminal_EXIT<B,R>
::Terminal_EXIT(const int& N,
                const R& snr,
                const R& sig_a,
                const std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> &t_snr,
                const int& cur_t,
                const int& trials,
                double& I_A,
                double& I_E,
                const std::string name)
: Terminal(name.c_str()),
  N(N),
  snr(snr),
  sig_a(sig_a),
  t_snr(t_snr),
  cur_t(cur_t),
  trials(trials),
  I_A(I_A),
  I_E(I_E),
  real_time_state(0)
{
}

template <typename B, typename R>
std::string Terminal_EXIT<B,R>
::get_time_format(float secondes)
{
	auto ss = (int)secondes % 60;
	auto mm = ((int)(secondes / 60.f) % 60);
	auto hh = (int)(secondes / 3600.f);

	// TODO: this is not a C++ style code
	char time_format[256];
	sprintf(time_format, "%2.2dh%2.2d'%2.2d", hh, mm, ss);
	std::string time_format2(time_format);

	return time_format2;
}

template <typename B, typename R>
void Terminal_EXIT<B,R>
::legend(std::ostream &stream)
{
	stream << "# " << "------|-------|-----------|-----------||----------|----------" << std::endl;
	stream << "# " << "  SNR | SIG_A |       I_A |       I_E || SIM_CTHR |    ET/RT " << std::endl;
	stream << "# " << " (dB) |  (dB) |           |           ||   (Mb/s) | (hhmmss) " << std::endl;
	stream << "# " << "------|-------|-----------|-----------||----------|----------" << std::endl;
}

template <typename B, typename R>
void Terminal_EXIT<B,R>
::_report(std::ostream &stream)
{
	using namespace std::chrono;
	using namespace std;

	auto simu_time = (float)duration_cast<nanoseconds>(steady_clock::now() - t_snr).count() * 0.000000001f;
	auto simu_cthr = (N * cur_t) /
		              simu_time ; // = bps
	simu_cthr /= 1000.f; // = kbps
	simu_cthr /= 1000.f; // = mbps

	stream << "  ";
	stream << setprecision(2) << fixed << setw(5) << snr   << " | ";
	stream << setprecision(2) << fixed << setw(5) << sig_a << " | ";
	stream << setprecision(6) << fixed << setw(9) << I_A   << " | ";
	stream << setprecision(6) << fixed << setw(9) << I_E   << " || ";
	stream << setprecision(2) << fixed << setw(8) << simu_cthr;
}

template <typename B, typename R>
void Terminal_EXIT<B,R>
::temp_report(std::ostream &stream)
{
	using namespace std::chrono;

	_report(stream);

	auto et = duration_cast<milliseconds>(steady_clock::now() - t_snr).count() / 1000.f;
	auto tr = et * (trials / cur_t) - et;
	auto tr_format = get_time_format((cur_t == 0) ? 0 : tr);

	stream << " | " << std::setprecision(0) << std::fixed << std::setw(8) << tr_format;

	stream << " ";
	switch (real_time_state)
	{
		case 0: stream << "*"; break;
		case 1: stream << "*"; break;
		case 2: stream << " "; break;
		case 3: stream << " "; break;
		default: break;
	}
	real_time_state = (real_time_state +1) % 4;
	stream << "\r";

	stream.flush();
}

template <typename B, typename R>
void Terminal_EXIT<B,R>
::final_report(std::ostream &stream)
{
	using namespace std::chrono;

	this->_report(stream);

	auto et = duration_cast<milliseconds>(steady_clock::now() - t_snr).count() / 1000.f;
	auto et_format = get_time_format(et);

	stream << " | " << std::setprecision(0) << std::fixed << std::setw(8) << et_format << "  " << std::endl;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Terminal_EXIT<B_8,R_8>;
template class Terminal_EXIT<B_16,R_16>;
template class Terminal_EXIT<B_32,R_32>;
template class Terminal_EXIT<B_64,R_64>;
#else
template class Terminal_EXIT<B,R>;
#endif
// ==================================================================================== explicit template instantiation
