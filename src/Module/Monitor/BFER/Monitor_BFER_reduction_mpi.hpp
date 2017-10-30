#ifdef ENABLE_MPI

#ifndef MONITOR_REDUCTION_MPI_HPP_
#define MONITOR_REDUCTION_MPI_HPP_

#include <thread>
#include <chrono>
#include <vector>

#include <mpi.h>

#include "Monitor_BFER_reduction.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Monitor_BFER_reduction_mpi : public Monitor_BFER_reduction<B>
{
private:
	const std::thread::id master_thread_id;
	bool is_fe_limit_achieved;

	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_last_mpi_comm;
	std::chrono::nanoseconds                                                     d_mpi_comm_frequency;

	MPI_Datatype MPI_monitor_vals;
	MPI_Op       MPI_SUM_monitor_vals;

public:
	Monitor_BFER_reduction_mpi(const int size, const unsigned max_fe,
	                           std::vector<Monitor_BFER<B>*> monitors,
	                           const std::thread::id master_thread_id,
	                           const std::chrono::nanoseconds d_mpi_comm_frequency = std::chrono::milliseconds(1000),
	                           const int n_frames = 1,
	                           const std::string name = "Monitor_BFER_reduction_mpi");
	virtual ~Monitor_BFER_reduction_mpi();

	bool fe_limit_achieved();

	void reset();
};
}
}

#endif /* MONITOR_REDUCTION_MPI_HPP_ */

#endif
