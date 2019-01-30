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
private:
	static bool                            stop_loop;
	static std::vector<Monitor_reduction*> monitors;
	static std::thread::id                 master_thread_id;
	static std::chrono::nanoseconds        d_reduce_frequency;

	static std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_last_reduction;

public:
	/*
	 * \brief check if any recorded monitor reduction has done after having done a reduction
	 *        if any monitor is done then call 'set_stop_loop()'
	 * \param final is true then call 'last_reduce_all()' else 'reduce_all()'
	 * \param fully call the reduction functions with this parameter
	 * \return 'get_stop_loop()' result
	 */
	static bool is_done_all(bool fully = false, bool final = false);

	/*
	 * \brief call __reduce__ with 'fully' and 'force' arguments
	 */
	static void reduce_all(bool fully = false, bool force = false);

	/*
	 * \brief loop on a forced reduction until '__reduce__' call return 'true' after having call 'set_stop_loop()'
	 */
	static void last_reduce_all(bool fully = false);

	/*
	 * reset 't_last_mpi_comm', clear 'stop_loop' and call 'reset_mr' on each monitor
	 */
	static void reset_all();

	static void set_master_thread_id(std::thread::id          t);

	static void set_reduce_frequency(std::chrono::nanoseconds d);

	/*
	 * \brief get if the current simulation loop must be stopped or not
	 * \return true if loop must be stopped
	 */
	static bool get_stop_loop();

	/*
	 * \brief set that the current simulation loop must be stopped
	 */
	static void set_stop_loop();

	/*
	 * \brief throw if all process do not have the same number of monitors to reduce
	 */
	static void check_reducible();

protected:
	Monitor_reduction();

	virtual ~Monitor_reduction() = default;

	/*
	 * \brief do the reduction of this monitor
	 */
	virtual void _reduce(bool fully = false) = 0;

	/*
	 * \brief reset this monitor
	 */
	virtual void reset_mr() = 0;

	/*
	 * \brief check if this monitor has done
	 * \return true if has done
	 */
	virtual bool is_done_mr() = 0;

private:
	/*
	 * \brief add the monitor in the 'monitors' list
	 */
	static void add_monitor(Monitor_reduction*);

	/*
	 * \brief do a reduction of the number of process that are at the final reduce step
	 * \return true if all process are at the final reduce step (always true without MPI)
	 */
	static bool reduce_stop_loop();

	/*
	 * \brief do the reductions of all 'monitors' if the thread calling it is the master thread and if the
	 *        'd_reduce_frequency' criteria is reached.
	 * \param force if set, do the reduction anyway
	 * \param fully if set, do a full reduction of all attributes
	 * \return the result of the 'reduce_stop_loop()' call after the reductions. If there were not, then return false.
	 */
	static bool __reduce__(bool fully, bool force);
};


template <class M> // M is the monitor on which must be applied the reduction
class Monitor_reduction_M : public Monitor_reduction, public M
{
	static_assert(std::is_base_of<Monitor, M>::value, "M have to be based on a module::Monitor class.");

private:
	const std::vector<std::unique_ptr<M>>& monitors;
	M collecter;

public:
	/*
	 * \brief do reductions upon a monitor list to merge data in this monitor
	 * \param monitors is the list of monitors on which the reductions are done
	 */
	explicit Monitor_reduction_M(const std::vector<std::unique_ptr<M>> &monitors);
	virtual ~Monitor_reduction_M() = default;

	virtual void reset();
	virtual void clear_callbacks();

protected:
	virtual void _reduce(bool fully = false);

	/*
	 * \brief call reset()
	 */
	virtual void reset_mr();

	/*
	 * \brief call is_done()
	 */
	virtual bool is_done_mr();
};
}
}

#include "Monitor_reduction.hxx"

#endif /* MONITOR_REDUCTION_HPP_ */
