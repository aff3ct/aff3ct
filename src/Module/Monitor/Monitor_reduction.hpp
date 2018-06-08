#ifndef MONITOR_REDUCTION_HPP_
#define MONITOR_REDUCTION_HPP_

#include <string>
#include <vector>
#include <cassert>

#include "Monitor.hpp"

namespace aff3ct
{
namespace module
{
template <class M> // M is the monitor on which must be applied the reduction
class Monitor_reduction : public M
{
	static_assert(std::is_base_of<Monitor, M>::value, "M shall be based on a module::Monitor class.");

private:
	std::vector<M*> monitors;

public:
	explicit Monitor_reduction(const std::vector<M*> &monitors);
	virtual ~Monitor_reduction() = default;

	virtual void reset();
	virtual void clear_callbacks();
	virtual void reduce(bool fully = false);
};
}
}

#include "Monitor_reduction.hxx"

#endif /* MONITOR_REDUCTION_HPP_ */
