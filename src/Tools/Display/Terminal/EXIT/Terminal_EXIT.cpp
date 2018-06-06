// #include <iostream>
// #include <iomanip>
// #include <sstream>
// #include <ios>

// #include <rang.hpp>

// #include "Terminal_EXIT.hpp"
// #include "Tools/Noise/noise_utils.h"

// using namespace aff3ct;
// using namespace aff3ct::tools;

// template <typename B, typename R>
// Terminal_EXIT<B,R>
// ::Terminal_EXIT(const module::Monitor_EXIT<B,R> &monitor)
// : Terminal(),
//   monitor(monitor),
//   t_snr(std::chrono::steady_clock::now()),
//   real_time_state(0),
//   sig_a(0.f),
//   n(nullptr)
// {
// }

// template<typename B, typename R>
// Terminal_EXIT<B,R>::~Terminal_EXIT()
// {
// 	if (this->n != nullptr)
// 		delete this->n;
// }

// template <typename B, typename R>
// void Terminal_EXIT<B,R>
// ::set_noise(const Noise<float>& noise)
// {
// 	if (this->n != nullptr)
// 		delete this->n;

// 	this->n = tools::cast<float>(noise);
// }

// template <typename B, typename R>
// void Terminal_EXIT<B,R>
// ::set_noise(const Noise<double>& noise)
// {
// 	if (this->n != nullptr)
// 		delete this->n;

// 	this->n = tools::cast<float>(noise);
// }

// template <typename B, typename R>
// void Terminal_EXIT<B,R>
// ::set_sig_a(float sig_a)
// {
// 	this->sig_a = sig_a;
// }

// template <typename B, typename R>
// void Terminal_EXIT<B,R>
// ::legend(std::ostream &stream)
// {
// 	this->cols_groups.resize(2);

// 	auto& bfer_title       = this->cols_groups[0].first;
// 	auto& bfer_cols        = this->cols_groups[0].second;
// 	auto& throughput_title = this->cols_groups[1].first;
// 	auto& throughput_cols  = this->cols_groups[1].second;

// 	bfer_title = std::make_pair("EXIT chart depending on the", "");
// 	bfer_cols.clear();

// 	if (this->n == nullptr)
// 	{
// 		std::stringstream message;
// 		message << "Undefined noise.";
// 		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
// 	}

// 	switch (this->n->get_type())
// 	{
// 		case Noise_type::SIGMA :
// 			bfer_title.second = "Signal Noise Ratio (SNR)";
// 			bfer_cols.push_back(std::make_pair("Es/N0", "(dB)"));
// 			bfer_cols.push_back(std::make_pair("Eb/N0", "(dB)"));
// 		break;
// 		case Noise_type::ROP :
// 			bfer_title.second = "Received Optical Power (ROP)";
// 			bfer_cols.push_back(std::make_pair("ROP", "(dB)"));
// 		break;
// 		case Noise_type::EP :
// 			bfer_title.second = "Erasure Probability (EP)";
// 			bfer_cols.push_back(std::make_pair("EP", ""));
// 		break;
// 	}

// 	bfer_title.second += " and the channel A noise";
// 	throughput_cols.clear();
// 	bfer_cols.push_back(std::make_pair("SIG_A",      "(dB)"));
// 	bfer_cols.push_back(std::make_pair("FRA",            ""));
// 	bfer_cols.push_back(std::make_pair("A_PRIORI",  "(I_A)"));
// 	bfer_cols.push_back(std::make_pair("EXTRINSIC", "(I_E)"));

// 	throughput_title = std::make_pair("Global throughput", "and elapsed time");
// 	throughput_cols.push_back(std::make_pair("SIM_THR", "(Mb/s)"));
// 	throughput_cols.push_back(std::make_pair("ET/RT", "(hhmmss)"));


// 	// stream << "# " << rang::style::bold << "----------------------------------------------------------||---------------------" << rang::style::reset << std::endl;
// 	// stream << "# " << rang::style::bold << "               EXIT chart depending on the                ||  Global throughput  " << rang::style::reset << std::endl;
// 	// stream << "# " << rang::style::bold << "     Signal Noise Ratio (SNR) and the channel A noise     ||  and elapsed time   " << rang::style::reset << std::endl;
// 	// stream << "# " << rang::style::bold << "----------------------------------------------------------||---------------------" << rang::style::reset << std::endl;
// 	// stream << "# " << rang::style::bold << "-------|-------|-------|----------|-----------|-----------||----------|----------" << rang::style::reset << std::endl;
// 	// stream << "# " << rang::style::bold << " Es/N0 | Eb/N0 | SIG_A |      FRA |  A_PRIORI | EXTRINSIC ||  SIM_THR |    ET/RT " << rang::style::reset << std::endl;
// 	// stream << "# " << rang::style::bold << "  (dB) |  (dB) |  (dB) |          |     (I_A) |     (I_E) ||   (Mb/s) | (hhmmss) " << rang::style::reset << std::endl;
// 	// stream << "# " << rang::style::bold << "-------|-------|-------|----------|-----------|-----------||----------|----------" << rang::style::reset << std::endl;

// 	Terminal::legend(stream); // print effectively the legend
// }

// template <typename B, typename R>
// void Terminal_EXIT<B,R>
// ::_report(std::ostream &stream)
// {
// 	using namespace std::chrono;
// 	using namespace std;

// 	const auto size = monitor.get_K();
// 	const auto fra  = monitor.get_n_analyzed_fra();
// 	const auto I_A  = monitor.get_I_A();
// 	const auto I_E  = monitor.get_I_E();

// 	auto simu_time = (float)duration_cast<nanoseconds>(steady_clock::now() - t_snr).count() * 0.000000001f;
// 	auto simu_cthr = (size * fra) / simu_time ; // = bps
// 	simu_cthr /= 1000.f; // = kbps
// 	simu_cthr /= 1000.f; // = mbps

// 	stream << data_tag;

// 	if (this->n == nullptr)
// 	{
// 		std::stringstream message;
// 		message << "Undefined noise.";
// 		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
// 	}

// 	switch (this->n->get_type())
// 	{
// 		case Noise_type::SIGMA :
// 		{
// 			auto sig = dynamic_cast<const tools::Sigma<>*>(this->n);
// 			stream << setprecision(2) << fixed << setw(column_width-1) << sig->get_esn0() << report_style
// 			       << spaced_scol_separator << rang::style::reset;
// 			stream << setprecision(2) << fixed << setw(column_width-1) << sig->get_ebn0() << report_style
// 			       << spaced_scol_separator << rang::style::reset;
// 			break;
// 		}
// 		case Noise_type::ROP :
// 		{
// 			stream << setprecision(2) << fixed << setw(column_width-1) << this->n->get_noise() << report_style
// 			       << spaced_scol_separator << rang::style::reset;
// 			break;
// 		}
// 		case Noise_type::EP :
// 		{
// 			stream << setprecision(4) << fixed << setw(column_width-1) << this->n->get_noise() << report_style
// 			       << spaced_scol_separator << rang::style::reset;
// 			break;
// 		}
// 	}

// 	stream << setprecision(2) << fixed << setw(column_width-1) << sig_a << rang::style::bold << spaced_scol_separator << rang::style::reset;
// 	stream << setprecision(2) << fixed << setw(column_width-1) << fra   << rang::style::bold << spaced_scol_separator << rang::style::reset;
// 	stream << setprecision(6) << fixed << setw(column_width-1) << I_A   << rang::style::bold << spaced_scol_separator << rang::style::reset;
// 	stream << setprecision(6) << fixed << setw(column_width-1) << I_E   << rang::style::bold << spaced_dcol_separator << rang::style::reset;
// 	stream << setprecision(2) << fixed << setw(column_width-1) << simu_cthr;
// }

// template <typename B, typename R>
// void Terminal_EXIT<B,R>
// ::temp_report(std::ostream &stream)
// {
// 	using namespace std::chrono;

// 	std::ios::fmtflags f(stream.flags());

// 	_report(stream);

// 	const auto n_trials = monitor.get_n_trials();
// 	const auto cur_fra = monitor.get_n_analyzed_fra();

// 	auto et = duration_cast<milliseconds>(steady_clock::now() - t_snr).count() / 1000.f;
// 	auto tr = et * (n_trials / cur_fra) - et;
// 	auto tr_format = get_time_format((cur_fra == 0) ? 0 : tr);

// 	stream << rang::style::bold << " | " << rang::style::reset << std::setprecision(0) << std::fixed << std::setw(8)
// 	       << tr_format << " ";

// 	switch (real_time_state)
// 	{
// 		case 0: stream << rang::style::bold << rang::fg::green << "*" << rang::style::reset; break;
// 		case 1: stream << rang::style::bold << rang::fg::green << "*" << rang::style::reset; break;
// 		case 2: stream << rang::style::bold << rang::fg::green << " " << rang::style::reset; break;
// 		case 3: stream << rang::style::bold << rang::fg::green << " " << rang::style::reset; break;
// 		default: break;
// 	}
// 	real_time_state = (real_time_state + (unsigned short)1) % (unsigned short)4;
// 	stream << "\r";

// 	stream.flush();
// 	stream.flags(f);
// }

// template <typename B, typename R>
// void Terminal_EXIT<B,R>
// ::final_report(std::ostream &stream)
// {
// 	using namespace std::chrono;

// 	std::ios::fmtflags f(stream.flags());

// 	Terminal::final_report(stream);

// 	this->_report(stream);

// 	auto et = duration_cast<milliseconds>(steady_clock::now() - t_snr).count() / 1000.f;
// 	auto et_format = get_time_format(et);

// 	stream << rang::style::bold << " | " << rang::style::reset << std::setprecision(0) << std::fixed << std::setw(8)
// 	       << et_format << "  " << std::endl;

// 	t_snr = std::chrono::steady_clock::now();

// 	stream.flags(f);
// }

// // ==================================================================================== explicit template instantiation
// #include "Tools/types.h"
// #ifdef MULTI_PREC
// template class aff3ct::tools::Terminal_EXIT<B_32,R_32>;
// template class aff3ct::tools::Terminal_EXIT<B_64,R_64>;
// #else
// template class aff3ct::tools::Terminal_EXIT<B,R>;
// #endif
// // ==================================================================================== explicit template instantiation
