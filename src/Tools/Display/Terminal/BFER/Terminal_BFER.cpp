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
  t_snr          (std::chrono::steady_clock::now()),
  real_time_state(0                               )
{
}

template <typename B>
Terminal_BFER<B>
::~Terminal_BFER()
{
}

template <typename B>
void Terminal_BFER<B>
::set_noise(const Noise<float>& n)
{
	this->noise = n;
}

template <typename B>
void Terminal_BFER<B>
::set_noise(const Noise<double>& n)
{
	this->noise.set_noise(n.get_noise(), n.get_type());
}

template <typename B>
void Terminal_BFER<B>
::legend(std::ostream &stream)
{
	this->cols_groups.resize(2);

	auto& bfer_title       = this->cols_groups[0].first;
	auto& bfer_cols        = this->cols_groups[0].second;
	auto& throughput_title = this->cols_groups[1].first;
	auto& throughput_cols  = this->cols_groups[1].second;

	bfer_title = std::make_pair("Bit Error Rate (BER) and Frame Error Rate (FER)", "");

	switch (this->noise.get_type())
	{
		case Noise_type::SIGMA :
			bfer_title.second = "depending on the Signal Noise Ratio (SNR)";
			bfer_cols.push_back(std::make_pair("Es/N0", "(dB)"));
			bfer_cols.push_back(std::make_pair("Eb/N0", "(dB)"));
		break;
		case Noise_type::ROP :
			bfer_title.second = "depending on the Received Optical Power (ROP)";
			bfer_cols.push_back(std::make_pair("ROP", "(dB)"));
		break;
		case Noise_type::EP :
			bfer_title.second = "depending on the Erasure Probability (EP)";
			bfer_cols.push_back(std::make_pair("EP", ""));
		break;
	}

	bfer_cols.push_back(std::make_pair("FRA", ""));
	bfer_cols.push_back(std::make_pair("BE",  ""));
	bfer_cols.push_back(std::make_pair("FE",  ""));
	bfer_cols.push_back(std::make_pair("BER", ""));
	bfer_cols.push_back(std::make_pair("FER", ""));

	throughput_title = std::make_pair("Global throughput", "and elapsed time");
	throughput_cols.push_back(std::make_pair("SIM_THR", "(Mb/s)"));
	throughput_cols.push_back(std::make_pair("ET/RT", "(hhmmss)"));

	// stream << "# " << "---------------------------------------------------------------------||---------------------" << std::endl;
	// stream << "# " << "      Bit Error Rate (BER) and Frame Error Rate (FER) depending      ||  Global throughput  " << std::endl;
	// stream << "# " << "                   on the Erasure Probability (EP)                   ||  and elapsed time   " << std::endl;
	// stream << "# " << "---------------------------------------------------------------------||---------------------" << std::endl;
	// stream << "# " << "---------|-----------|-----------|-----------|-----------|-----------||----------|----------" << std::endl;
	// stream << "# " << "      EP |       FRA |        BE |        FE |       BER |       FER ||  SIM_THR |    ET/RT " << std::endl;
	// stream << "# " << "         |           |           |           |           |           ||   (Mb/s) | (hhmmss) " << std::endl;
	// stream << "# " << "---------|-----------|-----------|-----------|-----------|-----------||----------|----------" << std::endl;

	Terminal::legend(stream); // print effectively the legend
}

std::string get_spaces_left(const std::string& str, const int total_width)
{
	return std::string(total_width - str.size() -1, ' '); // -1 because a space is reserved for the right
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
	auto simu_cthr = ((float)monitor.get_size() * (float)monitor.get_n_analyzed_fra()) / simu_time ; // = bps
	simu_cthr /= 1000.f; // = kbps
	simu_cthr /= 1000.f; // = mbps

	if (module::Monitor::is_interrupt()) stream << "\r";

	stream << data_tag;

	const auto report_style = rang::style::bold;

	switch (this->noise.get_type())
	{
		case Noise_type::SIGMA :
			stream << setprecision(2) << fixed << setw(column_width-1) << this->noise.get_esn0() << report_style
			       << spaced_scol_separator << rang::style::reset;
			stream << setprecision(2) << fixed << setw(column_width-1) << this->noise.get_ebn0() << report_style
			       << spaced_scol_separator << rang::style::reset;
			break;
		case Noise_type::ROP :
			stream << setprecision(2) << fixed << setw(column_width-1) << this->noise.get_rop() << report_style
			       << spaced_scol_separator << rang::style::reset;
			break;
		case Noise_type::EP :
			stream << setprecision(4) << fixed << setw(column_width-1) << this->noise.get_ep() << report_style
			       << spaced_scol_separator << rang::style::reset;
			break;
	}

	stringstream str_ber, str_fer;
	str_ber << setprecision(2) << scientific << setw(column_width-1) << ber;
	str_fer << setprecision(2) << scientific << setw(column_width-1) << fer;

	const unsigned long long l = 99999999;  // limit 0

	stream << setprecision((fra > l) ? 2 : 0) << ((fra > l) ? scientific : fixed) << setw(column_width-1) << ((fra > l) ? (float)fra : fra) << report_style << spaced_scol_separator << rang::style::reset;
	stream << setprecision(( be > l) ? 2 : 0) << ((be  > l) ? scientific : fixed) << setw(column_width-1) << (( be > l) ? (float) be :  be) << report_style << spaced_scol_separator << rang::style::reset;
	stream << setprecision(( fe > l) ? 2 : 0) << ((fe  > l) ? scientific : fixed) << setw(column_width-1) << (( fe > l) ? (float) fe :  fe) << report_style << spaced_scol_separator << rang::style::reset;
	stream <<                                                                                                                 str_ber.str() << report_style << spaced_scol_separator << rang::style::reset;
	stream <<                                                                                                                 str_fer.str() << report_style << spaced_dcol_separator << rang::style::reset;
	stream << setprecision(                2) <<                           fixed  << setw(column_width-1) <<                      simu_cthr;
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

	stream << rang::style::bold << spaced_scol_separator << rang::style::reset << std::setprecision(0) << std::fixed
	       << std::setw(column_width-1) << tr_format;

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

	stream << rang::style::bold << spaced_scol_separator << rang::style::reset << std::setprecision(0) << std::fixed
	       << std::setw(column_width-1) << et_format;

	stream << (module::Monitor::is_interrupt() ? " x" : "  ") << std::endl;

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
