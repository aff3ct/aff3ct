#ifndef TERMINAL_EXIT_HPP_
#define TERMINAL_EXIT_HPP_

#include <string>
#include <chrono>
#include <iostream>

#include "../Terminal.hpp"

namespace aff3ct
{
namespace tools
{
class Terminal_EXIT : public Terminal
{
protected:
	const int                                                                           N;
	const double                                                                        snr;
	const double                                                                        sig_a;
	      std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>  t_snr;
	const int                                                                          &cur_t;
	const int                                                                          &trials;
	const double                                                                       &I_A;
	const double                                                                       &I_E;
	unsigned short                                                                      real_time_state;

public:
	Terminal_EXIT(const int     N,
	              const double  snr,
	              const double  sig_a,
	              const int    &cur_t,
	              const int    &trials,
	              const double &I_A,
	              const double &I_E);

	virtual ~Terminal_EXIT() {}

	void legend      (std::ostream &stream = std::cout);
	void temp_report (std::ostream &stream = std::cout);
	void final_report(std::ostream &stream = std::cout);

protected:
	static std::string get_time_format(float secondes);
	void _report(std::ostream &stream);
};
}
}

#endif /* TERMINAL_EXIT_HPP_ */
