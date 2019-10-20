/*!
 * \file
 * \brief Class tools::Reporter_monitor.
 */
#ifndef REPORTER_MONITOR_HPP_
#define REPORTER_MONITOR_HPP_

#include <vector>
#include <utility>
#include <string>

#include "Tools/Display/Reporter/Reporter.hpp"

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
	explicit Reporter_monitor(const M &monitor);
	virtual ~Reporter_monitor() = default;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Display/Reporter/Reporter_monitor.hxx"
#endif

#endif /* REPORTER_MONITOR_HPP_ */
