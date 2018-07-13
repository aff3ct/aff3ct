#ifdef ENABLE_MPI
#include <sstream>
#include "Tools/Exception/exception.hpp"

#include "Monitor_reduction_mpi.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

std::vector<aff3ct::module::Monitor_mpi*>                                    aff3ct::module::Monitor_mpi::monitors;
std::thread::id                                                              aff3ct::module::Monitor_mpi::master_thread_id;
std::chrono::nanoseconds                                                     aff3ct::module::Monitor_mpi::d_mpi_comm_frequency;
std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> aff3ct::module::Monitor_mpi::t_last_mpi_comm;


Monitor_mpi
::Monitor_mpi()
{
	Monitor_mpi::add_monitor(this);
	Monitor_mpi::reset();
}

void Monitor_mpi
::add_monitor(Monitor_mpi* m)
{
	monitors.push_back(m);
}

void Monitor_mpi
::reset()
{
	t_last_mpi_comm      = std::chrono::steady_clock::now();
	d_mpi_comm_frequency = std::chrono::milliseconds(1000);
}

void Monitor_mpi
::reduce(bool fully)
{
	// only the master thread can do this
	if (std::this_thread::get_id() == master_thread_id &&
	    ((std::chrono::steady_clock::now() - t_last_mpi_comm) >= d_mpi_comm_frequency))
	{
		fully = false;

		for(auto& m : monitors)
			m->_reduce(fully);

		t_last_mpi_comm = std::chrono::steady_clock::now();
	}
}

void Monitor_mpi
::set_master_thread_id(std::thread::id t)
{
	master_thread_id = t;
}

void Monitor_mpi
::set_mpi_comm_frequency(std::chrono::nanoseconds d)
{
	d_mpi_comm_frequency = d;
}


#endif // ENABLE_MPI
