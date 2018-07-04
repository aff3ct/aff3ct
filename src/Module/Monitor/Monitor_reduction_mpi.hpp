#ifdef ENABLE_MPI

#ifndef MONITOR_REDUCTION_MPI_HPP_
#define MONITOR_REDUCTION_MPI_HPP_

#include <thread>
#include <chrono>
#include <vector>

#include <mpi.h>

#include "Monitor_reduction.hpp"

namespace aff3ct
{
namespace module
{
template <class M> // M is the monitor on which must be applied the reduction
class Monitor_reduction_mpi : public Monitor_reduction<M>
{
private:
	const std::thread::id          master_thread_id;
	const std::chrono::nanoseconds d_mpi_comm_frequency;

	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_last_mpi_comm;

	MPI_Datatype MPI_monitor_vals;
	MPI_Op       MPI_SUM_monitor_vals;

public:
	Monitor_reduction_mpi(const std::vector<M*> &monitors,
	                      const std::thread::id master_thread_id,
	                      const std::chrono::nanoseconds d_mpi_comm_frequency = std::chrono::milliseconds(1000));
	virtual ~Monitor_reduction_mpi() = default;

	void reset ();
	void reduce(bool fully = false);
};
}
}
#include "Monitor_reduction_mpi.hxx"

#endif /* MONITOR_REDUCTION_MPI_HPP_ */

#endif
