#ifndef TERMINAL_EXIT_HPP_
#define TERMINAL_EXIT_HPP_

#include <string>
#include <chrono>

#include "../Terminal.hpp"

template <typename B, typename R>
class Terminal_EXIT : public Terminal
{
protected:
	const int                                                                           N;
	const R                                                                             snr;
	const R                                                                             sig_a;
	const std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> &t_snr;
	const int                                                                          &cur_t;
	const int                                                                          &trials;
	double                                                                             &I_A;
	double                                                                             &I_E;
	unsigned short                                                                      real_time_state;

public:
	Terminal_EXIT(const int& N,
	              const R& snr,
	              const R& sig_a,
	              const std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> &t_snr,
	              const int& cur_t,
	              const int& trials,
	              double& I_A,
	              double& I_E,
	              const std::string name = "Terminal_EXIT");

	virtual ~Terminal_EXIT() {}

	void legend      (std::ostream &stream = std::cout);
	void temp_report (std::ostream &stream = std::cout);
	void final_report(std::ostream &stream = std::cout);

protected:
	static std::string get_time_format(float secondes);
	void _report(std::ostream &stream);
};

#endif /* TERMINAL_EXIT_HPP_ */
