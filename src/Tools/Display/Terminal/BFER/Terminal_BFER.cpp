#include <iostream>
#include <iomanip>
#include <sstream>
#include <ios>

#include <rang.hpp>

#include "Tools/Exception/exception.hpp"

#include "Terminal_BFER.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B>
Terminal_BFER<B>
::Terminal_BFER(const module::Monitor_BFER<B> &monitor)
: Terminal       (                                ),
  monitor        (monitor                         ),
  esn0           (0.f                             ),
  ebn0           (0.f                             ),
  is_esn0        (false                           ),
  is_ebn0        (false                           ),
  t_snr          (std::chrono::steady_clock::now()),
  real_time_state(0                               )
{
}

template <typename B>
void Terminal_BFER<B>
::set_esn0(const float esn0)
{
	this->is_esn0 = true;
	this->esn0    = esn0;
}

template <typename B>
void Terminal_BFER<B>
::set_ebn0(const float ebn0)
{
	this->is_ebn0 = true;
	this->ebn0    = ebn0;
}

template <typename B>
std::string Terminal_BFER<B>
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

template <typename B>
void Terminal_BFER<B>
::legend(std::ostream &stream)
{
	std::ios::fmtflags f(stream.flags());

#ifdef _WIN32
	stream << "# " << rang::style::bold << "---------------------------------------------------------------------------||---------------------" << rang::style::reset << std::endl;
	stream << "# " << rang::style::bold << "         Bit Error Rate (BER) and Frame Error Rate (FER) depending         ||  Global throughput  " << rang::style::reset << std::endl;
	stream << "# " << rang::style::bold << "                      on the Signal Noise Ratio (SNR)                      ||  and elapsed time   " << rang::style::reset << std::endl;
	stream << "# " << rang::style::bold << "---------------------------------------------------------------------------||---------------------" << rang::style::reset << std::endl;
	stream << "# " << rang::style::bold << "-------|-------|-----------|-----------|-----------|-----------|-----------||----------|----------" << rang::style::reset << std::endl;
	stream << "# " << rang::style::bold << " Es/N0 | Eb/N0 |       FRA |        BE |        FE |       BER |       FER ||  SIM_THR |    ET/RT " << rang::style::reset << std::endl;
	stream << "# " << rang::style::bold << "  (dB) |  (dB) |           |           |           |           |           ||   (Mb/s) | (hhmmss) " << rang::style::reset << std::endl;
	stream << "# " << rang::style::bold << "-------|-------|-----------|-----------|-----------|-----------|-----------||----------|----------" << rang::style::reset << std::endl;

#else
	stream << "# " << rang::style::bold << "----------------------------------------------------------------------||---------------------" << rang::style::reset << std::endl;
	stream << "# " << rang::style::bold << "      Bit Error Rate (BER) and Frame Error Rate (FER) depending       ||  Global throughput  " << rang::style::reset << std::endl;
	stream << "# " << rang::style::bold << "                   on the Signal Noise Ratio (SNR)                    ||  and elapsed time   " << rang::style::reset << std::endl;
	stream << "# " << rang::style::bold << "----------------------------------------------------------------------||---------------------" << rang::style::reset << std::endl;
	stream << "# " << rang::style::bold << "-------|-------|----------|----------|----------|----------|----------||----------|----------" << rang::style::reset << std::endl;
	stream << "# " << rang::style::bold << " Es/N0 | Eb/N0 |      FRA |       BE |       FE |      BER |      FER ||  SIM_THR |    ET/RT " << rang::style::reset << std::endl;
	stream << "# " << rang::style::bold << "  (dB) |  (dB) |          |          |          |          |          ||   (Mb/s) | (hhmmss) " << rang::style::reset << std::endl;
	stream << "# " << rang::style::bold << "-------|-------|----------|----------|----------|----------|----------||----------|----------" << rang::style::reset << std::endl;
#endif

	stream.flags(f);
}

template <typename B>
void Terminal_BFER<B>
::_report(std::ostream &stream)
{
	using namespace std::chrono;
	using namespace std;

	auto ber = monitor.get_ber();
	auto fer = monitor.get_fer();
	auto fra = monitor.get_n_analyzed_fra();
	auto be  = monitor.get_n_be();
	auto fe  = monitor.get_n_fe();

	auto simu_time = (float)duration_cast<nanoseconds>(steady_clock::now() - t_snr).count() * 0.000000001f;
	auto simu_cthr = ((float)monitor.get_size() * (float)monitor.get_n_analyzed_fra()) /
		              simu_time ; // = bps
	simu_cthr /= 1000.f; // = kbps
	simu_cthr /= 1000.f; // = mbps

	if (module::Monitor::is_interrupt()) stream << "\r";

	std::stringstream esn0_str;
	if (!is_esn0)
		esn0_str << "   -  ";
	else
		esn0_str << setprecision(2) << fixed << setw(6) << esn0;

	std::stringstream ebn0_str;
	if (!is_ebn0)
		ebn0_str << "  -  ";
	else
		ebn0_str << setprecision(2) << fixed << setw(5) << ebn0;

#ifdef _WIN32
	stringstream str_ber, str_fer;
	str_ber << setprecision(2) << scientific << setw(9) << ber;
	str_fer << setprecision(2) << scientific << setw(9) << fer;

	unsigned long long l0 = 99999999;  // limit 0
	unsigned long long l1 = 99999999;  // limit 1
//	auto               l2 = 99999.99f; // limit 2
	stream << "  ";
	stream <<                                                                                                     esn0_str.str()  << rang::style::bold << " | "  << rang::style::reset;
	stream <<                                                                                                     ebn0_str.str()  << rang::style::bold << " | "  << rang::style::reset;
	stream << setprecision((fra > l0) ? 2 : 0) << ((fra > l0) ? scientific : fixed) << setw(9) << ((fra > l0) ? (float)fra : fra) << rang::style::bold << " | "  << rang::style::reset;
	stream << setprecision(( be > l1) ? 2 : 0) << ((be  > l1) ? scientific : fixed) << setw(9) << (( be > l1) ? (float) be :  be) << rang::style::bold << " | "  << rang::style::reset;
	stream << setprecision(( fe > l1) ? 2 : 0) << ((fe  > l1) ? scientific : fixed) << setw(9) << (( fe > l1) ? (float) fe :  fe) << rang::style::bold << " | "  << rang::style::reset;
	stream <<                                                                                                       str_ber.str() << rang::style::bold << " | "  << rang::style::reset;
	stream <<                                                                                                       str_fer.str() << rang::style::bold << " || " << rang::style::reset;
	stream << setprecision(                 2) <<                            fixed  << setw(8) <<                       simu_cthr;
#else
	stringstream str_ber, str_fer;
	str_ber << setprecision(2) << scientific << setw(8) << ber;
	str_fer << setprecision(2) << scientific << setw(8) << fer;

	unsigned long long l0 = 99999999;  // limit 0
	unsigned long long l1 = 99999999;  // limit 1
	stream << "  ";
	stream <<                                                                                                     esn0_str.str()  << rang::style::bold << " | "  << rang::style::reset;
	stream <<                                                                                                     ebn0_str.str()  << rang::style::bold << " | "  << rang::style::reset;
	stream << setprecision((fra > l0) ? 2 : 0) << ((fra > l0) ? scientific : fixed) << setw(8) << ((fra > l0) ? (float)fra : fra) << rang::style::bold << " | "  << rang::style::reset;
	stream << setprecision(( be > l1) ? 2 : 0) << ((be  > l1) ? scientific : fixed) << setw(8) << (( be > l1) ? (float) be :  be) << rang::style::bold << " | "  << rang::style::reset;
	stream << setprecision(( fe > l1) ? 2 : 0) << ((fe  > l1) ? scientific : fixed) << setw(8) << (( fe > l1) ? (float) fe :  fe) << rang::style::bold << " | "  << rang::style::reset;
	stream <<                                                                                                       str_ber.str() << rang::style::bold << " | "  << rang::style::reset;
	stream <<                                                                                                       str_fer.str() << rang::style::bold << " || " << rang::style::reset;
	stream << setprecision(                 2) <<                            fixed  << setw(8) <<                       simu_cthr;
#endif
}

template <typename B>
void Terminal_BFER<B>
::temp_report(std::ostream &stream)
{
	using namespace std::chrono;

	std::ios::fmtflags f(stream.flags());

	_report(stream);

	auto et = duration_cast<milliseconds>(steady_clock::now() - t_snr).count() / 1000.f;
	auto tr = et * ((float)monitor.get_fe_limit() / (float)monitor.get_n_fe()) - et;
	auto tr_format = get_time_format((monitor.get_n_fe() == 0) ? 0 : tr);

	stream << rang::style::bold << " | " << rang::style::reset << std::setprecision(0) << std::fixed << std::setw(8) << tr_format;

	stream << " ";
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

template <typename B>
void Terminal_BFER<B>
::final_report(std::ostream &stream)
{
	using namespace std::chrono;

	std::ios::fmtflags f(stream.flags());

	Terminal::final_report(stream);

	this->_report(stream);

	auto et = duration_cast<milliseconds>(steady_clock::now() - t_snr).count() / 1000.f;
	auto et_format = get_time_format(et);

	stream << rang::style::bold << " | " << rang::style::reset << std::setprecision(0) << std::fixed << std::setw(8)
	       << et_format;

	if (module::Monitor::is_interrupt()) stream << " x" << std::endl;
	else                                 stream << "  " << std::endl;

	t_snr = std::chrono::steady_clock::now();

	stream.flags(f);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Terminal_BFER<B_8>;
template class aff3ct::tools::Terminal_BFER<B_16>;
template class aff3ct::tools::Terminal_BFER<B_32>;
template class aff3ct::tools::Terminal_BFER<B_64>;
#else
template class aff3ct::tools::Terminal_BFER<B>;
#endif
// ==================================================================================== explicit template instantiation
