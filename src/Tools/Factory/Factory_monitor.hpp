#ifndef FACTORY_MONITOR_HPP
#define FACTORY_MONITOR_HPP

#include "Module/Monitor/Monitor.hpp"
#include "Tools/params.h"

#include "Factory.hpp"

namespace aff3ct
{
template <typename B>
struct Factory_monitor : public Factory
{
	static Monitor<B>* build(const parameters &params, const int n_frames = 1);
};
}

#endif /* FACTORY_MONITOR_HPP */
