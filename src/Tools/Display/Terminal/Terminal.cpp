#include <csignal>

#include "Tools/Exception/exception.hpp"
#include "Terminal.hpp"

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

	// Install a signal handler
#ifdef ENABLE_MPI
	std::signal(SIGUSR1, Terminal::signal_interrupt_handler);
#else
	std::signal(SIGINT, Terminal::signal_interrupt_handler);
#endif
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

	if (!Terminal::is_over() || et >= 1.f)
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
	return Terminal::over;
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
	Terminal::interrupt_cnt++;

	auto t_now = std::chrono::steady_clock::now();
	if (!Terminal::first_interrupt)
	{
		auto d_delta_interrupt = t_now - Terminal::t_last_interrupt;
		if (d_delta_interrupt < std::chrono::milliseconds(500))
			Terminal::stop();

		if (d_delta_interrupt < std::chrono::milliseconds(2100))
		{
			if (Terminal::interrupt_cnt >= 4)
			{
				std::cerr << "\r# Killed by user interruption!"
				             "                                                                                         "
				          << std::endl;
				std::exit(EXIT_FAILURE);
			}
		}
		else
			Terminal::interrupt_cnt = 1;
	}
	Terminal::t_last_interrupt  = t_now;

	Terminal::first_interrupt = false;
	Terminal::interrupt       = true;
}