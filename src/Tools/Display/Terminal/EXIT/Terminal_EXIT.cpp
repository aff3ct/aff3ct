#include <iostream>
#include <iomanip>
#include <sstream>
#include <ios>

#include <rang.hpp>

#include "Terminal_EXIT.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename R>
Terminal_EXIT<B,R>
::Terminal_EXIT(const module::Monitor_EXIT<B,R> &monitor)
: Terminal(),
  monitor(monitor),
  esn0(0.f),
  ebn0(0.f),
  sig_a(0.f),
  t_snr(std::chrono::steady_clock::now()),
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
#ifdef _MSC_VER
	sprintf_s(time_format, 32, "%2.2dh%2.2d'%2.2d", hh, mm, ss);
#else
	sprintf(time_format, "%2.2dh%2.2d'%2.2d", hh, mm, ss);
#endif
	std::string time_format2(time_format);

	return time_format2;
}

template <typename B, typename R>
void Terminal_EXIT<B,R>
::set_esn0(const float esn0)
{
	this->esn0 = esn0;
}

template <typename B, typename R>
void Terminal_EXIT<B,R>
::set_ebn0(const float ebn0)
{
	this->ebn0 = ebn0;
}

template <typename B, typename R>
void Terminal_EXIT<B,R>
::set_sig_a(const float sig_a)
{
	this->sig_a = sig_a;
}

template <typename B, typename R>
void Terminal_EXIT<B,R>
::legend(std::ostream &stream)
{
	std::ios::fmtflags f(stream.flags());

	stream << "# " << rang::style::bold << "----------------------------------------------------------||---------------------" << rang::style::reset << std::endl;
	stream << "# " << rang::style::bold << "   EXIT chart depending on the Signal Noise Ratio (SNR)   ||  Global throughput  " << rang::style::reset << std::endl;
	stream << "# " << rang::style::bold << "                  and the channel A noise                 ||  and elapsed time   " << rang::style::reset << std::endl;
	stream << "# " << rang::style::bold << "----------------------------------------------------------||---------------------" << rang::style::reset << std::endl;
	stream << "# " << rang::style::bold << "-------|-------|-------|----------|-----------|-----------||----------|----------" << rang::style::reset << std::endl;
	stream << "# " << rang::style::bold << " Es/N0 | Eb/N0 | SIG_A |      FRA |  A_PRIORI | EXTRINSIC ||  SIM_THR |    ET/RT " << rang::style::reset << std::endl;
	stream << "# " << rang::style::bold << "  (dB) |  (dB) |  (dB) |          |     (I_A) |     (I_E) ||   (Mb/s) | (hhmmss) " << rang::style::reset << std::endl;
	stream << "# " << rang::style::bold << "-------|-------|-------|----------|-----------|-----------||----------|----------" << rang::style::reset << std::endl;

	stream.flags(f);
}

template <typename B, typename R>
void Terminal_EXIT<B,R>
::_report(std::ostream &stream)
{
	using namespace std::chrono;
	using namespace std;

	const auto size = monitor.get_size();
	const auto fra  = monitor.get_n_analyzed_fra();
	const auto I_A  = monitor.get_I_A();
	const auto I_E  = monitor.get_I_E();

	auto simu_time = (float)duration_cast<nanoseconds>(steady_clock::now() - t_snr).count() * 0.000000001f;
	auto simu_cthr = (size * fra) / simu_time ; // = bps
	simu_cthr /= 1000.f; // = kbps
	simu_cthr /= 1000.f; // = mbps

	stream << "   ";
	stream << setprecision(2) << fixed << setw(5) << esn0  << rang::style::bold << " | "  << rang::style::reset;
	stream << setprecision(2) << fixed << setw(5) << ebn0  << rang::style::bold << " | "  << rang::style::reset;
	stream << setprecision(2) << fixed << setw(5) << sig_a << rang::style::bold << " | "  << rang::style::reset;
	stream << setprecision(2) << fixed << setw(8) << fra   << rang::style::bold << " | "  << rang::style::reset;
	stream << setprecision(6) << fixed << setw(9) << I_A   << rang::style::bold << " | "  << rang::style::reset;
	stream << setprecision(6) << fixed << setw(9) << I_E   << rang::style::bold << " || " << rang::style::reset;
	stream << setprecision(2) << fixed << setw(8) << simu_cthr;
}

template <typename B, typename R>
void Terminal_EXIT<B,R>
::temp_report(std::ostream &stream)
{
	using namespace std::chrono;

	std::ios::fmtflags f(stream.flags());

	_report(stream);

	const auto n_trials = monitor.get_n_trials();
	const auto cur_fra = monitor.get_n_analyzed_fra();

	auto et = duration_cast<milliseconds>(steady_clock::now() - t_snr).count() / 1000.f;
	auto tr = et * (n_trials / cur_fra) - et;
	auto tr_format = get_time_format((cur_fra == 0) ? 0 : tr);

	stream << rang::style::bold << " | " << rang::style::reset << std::setprecision(0) << std::fixed << std::setw(8)
	       << tr_format << " ";

	switch (real_time_state)
	{
		case 0: stream << rang::style::bold << rang::fg::green << "*" << rang::style::reset; break;
		case 1: stream << rang::style::bold << rang::fg::green << "*" << rang::style::reset; break;
		case 2: stream << rang::style::bold << rang::fg::green << " " << rang::style::reset; break;
		case 3: stream << rang::style::bold << rang::fg::green << " " << rang::style::reset; break;
		default: break;
	}
	real_time_state = (real_time_state +1) % 4;
	stream << "\r";

	stream.flush();
	stream.flags(f);
}

template <typename B, typename R>
void Terminal_EXIT<B,R>
::final_report(std::ostream &stream)
{
	using namespace std::chrono;

	std::ios::fmtflags f(stream.flags());

	Terminal::final_report(stream);

	this->_report(stream);

	auto et = duration_cast<milliseconds>(steady_clock::now() - t_snr).count() / 1000.f;
	auto et_format = get_time_format(et);

	stream << rang::style::bold << " | " << rang::style::reset << std::setprecision(0) << std::fixed << std::setw(8)
	       << et_format << "  " << std::endl;

	t_snr = std::chrono::steady_clock::now();

	stream.flags(f);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Terminal_EXIT<B_32,R_32>;
template class aff3ct::tools::Terminal_EXIT<B_64,R_64>;
#else
template class aff3ct::tools::Terminal_EXIT<B,R>;
#endif
// ==================================================================================== explicit template instantiation
