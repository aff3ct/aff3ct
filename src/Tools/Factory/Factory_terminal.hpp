#ifndef FACTORY_TERMINAL_HPP
#define FACTORY_TERMINAL_HPP

#include "Tools/params.h"

#include "Terminal/Terminal.hpp"
#include "Module/Error/Error_analyzer.hpp"

template <typename B, typename R>
struct Factory_terminal
{
	static Terminal* build(const parameters &params,
	                       const R snr, 
	                       const Error_analyzer<B> *analyzer, 
	                       const std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> &t_snr,
	                       const std::chrono::nanoseconds &d_decod_total);
};

#endif /* FACTORY_TERMINAL_HPP */
