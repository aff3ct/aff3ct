#ifndef TERMINAL_BFER_HPP_
#define TERMINAL_BFER_HPP_

#include <string>
#include <chrono>

#include "../../Error/Error_analyzer.hpp"

#include "../Terminal.hpp"

template <typename B, typename R>
class Terminal_BFER : public Terminal
{
protected:
	const R                                                                             snr;
	const Error_analyzer<B>                                                            &err_analyzer;
	const std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> &t_snr;
	const std::chrono::nanoseconds                                                     &d_decod_total;
	unsigned short                                                                      real_time_state;
	const bool                                                                          use_only_decoder_time_thr;

public:
	Terminal_BFER(const R& snr,
	              const Error_analyzer<B> &err_analyzer,
	              const std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> &t_snr,
	              const std::chrono::nanoseconds &d_decod_total,
	              const bool use_only_decoder_time_thr = false);
	virtual ~Terminal_BFER() {}

	void legend      (std::ostream &stream = std::cout);
	void temp_report (std::ostream &stream = std::cout);
	void final_report(std::ostream &stream = std::cout);

protected:
	static std::string get_time_format(float secondes);
	void _report(std::ostream &stream);
};

#endif /* TERMINAL_BFER_HPP_ */
