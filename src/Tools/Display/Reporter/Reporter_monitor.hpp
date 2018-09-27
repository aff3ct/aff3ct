#ifndef REPORTER_MONITOR_HPP_
#define REPORTER_MONITOR_HPP_

#include <string>
#include <chrono>

#include "Module/Monitor/Monitor_reduction.hpp"

#include "Reporter.hpp"

namespace aff3ct
{
namespace tools
{
template <class M_t>
class Reporter_monitor : public Reporter
{
public:
	using M = M_t;
protected:
	const M& monitor;

	group_t monitor_group;

public:

	explicit Reporter_monitor(const M &monitor)
	: Reporter   (       ),
	  monitor    (monitor)
	 {}

	virtual ~Reporter_monitor() = default;
};
}
}

#endif /* REPORTER_MONITOR_HPP_ */
