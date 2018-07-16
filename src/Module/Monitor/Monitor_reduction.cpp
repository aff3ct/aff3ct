#include <sstream>
#include "Tools/Exception/exception.hpp"

#include "Monitor_reduction.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

std::vector<aff3ct::module::Monitor_reduction*>                              aff3ct::module::Monitor_reduction::monitors;
std::thread::id                                                              aff3ct::module::Monitor_reduction::master_thread_id = std::this_thread::get_id();
std::chrono::nanoseconds                                                     aff3ct::module::Monitor_reduction::d_reduce_frequency = std::chrono::milliseconds(1000);
std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> aff3ct::module::Monitor_reduction::t_last_mpi_comm;


Monitor_reduction
::Monitor_reduction()
{
	Monitor_reduction::add_monitor(this);
	Monitor_reduction::reset();
}

void Monitor_reduction
::add_monitor(Monitor_reduction* m)
{
	monitors.push_back(m);
}

void Monitor_reduction
::reset()
{
	master_thread_id = std::this_thread::get_id();
	t_last_mpi_comm  = std::chrono::steady_clock::now();
}

void Monitor_reduction
::reduce(bool fully, bool force)
{
	// only the master thread can do this
	if (force ||
	    (std::this_thread::get_id() == master_thread_id &&
	     (std::chrono::steady_clock::now() - t_last_mpi_comm) >= d_reduce_frequency)
	   )
	{
		for(auto& m : monitors)
			m->_reduce(fully);

		t_last_mpi_comm = std::chrono::steady_clock::now();
	}
}

void Monitor_reduction
::set_master_thread_id(std::thread::id t)
{
	master_thread_id = t;
}

void Monitor_reduction
::set_reduce_frequency(std::chrono::nanoseconds d)
{
	d_reduce_frequency = d;
}

