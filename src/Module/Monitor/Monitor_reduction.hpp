#ifndef MONITOR_REDUCTION_HPP_
#define MONITOR_REDUCTION_HPP_

#include <thread>
#include <string>
#include <vector>
#include <cassert>

#include "Monitor.hpp"

namespace aff3ct
{
namespace module
{

class Monitor_reduction
{
public:
	static void reduce(bool fully = false, bool force = false);

	static void set_master_thread_id(std::thread::id          t);
	static void set_reduce_frequency(std::chrono::nanoseconds d);

protected:
	Monitor_reduction();
	virtual ~Monitor_reduction() = default;

	virtual void _reduce(bool fully = false) = 0;

private:
	static void add_monitor(Monitor_reduction*);
	static void reset();

	static std::vector<Monitor_reduction*> monitors;
	static std::thread::id                 master_thread_id;
	static std::chrono::nanoseconds        d_reduce_frequency;

	static std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_last_mpi_comm;
};


template <class M> // M is the monitor on which must be applied the reduction
class Monitor_reduction_M : public Monitor_reduction, public M
{
	static_assert(std::is_base_of<Monitor, M>::value, "M shall be based on a module::Monitor class.");

private:
	std::vector<M*> monitors;

public:
	explicit Monitor_reduction_M(const std::vector<M*> &monitors);
	virtual ~Monitor_reduction_M() = default;

	virtual void reset();
	virtual void clear_callbacks();

protected:
	virtual void _reduce(bool fully = false);
};
}
}

#include "Monitor_reduction.hxx"

#endif /* MONITOR_REDUCTION_HPP_ */
