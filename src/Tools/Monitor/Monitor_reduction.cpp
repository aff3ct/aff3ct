#include <cmath>
#include <sstream>
#ifdef AFF3CT_MPI
#include <mpi.h>
#endif

#include "Tools/Exception/exception.hpp"
#include "Tools/Monitor/Monitor_reduction.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

bool                                                                         aff3ct::tools::Monitor_reduction_static::stop_loop = false;
std::vector<aff3ct::tools::Monitor_reduction_static*>                        aff3ct::tools::Monitor_reduction_static::monitors;
std::thread::id                                                              aff3ct::tools::Monitor_reduction_static::master_thread_id = std::this_thread::get_id();
std::chrono::nanoseconds                                                     aff3ct::tools::Monitor_reduction_static::d_reduce_frequency = std::chrono::milliseconds(1000);
std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> aff3ct::tools::Monitor_reduction_static::t_last_reduction;

Monitor_reduction_static
::Monitor_reduction_static()
{
	Monitor_reduction_static::add_monitor(this);
}

Monitor_reduction_static
::~Monitor_reduction_static()
{
	Monitor_reduction_static::remove_monitor(this);
}

void Monitor_reduction_static
::add_monitor(Monitor_reduction_static* m)
{
	Monitor_reduction_static::monitors.push_back(m);
}

void Monitor_reduction_static
::remove_monitor(Monitor_reduction_static* m)
{
	for (size_t i = 0; i < Monitor_reduction_static::monitors.size(); i++)
		if (m == Monitor_reduction_static::monitors[i])
		{
			 Monitor_reduction_static::monitors.erase(Monitor_reduction_static::monitors.begin() +i);
			 break;
		}
}

void Monitor_reduction_static
::reset_all()
{
	Monitor_reduction_static::t_last_reduction = std::chrono::steady_clock::now();
	Monitor_reduction_static::stop_loop        = false;

	for(auto& m : Monitor_reduction_static::monitors)
		m->reset();
}

bool Monitor_reduction_static
::is_done()
{
	return this->_is_done();
}

bool Monitor_reduction_static
::is_done_all(bool fully)
{
	reduce_all(fully, false);

	bool is_done = false;

	for(auto& m : Monitor_reduction_static::monitors)
		is_done |= m->_is_done();

	if (is_done)
		set_stop_loop();

	return get_stop_loop();
}

void Monitor_reduction_static
::reduce_all(bool fully, bool force)
{
	_reduce(fully, force);
}

void Monitor_reduction_static
::last_reduce_all(bool fully)
{
	Monitor_reduction_static::set_stop_loop();
	while(!_reduce(fully, true));
}

void Monitor_reduction_static
::check_reducible()
{
#ifdef AFF3CT_MPI
	int n_monitor_send = Monitor_reduction_static::monitors.size(), n_monitor_recv;
	if (auto ret = MPI_Allreduce(&n_monitor_send, &n_monitor_recv, 1, MPI_INT, MPI_PROD, MPI_COMM_WORLD))
	{
		std::stringstream message;
		message << "'MPI_Allreduce' returned '" << ret << "' error code.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	int np;
	if (auto ret = MPI_Comm_size(MPI_COMM_WORLD, &np))
	{
		std::stringstream message;
		message << "'MPI_Comm_size' returned '" << ret << "' error code.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	int pow_np = std::pow(n_monitor_send, np);
	if (n_monitor_recv != pow_np)
	{
		int mpi_rank;
		if (auto ret = MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank))
		{
			std::stringstream message;
			message << "'MPI_Comm_rank' returned '" << ret << "' error code.";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		std::stringstream message;
		message << "The number of monitors to reduce (" << n_monitor_send << " monitors) with MPI on the process "
		        << mpi_rank << " is different than for others processes. ('n_monitor_recv' = " << n_monitor_recv
		        << ", and 'pow_np' = " << pow_np << ").";
		throw tools::logic_error(__FILE__, __LINE__, __func__, message.str());
	}
#endif
}

bool Monitor_reduction_static
::reduce_stop_loop()
{
#ifdef AFF3CT_MPI
	int n_stop_recv, stop_send = Monitor_reduction_static::get_stop_loop() ? 1 : 0;
	if (auto ret = MPI_Allreduce(&stop_send, &n_stop_recv, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD))
	{
		std::stringstream message;
		message << "'MPI_Allreduce' returned '" << ret << "' error code.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (n_stop_recv > 0)
		Monitor_reduction_static::set_stop_loop();

	int np;
	if (auto ret = MPI_Comm_size(MPI_COMM_WORLD, &np))
	{
		std::stringstream message;
		message << "'MPI_Comm_size' returned '" << ret << "' error code.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return n_stop_recv == np;
#else
	return true;
#endif
}

bool Monitor_reduction_static
::_reduce(bool fully, bool force)
{
	bool all_process_on_last = false;

	// only the master thread can do this
	if (force || (std::this_thread::get_id() == Monitor_reduction_static::master_thread_id &&
	              (std::chrono::steady_clock::now() - Monitor_reduction_static::t_last_reduction) >=
	               Monitor_reduction_static::d_reduce_frequency))
	{
		for (auto& m : Monitor_reduction_static::monitors)
			m->reduce(fully);

		all_process_on_last = reduce_stop_loop();

		Monitor_reduction_static::t_last_reduction = std::chrono::steady_clock::now();
	}

	return all_process_on_last;
}

void Monitor_reduction_static
::set_master_thread_id(std::thread::id t)
{
	Monitor_reduction_static::master_thread_id = t;
}

void Monitor_reduction_static
::set_reduce_frequency(std::chrono::nanoseconds d)
{
	Monitor_reduction_static::d_reduce_frequency = d;
}

bool Monitor_reduction_static
::get_stop_loop()
{
	return Monitor_reduction_static::stop_loop;
}

void Monitor_reduction_static
::set_stop_loop()
{
	Monitor_reduction_static::stop_loop = true;
}