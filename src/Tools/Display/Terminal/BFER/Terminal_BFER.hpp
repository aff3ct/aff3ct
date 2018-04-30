#ifndef TERMINAL_BFER_HPP_
#define TERMINAL_BFER_HPP_

#include <string>
#include <chrono>

#include "Module/Monitor/BFER/Monitor_BFER.hpp"
#include "Tools/Noise/Noise.hpp"

#include "../Terminal.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
class Terminal_BFER : public Terminal
{
protected:
	const module::Monitor_BFER<B,R> &monitor;
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_snr;
	unsigned short real_time_state;
	Noise<float>* n;
	const bool display_mutinfo;

public:
	explicit Terminal_BFER(const module::Monitor_BFER<B,R> &monitor, bool display_mutinfo = false);

	virtual ~Terminal_BFER() = default;

	void set_noise(const Noise<float>& noise);
	void set_noise(const Noise<double>& noise);

	void legend      (std::ostream &stream = std::cout);
	void temp_report (std::ostream &stream = std::cout);
	void final_report(std::ostream &stream = std::cout);

protected:
	void _report(std::ostream &stream);
};
}
}

#endif /* TERMINAL_BFER_HPP_ */
