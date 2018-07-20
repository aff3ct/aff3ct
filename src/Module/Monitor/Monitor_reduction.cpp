#include <sstream>
#include "Tools/Exception/exception.hpp"

#include "Monitor_reduction.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

bool                                                                         aff3ct::module::Monitor_reduction::at_last_reduce = false;
std::vector<aff3ct::module::Monitor_reduction*>                              aff3ct::module::Monitor_reduction::monitors;
std::thread::id                                                              aff3ct::module::Monitor_reduction::master_thread_id = std::this_thread::get_id();
std::chrono::nanoseconds                                                     aff3ct::module::Monitor_reduction::d_reduce_frequency = std::chrono::milliseconds(1000);
std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> aff3ct::module::Monitor_reduction::t_last_mpi_comm;


Monitor_reduction
::Monitor_reduction()
{
	Monitor_reduction::add_monitor(this);
}

void Monitor_reduction
::add_monitor(Monitor_reduction* m)
{
	monitors.push_back(m);
}

void Monitor_reduction
::reset_all()
{
	t_last_mpi_comm  = std::chrono::steady_clock::now();
	at_last_reduce   = false;

		for(auto& m : monitors)
			m->reset_mr();
}

void Monitor_reduction
::reduce(bool fully, bool force)
{
	__reduce__(fully, force, false);
}

void Monitor_reduction
::last_reduce(bool fully)
{
	while(!__reduce__(fully, true, true))
		;
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


bool Monitor_reduction
::__reduce__(bool fully, bool force, bool last)
{
	bool all_done = last;

	// only the master thread can do this
	if (std::this_thread::get_id() == master_thread_id
	    && (force || (std::chrono::steady_clock::now() - t_last_mpi_comm) >= d_reduce_frequency)
	   )
	{
		for(auto& m : monitors)
			all_done &= m->_reduce(fully, last);

		t_last_mpi_comm = std::chrono::steady_clock::now();
	}

	return all_done;
}