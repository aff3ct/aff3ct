#ifndef FACTORY_TERMINAL_HPP
#define FACTORY_TERMINAL_HPP

#include "Module/Monitor/Monitor.hpp"
#include "Tools/params.h"
#include "Tools/Display/Terminal/Terminal.hpp"

#include "Factory.hpp"

template <typename B, typename R>
struct Factory_terminal : public Factory
{
	static Terminal* build(const parameters &params,
	                       const R snr_s,
	                       const R snr_b,
	                       const Monitor<B> *monitor,
	                       const std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> &t_snr,
	                       const std::chrono::nanoseconds *d_decod_total = nullptr);
};

#endif /* FACTORY_TERMINAL_HPP */
