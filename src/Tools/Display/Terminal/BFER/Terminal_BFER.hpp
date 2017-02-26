#ifndef TERMINAL_BFER_HPP_
#define TERMINAL_BFER_HPP_

#include <string>
#include <chrono>

#include "Module/Monitor/Monitor.hpp"
#include "../Terminal.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
class Terminal_BFER : public Terminal
{
protected:
	const R                                                                             snr_s;
	const R                                                                             snr_b;
	const module::Monitor<B,R>                                                         &monitor;
	const std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> &t_snr;
	const std::chrono::nanoseconds                                                     *d_decod_total;
	unsigned short                                                                      real_time_state;

public:
	Terminal_BFER(const R& snr_s,
	              const R& snr_b,
	              const module::Monitor<B,R> &monitor,
	              const std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> &t_snr,
	              const std::chrono::nanoseconds *d_decod_total = nullptr);

	virtual ~Terminal_BFER() {}

	void legend      (std::ostream &stream = std::cout);
	void temp_report (std::ostream &stream = std::cout);
	void final_report(std::ostream &stream = std::cout);

protected:
	static std::string get_time_format(float secondes);
	void _report(std::ostream &stream);
};
}
}

#endif /* TERMINAL_BFER_HPP_ */
