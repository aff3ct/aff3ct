#ifdef AFF3CT_MPI
#include <mpi.h>
#include <sstream>
#include "Tools/Exception/exception.hpp"
#endif
#include <csignal>
#include <cstdlib>

#include "Tools/Display/Terminal/Terminal.hpp"

bool aff3ct::tools::Terminal::interrupt       = false;
bool aff3ct::tools::Terminal::first_interrupt = true;
int  aff3ct::tools::Terminal::interrupt_cnt   = 0;
bool aff3ct::tools::Terminal::over            = false;

std::chrono::time_point<std::chrono::steady_clock> aff3ct::tools::Terminal::t_last_interrupt;

using namespace aff3ct;
using namespace aff3ct::tools;

Terminal
::Terminal()
: stop_terminal(false),
  t_term(std::chrono::steady_clock::now()),
  real_time_state(0)
{
	Terminal::interrupt = false;
	Terminal::init();
}

void Terminal
::init()
{
	// Install a signal handler
#if !defined(_WIN64) && !defined(_WIN32)
	std::signal(SIGUSR1, Terminal::signal_interrupt_handler);
	std::signal(SIGUSR2, Terminal::signal_interrupt_handler);
#endif
	std::signal(SIGINT,  Terminal::signal_interrupt_handler);
	std::signal(SIGTERM, Terminal::signal_interrupt_handler);
}

Terminal
::~Terminal()
{
	stop_temp_report(); // try to join the thread if this is not been done by the user
}

void Terminal
::temp_report(std::ostream &stream)
{
	this->report(stream, false);
}

void Terminal
::final_report(std::ostream &stream)
{
	this->stop_temp_report();

	auto et = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - t_term).count();

	if (!Terminal::over || et >= 1.f)
		this->report(stream, true);

	t_term = std::chrono::steady_clock::now();
}

void Terminal
::start_temp_report(const std::chrono::milliseconds freq)
{
	this->stop_temp_report();

	// launch a thread dedicated to the terminal display
	term_thread = std::thread(Terminal::start_thread_terminal, this, freq);

	t_term = std::chrono::steady_clock::now();
}

void Terminal
::stop_temp_report()
{
	if (term_thread.joinable())
	{
		stop_terminal = true;
		cond_terminal.notify_all();
		// wait the terminal thread to finish
		term_thread.join();
		stop_terminal = false;
	}

	real_time_state = 0;
}

void Terminal
::start_thread_terminal(Terminal *terminal, const std::chrono::milliseconds freq)
{
	const auto sleep_time = freq - std::chrono::milliseconds(0);
	while (!terminal->stop_terminal)
	{
		std::unique_lock<std::mutex> lock(terminal->mutex_terminal);
		if (terminal->cond_terminal.wait_for(lock, sleep_time) == std::cv_status::timeout)
			terminal->temp_report(std::clog); // display statistics in the terminal
	}
}

void Terminal
::reset()
{
	Terminal::interrupt = false;
	Terminal::interrupt_cnt = 0;
}

bool Terminal
::is_interrupt()
{
	return Terminal::interrupt;
}

bool Terminal
::is_over()
{
#ifdef AFF3CT_MPI
	char over_send = (char)Terminal::over, over_recv;

	if (auto ret = MPI_Allreduce(&over_send, &over_recv, 1, MPI_CHAR, MPI_LOR, MPI_COMM_WORLD))
	{
		std::stringstream message;
		message << "'MPI_Allreduce' returned '" << ret << "' error code.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return (bool)over_recv;
#else
	return Terminal::over;
#endif
}

void Terminal
::stop()
{
	Terminal::interrupt = true;
	Terminal::over      = true;
}

void Terminal
::signal_interrupt_handler(int signal)
{
	bool kill = false;

#if defined(_WIN64) || defined(_WIN32)
	if (signal == SIGINT)
#else
	if (signal == SIGUSR1 || signal == SIGINT)
#endif
	{
		Terminal::interrupt_cnt++;

		auto t_now = std::chrono::steady_clock::now();
		if (!Terminal::first_interrupt)
		{
			auto d_delta_interrupt = t_now - Terminal::t_last_interrupt;
			if (d_delta_interrupt < std::chrono::milliseconds(500))
				Terminal::stop();

			if (d_delta_interrupt < std::chrono::milliseconds(2100))
				kill = true;
			else
				Terminal::interrupt_cnt = 1;
		}
		Terminal::t_last_interrupt = t_now;

		Terminal::first_interrupt = false;
		Terminal::interrupt       = true;
	}
#if !defined(_WIN64) && !defined(_WIN32)
	else if (signal == SIGUSR2)
	{
		Terminal::stop();
	}
#endif
	else if (signal == SIGTERM)
	{
		kill = true;
	}


	if (kill)
	{
		if (Terminal::interrupt_cnt >= 4)
		{
			std::cerr << "\r# Killed by user interruption!"
			             "                                                                                         "
			          << std::endl;
			std::exit(EXIT_FAILURE);
		}
	}
}