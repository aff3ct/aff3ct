#ifndef FACTORY_TERMINAL_HPP
#define FACTORY_TERMINAL_HPP

#include <chrono>
#include <string>

#include "Module/Monitor/Monitor.hpp"
#include "Tools/Display/Terminal/Terminal.hpp"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_terminal : public Factory
{
	static Terminal* build(const std::string                                                                   type,
	                       const int                                                                           K,
	                       const int                                                                           N,
	                       const float                                                                        &snr_s,
	                       const float                                                                        &snr_b,
	                       const module::Monitor<B>                                                           &monitor,
	                       const std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> &t_snr,
	                       const std::chrono::nanoseconds                                                     *d_decod_total = nullptr);
};
}
}

#endif /* FACTORY_TERMINAL_HPP */
