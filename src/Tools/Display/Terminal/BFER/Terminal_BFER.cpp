#include <iostream>
#include <iomanip>
#include <sstream>
#include <ios>
#include <rang.hpp>

#include "Tools/Exception/exception.hpp"
#include "Tools/Noise/noise_utils.h"

#include "Terminal_BFER.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename R>
Terminal_BFER<B,R>
::Terminal_BFER(const module::Monitor_BFER<B,R> &monitor, bool display_mutinfo, bool display_bfer)
: Terminal       (                                ),
  monitor        (monitor                         ),
  t_snr          (std::chrono::steady_clock::now()),
  real_time_state(0                               ),
  n              (nullptr                         ),
  display_mutinfo(display_mutinfo                 ),
  display_bfer   (display_bfer                    )
{
}

template<typename B, typename R>
Terminal_BFER<B, R>::~Terminal_BFER()
{
	if (this->n != nullptr)
		delete this->n;
}

template <typename B, typename R>
void Terminal_BFER<B,R>
::set_noise(const Noise<float>& noise)
{
	if (this->n != nullptr)
		delete this->n;

	this->n = tools::cast<float>(noise);
}

template <typename B, typename R>
void Terminal_BFER<B,R>
::set_noise(const Noise<double>& noise)
{
	if (this->n != nullptr)
		delete this->n;

	this->n = tools::cast<float>(noise);
}

template <typename B, typename R>
void Terminal_BFER<B,R>
::legend(std::ostream &stream)
{
	this->cols_groups.resize(2);

	auto& bfer_title       = this->cols_groups[0].first;
	auto& bfer_cols        = this->cols_groups[0].second;
	auto& throughput_title = this->cols_groups[1].first;
	auto& throughput_cols  = this->cols_groups[1].second;

	if (display_mutinfo)
		bfer_title.first = "MutInfo (MI)";

	if (display_bfer)
	{
		if (!bfer_title.first.empty())
			bfer_title.first += ", ";

		bfer_title.first += "Bit Error Rate (BER) and Frame Error Rate (FER)";
	}

	bfer_cols.clear();

	if (this->n == nullptr)
	{
		std::stringstream message;
		message << "Undefined noise.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	switch (this->n->get_type())
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

	if (display_mutinfo)
		bfer_cols.push_back(std::make_pair("MI", ""));

	if (display_bfer)
	{
		bfer_cols.push_back(std::make_pair("FRA", ""));
		bfer_cols.push_back(std::make_pair("BE", ""));
		bfer_cols.push_back(std::make_pair("FE", ""));
		bfer_cols.push_back(std::make_pair("BER", ""));
		bfer_cols.push_back(std::make_pair("FER", ""));
	}

	throughput_title = std::make_pair("Global throughput", "and elapsed time");
	throughput_cols.clear();
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

template <typename B, typename R>
void Terminal_BFER<B,R>
::_report(std::ostream &stream)
{
	using namespace std::chrono;
	using namespace std;

	auto ber = monitor.get_ber();
	auto fer = monitor.get_fer();
	auto fra = monitor.get_n_analyzed_fra();
	auto be  = monitor.get_n_be();
	auto fe  = monitor.get_n_fe();
	auto MI  = monitor.get_MI  ();

	auto simu_time = (float)duration_cast<nanoseconds>(steady_clock::now() - t_snr).count() * 0.000000001f;
	auto simu_cthr = ((float)monitor.get_K() * (float)monitor.get_n_analyzed_fra()) / simu_time ; // = bps
	simu_cthr /= 1000.f; // = kbps
	simu_cthr /= 1000.f; // = mbps

	if (module::Monitor::is_interrupt()) stream << "\r";

	stream << data_tag;

	const auto report_style = rang::style::bold;

	if (this->n == nullptr)
	{
		std::stringstream message;
		message << "Undefined noise.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	stream << std::string(extra_spaces(cols_groups[0]), ' ');

	switch (this->n->get_type())
	{
		case Noise_type::SIGMA :
		{
			auto sig = dynamic_cast<const tools::Sigma<>*>(this->n);
			stream << setprecision(2) << fixed << setw(column_width - 1) << sig->get_esn0() << report_style << spaced_scol_separator << rang::style::reset;
			stream << setprecision(2) << fixed << setw(column_width - 1) << sig->get_ebn0();
			break;
		}
		case Noise_type::ROP :
		{
			stream << setprecision(2) << fixed << setw(column_width - 1) << this->n->get_noise();;
			break;
		}
		case Noise_type::EP :
		{
			stream << setprecision(4) << fixed << setw(column_width - 1) << this->n->get_noise();;
			break;
		}
	}
	stream << report_style << ((display_bfer || display_mutinfo) ? spaced_scol_separator : spaced_dcol_separator) << rang::style::reset;


	stringstream str_ber, str_fer, str_MI;
	str_ber << setprecision(2) << scientific << setw(column_width-1) << ber;
	str_fer << setprecision(2) << scientific << setw(column_width-1) << fer;
	str_MI  << setprecision(2) << scientific << setw(column_width-1) << MI;

	const unsigned long long l = 99999999;  // limit 0

	if (display_mutinfo)
		stream << str_MI.str() << report_style << (display_bfer ? spaced_scol_separator : spaced_dcol_separator) << rang::style::reset;

	if (display_bfer)
	{
		stream << setprecision((fra > l) ? 2 : 0) << ((fra > l) ? scientific : fixed) << setw(column_width - 1) << ((fra > l) ? (float) fra : fra) << report_style << spaced_scol_separator << rang::style::reset;
		stream << setprecision((be  > l) ? 2 : 0) << ((be  > l) ? scientific : fixed) << setw(column_width - 1) << ((be  > l) ? (float) be  : be ) << report_style << spaced_scol_separator << rang::style::reset;
		stream << setprecision((fe  > l) ? 2 : 0) << ((fe  > l) ? scientific : fixed) << setw(column_width - 1) << ((fe  > l) ? (float) fe  : fe ) << report_style << spaced_scol_separator << rang::style::reset;

		stream << str_ber.str() << report_style << spaced_scol_separator << rang::style::reset;
		stream << str_fer.str() << report_style << spaced_dcol_separator << rang::style::reset;
	}

	stream << std::string(extra_spaces(cols_groups[1]), ' ');
	stream << setprecision(2) << fixed  << setw(column_width-1) << simu_cthr;
}

template <typename B, typename R>
void Terminal_BFER<B,R>
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
	real_time_state = (real_time_state + (unsigned short)1) % (unsigned short)4;
	stream << "\r";

	stream.flush();
	stream.flags(f);
}

template <typename B, typename R>
void Terminal_BFER<B,R>
::final_report(std::ostream &stream)
{
	using namespace std::chrono;

	std::ios::fmtflags f(stream.flags());

	Terminal::final_report(stream);

	auto et = duration_cast<milliseconds>(steady_clock::now() - t_snr).count() / 1000.f;

	if (!module::Monitor::is_over() || et >= 1.f)
	{
		this->_report(stream);

		auto et_format = get_time_format(et);

		stream << rang::style::bold << spaced_scol_separator << rang::style::reset << std::setprecision(0) << std::fixed
		       << std::setw(column_width - 1) << et_format;

		stream << (module::Monitor::is_interrupt() ? " x" : "  ") << std::endl;
	}

	t_snr = std::chrono::steady_clock::now();

	stream.flags(f);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Terminal_BFER<B_8, R_8>;
template class aff3ct::tools::Terminal_BFER<B_16,R_16>;
template class aff3ct::tools::Terminal_BFER<B_32,R_32>;
template class aff3ct::tools::Terminal_BFER<B_64,R_64>;
#else
template class aff3ct::tools::Terminal_BFER<B,R>;
#endif
// ==================================================================================== explicit template instantiation
