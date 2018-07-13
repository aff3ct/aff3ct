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

class Monitor_mpi
{
public:
	static void reduce(bool fully = false);

	static void set_master_thread_id  (std::thread::id         );
	static void set_mpi_comm_frequency(std::chrono::nanoseconds);

protected:
	Monitor_mpi();

	virtual void _reduce(bool fully = false) = 0;

private:
	static void add_monitor(Monitor_mpi*);
	static void reset();

	static std::vector<Monitor_mpi*> monitors;
	static std::thread::id           master_thread_id;
	static std::chrono::nanoseconds  d_mpi_comm_frequency;

	static std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_last_mpi_comm;
};


template <class M> // M is the monitor on which must be applied the reduction
class Monitor_reduction_mpi : public Monitor_mpi, public Monitor_reduction<M>
{
private:
	MPI_Datatype MPI_monitor_vals;
	MPI_Op       MPI_SUM_monitor_vals;

public:
	Monitor_reduction_mpi(const std::vector<M*> &monitors);
	virtual ~Monitor_reduction_mpi() = default;

	void reset();

protected:
	void _reduce(bool fully = false);
};
}
}
#include "Monitor_reduction_mpi.hxx"

#endif /* MONITOR_REDUCTION_MPI_HPP_ */

#endif
