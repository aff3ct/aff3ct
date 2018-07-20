#include <sstream>
#include "Tools/Exception/exception.hpp"

#include "Monitor_reduction.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

int                                                                          aff3ct::module::Monitor_reduction::number_processes = 1;
bool                                                                         aff3ct::module::Monitor_reduction::stop_loop = false;
std::vector<aff3ct::module::Monitor_reduction*>                              aff3ct::module::Monitor_reduction::monitors;
std::thread::id                                                              aff3ct::module::Monitor_reduction::master_thread_id = std::this_thread::get_id();
std::chrono::nanoseconds                                                     aff3ct::module::Monitor_reduction::d_reduce_frequency = std::chrono::milliseconds(1000);
std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> aff3ct::module::Monitor_reduction::t_last_reduction;


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
	t_last_reduction = std::chrono::steady_clock::now();
	stop_loop        = false;

		for(auto& m : monitors)
			m->reset_mr();
}

void Monitor_reduction
::reduce(bool fully, bool force)
{
	stop_loop = __reduce__(fully, force, false);
}

void Monitor_reduction
::last_reduce(bool fully)
{
	stop_loop = true;

	while(!__reduce__(fully, true, true))
		;
}

void Monitor_reduction
::set_master_thread_id(std::thread::id t)
{
	master_thread_id = t;
}

void Monitor_reduction
::set_n_processes(int np)
{
	number_processes = np;
}

void Monitor_reduction
::set_reduce_frequency(std::chrono::nanoseconds d)
{
	d_reduce_frequency = d;
}

bool Monitor_reduction
::get_stop_loop()
{
	return stop_loop;
}


bool Monitor_reduction
::__reduce__(bool fully, bool force, bool stop_simu)
{
	bool stop = stop_simu;

	// only the master thread can do this
	if (std::this_thread::get_id() == master_thread_id
	    && (force || (std::chrono::steady_clock::now() - t_last_reduction) >= d_reduce_frequency)
	   )
	{
		for(auto& m : monitors)
		{
			int n_stop = m->_reduce(fully, stop_simu);
			if (stop_simu)
				stop &= n_stop == number_processes;
			else
				stop |= n_stop != 0;
		}

		t_last_reduction = std::chrono::steady_clock::now();
	}

	return stop;
}