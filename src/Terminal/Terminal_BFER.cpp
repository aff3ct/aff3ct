#include <iostream>
#include <iomanip>
#include <sstream>

#include "../Tools/bash_tools.h"

#include "Terminal_BFER.hpp"

template <typename B, typename R>
Terminal_BFER<B,R>
::Terminal_BFER(const R& snr,
                const Error_analyzer<B> &err_analyzer,
                const std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> &t_snr,
                const std::chrono::nanoseconds &d_decod_total,
                const std::chrono::nanoseconds &d_store_total,
                const bool use_only_decoder_time_thr,
                const std::string name)
: Terminal(name.c_str()),
  snr(snr),
  err_analyzer(err_analyzer),
  t_snr(t_snr),
  d_decod_total(d_decod_total),
  real_time_state(0),
  use_only_decoder_time_thr(use_only_decoder_time_thr)
{
}

template <typename B, typename R>
std::string Terminal_BFER<B,R>
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
void Terminal_BFER<B,R>
::legend(std::ostream &stream)
{
#ifdef _WIN32
	stream << "# " << "------------------------------------------------------------------||---------------------------------||---------------------" << std::endl;
	stream << "# " << "    Bit Error Rate (BER) and Frame Error Rate (FER) depending     ||     Decoder throughput and      ||  Global throughput  " << std::endl;
	stream << "# " << "              on the Signal Noise Ratio (SNR) Eb/N0               ||      latency (per thread)       ||  and elapsed time   " << std::endl;
	stream << "# " << "------------------------------------------------------------------||---------------------------------||---------------------" << std::endl;
	stream << "# " << "------|-----------|-----------|-----------|-----------|-----------||----------|----------|-----------||----------|----------" << std::endl;
	stream << "# " << "  SNR |       FRA |        BE |        FE |       BER |       FER ||     CTHR |    ITHR  |   LATENCY || SIM_CTHR |    ET/RT " << std::endl;
	stream << "# " << " (dB) |           |           |           |           |           ||   (Mb/s) |  (Mb/s)  |      (us) ||   (Mb/s) | (hhmmss) " << std::endl;
	stream << "# " << "------|-----------|-----------|-----------|-----------|-----------||----------|----------|-----------||----------|----------" << std::endl;
#else
	stream << "# " << "-------------------------------------------------------------||--------------------------------||---------------------" << std::endl;
	stream << "# " << "  Bit Error Rate (BER) and Frame Error Rate (FER) depending  ||     Decoder throughput and     ||  Global throughput  " << std::endl;
	stream << "# " << "            on the Signal Noise Ratio (SNR) Eb/N0            ||      latency (per thread)      ||  and elapsed time   " << std::endl;
	stream << "# " << "-------------------------------------------------------------||--------------------------------||---------------------" << std::endl;
	stream << "# " << "------|----------|----------|----------|----------|----------||----------|----------|----------||----------|----------" << std::endl;
	stream << "# " << "  SNR |      FRA |       BE |       FE |      BER |      FER ||     CTHR |     ITHR |  LATENCY || SIM_CTHR |    ET/RT " << std::endl;
	stream << "# " << " (dB) |          |          |          |          |          ||   (Mb/s) |   (Mb/s) |     (us) ||   (Mb/s) | (hhmmss) " << std::endl;
	stream << "# " << "------|----------|----------|----------|----------|----------||----------|----------|----------||----------|----------" << std::endl;
#endif
}

template <typename B, typename R>
void Terminal_BFER<B,R>
::_report(std::ostream &stream)
{
	using namespace std::chrono;
	using namespace std;

	auto ber = 0.f, fer = 0.f;
	if(err_analyzer.get_n_be() != 0 )
	{
		ber = (float)err_analyzer.get_ber_value();
		fer = (float)err_analyzer.get_fer_value();
	}
	else
	{
		ber = (1.f) / ((float)err_analyzer.get_n_analyzed_frames()) / err_analyzer.get_K();
		fer = (1.f) / ((float)err_analyzer.get_n_analyzed_frames());
	}
	auto fra = err_analyzer.get_n_analyzed_frames();
	auto be  = err_analyzer.get_n_be();
	auto fe  = err_analyzer.get_n_fe();

	auto decod_time_ms = (float)d_decod_total.count() * 0.000001f;
	auto total_time = d_decod_total.count();

	auto dec_cthr = ((float)err_analyzer.get_N() * (float)err_analyzer.get_n_analyzed_frames()) /
		             (total_time * 0.000000001f); // = bps
	dec_cthr /= 1000.f; // = kbps
	dec_cthr /= 1000.f; // = mbps

	auto dec_ithr = dec_cthr * ((float)err_analyzer.get_K() / (float)err_analyzer.get_N());

	auto simu_time = (float)duration_cast<nanoseconds>(steady_clock::now() - t_snr).count() * 0.000000001f;
	auto simu_cthr = ((float)err_analyzer.get_N() * (float)err_analyzer.get_n_analyzed_frames()) /
		              simu_time ; // = bps
	simu_cthr /= 1000.f; // = kbps
	simu_cthr /= 1000.f; // = mbps

	auto lat = decod_time_ms * 1000.f;
	lat = (lat / (float) err_analyzer.get_n_analyzed_frames()) * err_analyzer.get_n_frames();

	if (Error_analyzer<B>::is_interrupt()) stream << "\r";

#ifdef _WIN32
	stringstream str_ber, str_fer;
	str_ber << setprecision(2) << scientific << setw(9) << ber;
	str_fer << setprecision(2) << scientific << setw(9) << fer;

	unsigned long long l0 = 99999999;  // limit 0
	auto               l1 = 99999999;  // limit 1
	auto               l2 = 99999.99f; // limit 2
	stream << "  ";
	stream << setprecision(                 2) <<                            fixed  << setw(5) <<                             snr << " | ";
	stream << setprecision((fra > l0) ? 2 : 0) << ((fra > l0) ? scientific : fixed) << setw(9) << ((fra > l0) ? (float)fra : fra) << " | ";
	stream << setprecision(( be > l1) ? 2 : 0) << ((be  > l1) ? scientific : fixed) << setw(9) << (( be > l1) ? (float) be :  be) << " | ";
	stream << setprecision(( fe > l1) ? 2 : 0) << ((fe  > l1) ? scientific : fixed) << setw(9) << (( fe > l1) ? (float) fe :  fe) << " | ";
	stream <<                                                                                                       str_ber.str() << " | ";
	stream <<                                                                                                       str_fer.str() << " || ";
	stream << setprecision(                 2) <<                            fixed  << setw(8) <<                        dec_cthr << " | ";
	stream << setprecision(                 2) <<                            fixed  << setw(8) <<                        dec_ithr << " | ";
	stream << setprecision(                 2) << ((lat > l2) ? scientific : fixed) << setw(9) <<                             lat << " || ";
	stream << setprecision(                 2) <<                            fixed  << setw(8) <<                       simu_cthr;
#else
	stringstream str_ber, str_fer;
	str_ber << setprecision(2) << scientific << setw(8) << ber;
	str_fer << setprecision(2) << scientific << setw(8) << fer;

	unsigned long long l0 = 99999999;  // limit 0
	auto               l1 = 99999999;  // limit 1
	auto               l2 = 99999.99f; // limit 2
	stream << "  ";
	stream << setprecision(                 2) <<                            fixed  << setw(5) <<                             snr << " | ";
	stream << setprecision((fra > l0) ? 2 : 0) << ((fra > l0) ? scientific : fixed) << setw(8) << ((fra > l0) ? (float)fra : fra) << " | ";
	stream << setprecision(( be > l1) ? 2 : 0) << ((be  > l1) ? scientific : fixed) << setw(8) << (( be > l1) ? (float) be :  be) << " | ";
	stream << setprecision(( fe > l1) ? 2 : 0) << ((fe  > l1) ? scientific : fixed) << setw(8) << (( fe > l1) ? (float) fe :  fe) << " | ";
	stream <<                                                                                                       str_ber.str() << " | ";
	stream <<                                                                                                       str_fer.str() << " || ";
	stream << setprecision(                 2) <<                            fixed  << setw(8) <<                        dec_cthr << " | ";
	stream << setprecision(                 2) <<                            fixed  << setw(8) <<                        dec_ithr << " | ";
	stream << setprecision(                 2) << ((lat > l2) ? scientific : fixed) << setw(8) <<                             lat << " || ";
	stream << setprecision(                 2) <<                            fixed  << setw(8) <<                       simu_cthr;
#endif
}

template <typename B, typename R>
void Terminal_BFER<B,R>
::temp_report(std::ostream &stream)
{
	using namespace std::chrono;

	_report(stream);

	auto et = duration_cast<milliseconds>(steady_clock::now() - t_snr).count() / 1000.f;
	auto tr = et * ((float)err_analyzer.get_fe_limit() / (float)err_analyzer.get_n_fe()) - et;
	auto tr_format = get_time_format((err_analyzer.get_n_fe() == 0) ? 0 : tr);

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
void Terminal_BFER<B,R>
::final_report(std::ostream &stream)
{
	using namespace std::chrono;

	this->_report(stream);

	auto et = duration_cast<milliseconds>(steady_clock::now() - t_snr).count() / 1000.f;
	auto et_format = get_time_format(et);

	stream << " | " << std::setprecision(0) << std::fixed << std::setw(8) << et_format;

	if (Error_analyzer<B>::is_interrupt()) stream << " x" << std::endl;
	else                                   stream << "  " << std::endl;
}

// ==================================================================================== explicit template instantiation 
#include "../Tools/types.h"
#ifdef MULTI_PREC
template class Terminal_BFER<B_8,R_8>;
template class Terminal_BFER<B_16,R_16>;
template class Terminal_BFER<B_32,R_32>;
template class Terminal_BFER<B_64,R_64>;
#else
template class Terminal_BFER<B,R>;
#endif
// ==================================================================================== explicit template instantiation