#include "Terminal.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

Terminal
::Terminal()
: stop_terminal(false)
{
}

Terminal
::~Terminal()
{
}

void Terminal
::legend(std::ostream &stream)
{
}

void Terminal
::final_report(std::ostream &stream)
{
	this->stop_temp_report();
}

void Terminal
::start_temp_report(const std::chrono::milliseconds freq)
{
	this->stop_temp_report();

	// launch a thread dedicated to the terminal display
	term_thread = std::thread(Terminal::start_thread_terminal, this, freq);
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
