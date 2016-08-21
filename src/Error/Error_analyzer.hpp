#ifndef ERROR_ANALYZER_HPP_
#define ERROR_ANALYZER_HPP_

#include <csignal>
#include <chrono>
#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename B>
class Error_analyzer
{
private:
	protected:
	static bool interrupt;
	static bool first_interrupt;
	static bool over;
	static std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_last_interrupt;
	static std::chrono::nanoseconds d_delta_interrupt;

protected:
	const int K; // number of information bits
	const int N; // frame size
	      int n_frames;

public:
	Error_analyzer(const int& K, const int& N, const int& n_frames = 1, const std::string name = "Error_analyzer")
	: K(K), N(N), n_frames(n_frames) 
	{
		Error_analyzer<B>::interrupt = false;
		Error_analyzer<B>::d_delta_interrupt = std::chrono::nanoseconds(0);

		// Install a signal handler
		std::signal(SIGINT, Error_analyzer<B>::signal_interrupt_handler);
	}

	virtual ~Error_analyzer() {}

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

	static bool is_interrupt() { return Error_analyzer<B>::interrupt; }
	static bool is_over     () { return Error_analyzer<B>::over;      }

private:
	static void signal_interrupt_handler(int signal)
	{
		auto t_now = std::chrono::steady_clock::now();
		if (!Error_analyzer<B>::first_interrupt)
		{
			Error_analyzer<B>::d_delta_interrupt = t_now - Error_analyzer<B>::t_last_interrupt;
			if (Error_analyzer<B>::d_delta_interrupt < std::chrono::milliseconds(500))
				Error_analyzer<B>::over = true;
		}
		Error_analyzer<B>::t_last_interrupt  = t_now;

		Error_analyzer<B>::first_interrupt = false;
		Error_analyzer<B>::interrupt       = true;
	}
};

template <typename B>
bool Error_analyzer<B>::interrupt = false;

template <typename B>
bool Error_analyzer<B>::first_interrupt = true;

template <typename B>
bool Error_analyzer<B>::over = false;

template <typename B>
std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> Error_analyzer<B>::t_last_interrupt;

template <typename B>
std::chrono::nanoseconds Error_analyzer<B>::d_delta_interrupt = std::chrono::nanoseconds(0);

#endif /* ERROR_ANALYZER_HPP_ */
