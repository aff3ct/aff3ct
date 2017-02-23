#ifndef FACTORY_MONITOR_HPP
#define FACTORY_MONITOR_HPP

#include "Module/Monitor/Monitor.hpp"
#include "Tools/params.h"

#include "Factory.hpp"

template <typename B, typename R>
struct Factory_monitor : public Factory
{
	static Monitor<B,R>* build(const parameters &params, const int n_frames = 1);
};

#endif /* FACTORY_MONITOR_HPP */
