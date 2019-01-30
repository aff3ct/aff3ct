#include <cmath>
#include <sstream>

#ifdef AFF3CT_MPI
#include <mpi.h>
#endif

#include "Tools/Exception/exception.hpp"

#include "Monitor_reduction.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

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
	Monitor_reduction::monitors.push_back(m);
}

void Monitor_reduction
::reset_all()
{
	Monitor_reduction::t_last_reduction = std::chrono::steady_clock::now();
	Monitor_reduction::stop_loop        = false;

	for(auto& m : Monitor_reduction::monitors)
		m->reset_mr();
}

bool Monitor_reduction
::is_done_all(bool fully, bool final)
{
	if (final)
		last_reduce_all(fully);
	else
		reduce_all(fully, false);

	bool is_done = false;

	for(auto& m : Monitor_reduction::monitors)
		is_done |= m->is_done_mr();

	if (is_done)
		set_stop_loop();

	return get_stop_loop();
}

void Monitor_reduction
::reduce_all(bool fully, bool force)
{
	__reduce__(fully, force);
}

void Monitor_reduction
::last_reduce_all(bool fully)
{
	Monitor_reduction::set_stop_loop();

	while(!__reduce__(fully, true));
}

void Monitor_reduction
::check_reducible()
{
#ifdef AFF3CT_MPI
	int n_monitor_send = Monitor_reduction::monitors.size(), n_monitor_recv;
	MPI_Allreduce(&n_monitor_send, &n_monitor_recv, 1, MPI_INT, MPI_PROD, MPI_COMM_WORLD);

	int np;
	MPI_Comm_size(MPI_COMM_WORLD, &np);

	int pow_np = std::pow(n_monitor_send, np);
	if (n_monitor_recv != pow_np)
	{
		int mpi_rank;
		MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

		std::stringstream message;
		message << "The number of monitors to reduce (" << n_monitor_send << " monitors) with MPI on the process "
		        << mpi_rank << " is different than for others processes. ('n_monitor_recv' = " << n_monitor_recv
		        << ", and 'pow_np' = " << pow_np << ").";
		throw tools::logic_error(__FILE__, __LINE__, __func__, message.str());
	}
#endif
}

bool Monitor_reduction
::reduce_stop_loop()
{
#ifdef AFF3CT_MPI
	int n_stop_recv, stop_send = Monitor_reduction::get_stop_loop() ? 1 : 0;
	MPI_Allreduce(&stop_send, &n_stop_recv, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

	if (n_stop_recv > 0)
		Monitor_reduction::set_stop_loop();

	int np;
	MPI_Comm_size(MPI_COMM_WORLD, &np);

	return n_stop_recv == np;
#else
	return true;
#endif
}

bool Monitor_reduction
::__reduce__(bool fully, bool force)
{
	bool all_process_on_last = false;

	// only the master thread can do this
	if (force || (std::this_thread::get_id() == Monitor_reduction::master_thread_id &&
	              (std::chrono::steady_clock::now() - Monitor_reduction::t_last_reduction) >=
	               Monitor_reduction::d_reduce_frequency))
	{
		for (auto& m : Monitor_reduction::monitors)
			m->_reduce(fully);

		all_process_on_last = reduce_stop_loop();

		Monitor_reduction::t_last_reduction = std::chrono::steady_clock::now();
	}

	return all_process_on_last;
}

void Monitor_reduction
::set_master_thread_id(std::thread::id t)
{
	Monitor_reduction::master_thread_id = t;
}

void Monitor_reduction
::set_reduce_frequency(std::chrono::nanoseconds d)
{
	Monitor_reduction::d_reduce_frequency = d;
}

bool Monitor_reduction
::get_stop_loop()
{
	return Monitor_reduction::stop_loop;
}

void Monitor_reduction
::set_stop_loop()
{
	Monitor_reduction::stop_loop = true;
}