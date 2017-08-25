#include <iostream>
#include <iomanip>
#include <sstream>

#include "Tools/Display/bash_tools.h"

#include "Terminal_EXIT.hpp"

using namespace aff3ct::tools;

Terminal_EXIT
::Terminal_EXIT(const int     N,
                const int    &cur_t,
                const int    &trials,
                const double &I_A,
                const double &I_E)
: Terminal(),
  N(N),
  esn0(0.f),
  ebn0(0.f),
  sig_a(0.f),
  t_snr(std::chrono::steady_clock::now()),
  cur_t(cur_t),
  trials(trials),
  I_A(I_A),
  I_E(I_E),
  real_time_state(0)
{
}

std::string Terminal_EXIT
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

void Terminal_EXIT
::set_esn0(const float esn0)
{
	this->esn0 = esn0;
}

void Terminal_EXIT
::set_ebn0(const float ebn0)
{
	this->ebn0 = ebn0;
}

void Terminal_EXIT
::set_sig_a(const float sig_a)
{
	this->sig_a = sig_a;
}

void Terminal_EXIT
::legend(std::ostream &stream)
{
	stream << "# " << format("-----------------------------------------------||---------------------", Style::BOLD) << std::endl;
	stream << "# " << format("    EXIT chart depending on the Signal Noise   ||  Global throughput  ", Style::BOLD) << std::endl;
	stream << "# " << format("      Ratio (SNR) and the channel A noise      ||  and elapsed time   ", Style::BOLD) << std::endl;
	stream << "# " << format("-----------------------------------------------||---------------------", Style::BOLD) << std::endl;
	stream << "# " << format("-------|-------|-------|-----------|-----------||----------|----------", Style::BOLD) << std::endl;
	stream << "# " << format(" Es/N0 | Eb/N0 | SIG_A |  A_PRIORI | EXTRINSIC || SIM_CTHR |    ET/RT ", Style::BOLD) << std::endl;
	stream << "# " << format("  (dB) |  (dB) |  (dB) |     (I_A) |     (I_E) ||   (Mb/s) | (hhmmss) ", Style::BOLD) << std::endl;
	stream << "# " << format("-------|-------|-------|-----------|-----------||----------|----------", Style::BOLD) << std::endl;
}

void Terminal_EXIT
::_report(std::ostream &stream)
{
	using namespace std::chrono;
	using namespace std;

	auto simu_time = (float)duration_cast<nanoseconds>(steady_clock::now() - t_snr).count() * 0.000000001f;
	auto simu_cthr = (N * cur_t) /
		              simu_time ; // = bps
	simu_cthr /= 1000.f; // = kbps
	simu_cthr /= 1000.f; // = mbps

	stream << "   ";
	stream << setprecision(2) << fixed << setw(5) << esn0  << format(" | ",  Style::BOLD);
	stream << setprecision(2) << fixed << setw(5) << ebn0  << format(" | ",  Style::BOLD);
	stream << setprecision(2) << fixed << setw(5) << sig_a << format(" | ",  Style::BOLD);
	stream << setprecision(6) << fixed << setw(9) << I_A   << format(" | ",  Style::BOLD);
	stream << setprecision(6) << fixed << setw(9) << I_E   << format(" || ", Style::BOLD);
	stream << setprecision(2) << fixed << setw(8) << simu_cthr;
}

void Terminal_EXIT
::temp_report(std::ostream &stream)
{
	using namespace std::chrono;

	_report(stream);

	auto et = duration_cast<milliseconds>(steady_clock::now() - t_snr).count() / 1000.f;
	auto tr = et * (trials / cur_t) - et;
	auto tr_format = get_time_format((cur_t == 0) ? 0 : tr);

	stream << format(" | ", Style::BOLD) << std::setprecision(0) << std::fixed << std::setw(8) << tr_format;

	stream << " ";
	switch (real_time_state)
	{
		case 0: stream << format("*", Style::BOLD | FG::Color::GREEN); break;
		case 1: stream << format("*", Style::BOLD | FG::Color::GREEN); break;
		case 2: stream << format(" ", Style::BOLD | FG::Color::GREEN); break;
		case 3: stream << format(" ", Style::BOLD | FG::Color::GREEN); break;
		default: break;
	}
	real_time_state = (real_time_state +1) % 4;
	stream << "\r";

	stream.flush();
}

void Terminal_EXIT
::final_report(std::ostream &stream)
{
	using namespace std::chrono;

	this->_report(stream);

	auto et = duration_cast<milliseconds>(steady_clock::now() - t_snr).count() / 1000.f;
	auto et_format = get_time_format(et);

	stream << format(" | ", Style::BOLD) << std::setprecision(0) << std::fixed << std::setw(8) << et_format << "  "
	       << std::endl;

	t_snr = std::chrono::steady_clock::now();
}
