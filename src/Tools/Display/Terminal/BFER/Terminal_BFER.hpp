#ifndef TERMINAL_BFER_HPP_
#define TERMINAL_BFER_HPP_

#include <string>
#include <chrono>

#include "Module/Monitor/BFER/Monitor_BFER.hpp"

#include "../Terminal.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
class Terminal_BFER : public Terminal
{
protected:
	const module::Monitor_BFER<B>                                                      &monitor;
	      float                                                                         esn0;
	      float                                                                         ebn0;
	      bool                                                                          is_esn0;
	      bool                                                                          is_ebn0;
	      std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>  t_snr;
	unsigned short                                                                      real_time_state;

public:
	Terminal_BFER(const module::Monitor_BFER<B> &monitor);

	virtual ~Terminal_BFER() {}

	void set_esn0(const float esn0);
	void set_ebn0(const float ebn0);

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
