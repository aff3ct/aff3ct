#include "Tools/Exception/exception.hpp"

#include "Monitor.hpp"

bool aff3ct::module::Monitor::interrupt       = false;
bool aff3ct::module::Monitor::first_interrupt = true;
int  aff3ct::module::Monitor::interrupt_cnt   = 0;
bool aff3ct::module::Monitor::over            = false;

std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> aff3ct::module::Monitor::t_last_interrupt;

using namespace aff3ct;
using namespace aff3ct::module;

Monitor
::Monitor(const int K, const int N, int n_frames)
: Module(n_frames), K(K), N(N)
{
	const std::string name = "Monitor";
	this->set_name(name);
	this->set_short_name(name);

	if (K <= 0)
	{
		std::stringstream message;
		message << "'K' has to be greater than 0 ('K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (N <= 0)
	{
		std::stringstream message;
		message << "'N' has to be greater than 0 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	Monitor::interrupt = false;

#ifndef ENABLE_MPI
	// Install a signal handler
	std::signal(SIGINT, Monitor::signal_interrupt_handler);
#endif

	this->tasks_with_nullptr.resize((size_t)mnt::tsk::SIZE);
	for (size_t t = 0; t < (size_t)mnt::tsk::SIZE; t++)
		this->tasks_with_nullptr[t] = nullptr;
}

/*!
 * \brief Destructor.
 */
Monitor
::~Monitor()
{
}

int Monitor
::get_K() const
{
	return K;
}

int Monitor
::get_N() const
{
	return N;
}

void Monitor
::reset()
{
	Monitor::interrupt = false;
	Monitor::interrupt_cnt = 0;
}

void Monitor
::clear_callbacks()
{
}

bool Monitor
::is_interrupt()
{
	return Monitor::interrupt;
}

bool Monitor
::is_over()
{
	return Monitor::over;
}

void Monitor
::stop()
{
	Monitor::interrupt = true;
	Monitor::over      = true;
}

void Monitor
::signal_interrupt_handler(int signal)
{
	Monitor::interrupt_cnt++;

	auto t_now = std::chrono::steady_clock::now();
	if (!Monitor::first_interrupt)
	{
		auto d_delta_interrupt = t_now - Monitor::t_last_interrupt;
		if (d_delta_interrupt < std::chrono::milliseconds(500))
			Monitor::stop();

		if (d_delta_interrupt < std::chrono::milliseconds(2100))
		{
			if (Monitor::interrupt_cnt >= 4)
			{
				std::cerr << "\r# Killed by user interruption!"
				             "                                                                  "
				          << std::endl;
				std::exit(EXIT_FAILURE);
			}
		}
		else
			Monitor::interrupt_cnt = 1;
	}
	Monitor::t_last_interrupt  = t_now;

	Monitor::first_interrupt = false;
	Monitor::interrupt       = true;
}