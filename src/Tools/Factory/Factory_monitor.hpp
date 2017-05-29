#ifndef FACTORY_MONITOR_HPP
#define FACTORY_MONITOR_HPP

#include <string>

#include "Module/Monitor/Monitor.hpp"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_monitor : public Factory
{
	static module::Monitor<B>* build(const std::string type,
	                                 const int         K,
	                                 const int         fe       = 100,
	                                 const int         n_frames = 0);
};
}
}

#endif /* FACTORY_MONITOR_HPP */
