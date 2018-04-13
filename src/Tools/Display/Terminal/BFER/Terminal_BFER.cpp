#include <iostream>
#include <iomanip>
#include <sstream>
#include <utility>
#include <ios>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/bash_tools.h"

#include "Terminal_BFER.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

const char comment_tag    = '#';
const char col_separator  = '|';
const char line_separator = '-';
const std::string spaced_scol_separator = " |" ;
const std::string spaced_dcol_separator = " ||";

#ifdef _WIN32
const int column_width = 11;
#else
const int column_width = 10;
#endif

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

	// vector of pairs {group title, columns titles}
	// group title is a pair {first line, second line}
	// columns titles is a vector of pair {first line, second line}
	std::vector<std::pair<std::pair<std::string, std::string>, std::vector<std::pair<std::string, std::string>>>> cols_groups(2);


	auto& bfer_title       = cols_groups[0].first;
	auto& bfer_cols        = cols_groups[0].second;
	auto& throughput_title = cols_groups[1].first;
	auto& throughput_cols  = cols_groups[1].second;

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


	// stream << "# " << format("---------------------------------------------------------------------||---------------------", Style::BOLD) << std::endl;
	// stream << "# " << format("      Bit Error Rate (BER) and Frame Error Rate (FER) depending      ||  Global throughput  ", Style::BOLD) << std::endl;
	// stream << "# " << format("                   on the Erasure Probability (EP)                   ||  and elapsed time   ", Style::BOLD) << std::endl;
	// stream << "# " << format("---------------------------------------------------------------------||---------------------", Style::BOLD) << std::endl;
	// stream << "# " << format("---------|-----------|-----------|-----------|-----------|-----------||----------|----------", Style::BOLD) << std::endl;
	// stream << "# " << format("      EP |       FRA |        BE |        FE |       BER |       FER ||  SIM_THR |    ET/RT ", Style::BOLD) << std::endl;
	// stream << "# " << format("         |           |           |           |           |           ||   (Mb/s) | (hhmmss) ", Style::BOLD) << std::endl;
	// stream << "# " << format("---------|-----------|-----------|-----------|-----------|-----------||----------|----------", Style::BOLD) << std::endl;


	const auto legend_style = Style::BOLD;

	// print first line of the table
	stream << comment_tag << " ";
	for (unsigned i = 0; i < cols_groups.size(); i++)
	{
		const unsigned group_width = cols_groups[i].second.size()*(column_width+1)-1; // add a col separator between each exept for the last
		stream << format(std::string(group_width, line_separator), legend_style);

		if (i < (cols_groups.size() -1)) // print group separator except for last
			stream << format(std::string(2, col_separator), legend_style);
	}
	stream << std::endl;

	// print line 2 and 3 of the table (group title lines)
	for (auto l = 0; l < 2; l++)
	{
		stream << comment_tag << " ";
		for (unsigned i = 0; i < cols_groups.size(); i++)
		{
			const auto& text = l == 0 ? cols_groups[i].first.first : cols_groups[i].first.second;

			const unsigned group_width = cols_groups[i].second.size()*(column_width+1)-1; // add a col separator between each exept for the last
			const int n_spaces = (int)group_width - (int)text.size();
			const int n_spaces_left  = n_spaces/2;
			const int n_spaces_right = n_spaces - n_spaces_left; // can be different than n_spaces/2 if odd size
			stream << format(std::string(n_spaces_left,  ' '), legend_style);
			stream << format(text, legend_style);
			stream << format(std::string(n_spaces_right, ' '), legend_style);

			if (i < (cols_groups.size() -1)) // print group separator except for last
				stream << format(std::string(2, col_separator), legend_style);
		}
		stream << std::endl;
	}

	// print line 4 of the table
	stream << comment_tag << " ";
	for (unsigned i = 0; i < cols_groups.size(); i++)
	{
		const unsigned group_width = cols_groups[i].second.size()*(column_width+1)-1; // add a col separator between each exept for the last
		stream << format(std::string(group_width, line_separator), legend_style);

		if (i < (cols_groups.size() -1)) // print group separator except for last
			stream << format(std::string(2, col_separator), legend_style);
	}
	stream << std::endl;

	// print line 5 of the table
	stream << comment_tag << " ";
	for (unsigned i = 0; i < cols_groups.size(); i++)
	{
		for (unsigned j = 0; j < cols_groups[i].second.size(); j++)
		{
			stream << format(std::string(column_width, line_separator), legend_style);
			if (j < (cols_groups[i].second.size() -1)) // print column separator except for last
				stream << format(std::string(1, col_separator), legend_style);
		}

		if (i < (cols_groups.size() -1)) // print group separator except for last
			stream << format(std::string(2, col_separator), legend_style);
	}
	stream << std::endl;

	// print line 6 and 7 of the table (column title lines)
	for (auto l = 0; l < 2; l++)
	{
		stream << comment_tag << " ";
		for (unsigned i = 0; i < cols_groups.size(); i++)
		{

			for (unsigned j = 0; j < cols_groups[i].second.size(); j++)
			{
				const auto& text = l == 0 ? cols_groups[i].second[j].first : cols_groups[i].second[j].second;
				const int n_spaces = (int)column_width - (int)text.size() -1;
				stream << format(std::string(n_spaces,  ' '), legend_style);
				stream << format(text + " ", legend_style);

				if (j < (cols_groups[i].second.size() -1)) // print column separator except for last
					stream << format(std::string(1, col_separator), legend_style);
			}

			if (i < (cols_groups.size() -1)) // print group separator except for last
				stream << format(std::string(2, col_separator), legend_style);
		}
		stream << std::endl;
	}

	// print line 8 of the table
	stream << comment_tag << " ";
	for (unsigned i = 0; i < cols_groups.size(); i++)
	{
		for (unsigned j = 0; j < cols_groups[i].second.size(); j++)
		{
			stream << format(std::string(column_width, line_separator), legend_style);
			if (j < (cols_groups[i].second.size() -1)) // print column separator except for last
				stream << format(std::string(1, col_separator), legend_style);
		}

		if (i < (cols_groups.size() -1)) // print group separator except for last
			stream << format(std::string(2, col_separator), legend_style);
	}
	stream << std::endl;

	stream.flags(f);
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

	stream << "  " ; // left offset

	const std::string undefined_noise_tag = "- ";


	switch (this->noise.get_type())
	{
		case Noise_type::SIGMA :
			stream << setprecision(2) << fixed << setw(column_width-1) << this->noise.get_esn0() << format(spaced_scol_separator, Style::BOLD);
			stream << setprecision(2) << fixed << setw(column_width-1) << this->noise.get_ebn0() << format(spaced_scol_separator, Style::BOLD);
			break;
		case Noise_type::ROP :
			stream << setprecision(2) << fixed << setw(column_width-1) << this->noise.get_rop() << format(spaced_scol_separator, Style::BOLD);
			break;
		case Noise_type::EP :
			stream << setprecision(4) << fixed << setw(column_width-1) << this->noise.get_ep() << format(spaced_scol_separator, Style::BOLD);
			break;
	}

	stringstream str_ber, str_fer;
	str_ber << setprecision(2) << scientific << setw(column_width-1) << ber;
	str_fer << setprecision(2) << scientific << setw(column_width-1) << fer;

	const unsigned long long l0 = 99999999;  // limit 0
	const unsigned long long l1 = 99999999;  // limit 1

	stream << setprecision((fra > l) ? 2 : 0) << ((fra > l) ? scientific : fixed) << setw(column_width-1) << ((fra > l) ? (float)fra : fra) << format(spaced_scol_separator, Style::BOLD);
	stream << setprecision(( be > l) ? 2 : 0) << ((be  > l) ? scientific : fixed) << setw(column_width-1) << (( be > l) ? (float) be :  be) << format(spaced_scol_separator, Style::BOLD);
	stream << setprecision(( fe > l) ? 2 : 0) << ((fe  > l) ? scientific : fixed) << setw(column_width-1) << (( fe > l) ? (float) fe :  fe) << format(spaced_scol_separator, Style::BOLD);
	stream <<                                                                                                                 str_ber.str() << format(spaced_scol_separator, Style::BOLD);
	stream <<                                                                                                                 str_fer.str() << format(spaced_dcol_separator, Style::BOLD);
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

	stream << format(spaced_scol_separator, Style::BOLD) << std::setprecision(0) << std::fixed << std::setw(column_width-1) << tr_format;

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

	stream << format(spaced_scol_separator, Style::BOLD) << std::setprecision(0) << std::fixed << std::setw(column_width-1) << et_format;

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
