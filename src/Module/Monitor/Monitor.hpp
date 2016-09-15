#ifndef MONITOR_HPP_
#define MONITOR_HPP_

#include <csignal>
#include <chrono>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Module.hpp"

template <typename B>
class Monitor_interface : public Module // please use Monitor<B> for inheritance (instead of Monitor_interface<B>)
{
protected:
	static bool interrupt;
	static bool first_interrupt;
	static bool over;
	static std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_last_interrupt;
	static std::chrono::nanoseconds d_delta_interrupt;

	const int K; // number of information bits
	const int N; // frame size

public:
	Monitor_interface(const int& K, const int& N, const int& n_frames = 1, 
	                  const std::string name = "Monitor_interface")
	: Module(n_frames, name), K(K), N(N)
	{
		Monitor_interface<B>::interrupt = false;
		Monitor_interface<B>::d_delta_interrupt = std::chrono::nanoseconds(0);

		// Install a signal handler
		std::signal(SIGINT, Monitor_interface<B>::signal_interrupt_handler);
	}

	virtual ~Monitor_interface() {}

	int get_N()        const { return N;        }
	int get_K()        const { return K;        }
	int get_n_frames() const { return n_frames; }

	virtual void set_n_frames(const int n_frames)
	{
		assert(n_frames > 0);
		this->n_frames = n_frames;
	}

	virtual void check_errors(const mipp::vector<B>& U, const mipp::vector<B>& V) = 0;

	virtual int get_n_be() const = 0;
	virtual int get_n_fe() const = 0;

	virtual float get_ber_value() const = 0;
	virtual float get_fer_value() const = 0;

	virtual unsigned long long get_n_analyzed_frames() const = 0;

	virtual int  get_fe_limit     () const = 0;
	virtual bool fe_limit_achieved() const = 0;

	static bool is_interrupt() { return Monitor_interface<B>::interrupt; }
	static bool is_over     () { return Monitor_interface<B>::over;      }

private:
	static void signal_interrupt_handler(int signal)
	{
		auto t_now = std::chrono::steady_clock::now();
		if (!Monitor_interface<B>::first_interrupt)
		{
			Monitor_interface<B>::d_delta_interrupt = t_now - Monitor_interface<B>::t_last_interrupt;
			if (Monitor_interface<B>::d_delta_interrupt < std::chrono::milliseconds(500))
				Monitor_interface<B>::over = true;
		}
		Monitor_interface<B>::t_last_interrupt  = t_now;

		Monitor_interface<B>::first_interrupt = false;
		Monitor_interface<B>::interrupt       = true;
	}
};

template <typename B>
bool Monitor_interface<B>::interrupt = false;

template <typename B>
bool Monitor_interface<B>::first_interrupt = true;

template <typename B>
bool Monitor_interface<B>::over = false;

template <typename B>
std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> Monitor_interface<B>::t_last_interrupt;

template <typename B>
std::chrono::nanoseconds Monitor_interface<B>::d_delta_interrupt = std::chrono::nanoseconds(0);

#include "SC_Monitor.hpp"

#endif /* MONITOR_HPP_ */
