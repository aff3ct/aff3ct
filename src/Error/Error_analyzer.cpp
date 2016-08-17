#include "Error_analyzer.hpp"

#include <string>
#include <vector>
#include <cassert>
#include <csignal>

template <typename B, typename R>
bool Error_analyzer<B,R>::interrupt = false;

template <typename B, typename R>
bool Error_analyzer<B,R>::first_interrupt = true;

template <typename B, typename R>
bool Error_analyzer<B,R>::over = false;

template <typename B, typename R>
std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> Error_analyzer<B,R>::t_last_interrupt;

template <typename B, typename R>
std::chrono::nanoseconds Error_analyzer<B,R>::d_delta_interrupt = std::chrono::nanoseconds(0);

template <typename B, typename R>
Error_analyzer<B,R>
::Error_analyzer(const int& K, const int& N, const int& max_fe, const int& n_frames, const std::string name)
: K(K),
  N(N),
  max_fe(max_fe),
  n_frames(n_frames),
  n_bit_errors(0),
  n_frame_errors(0),
  n_analyzed_frames(0)
{
	assert(n_frames > 0);

	Error_analyzer<B,R>::interrupt = false;
	Error_analyzer<B,R>::d_delta_interrupt = std::chrono::nanoseconds(0);

	// Install a signal handler
	std::signal(SIGINT, Error_analyzer<B,R>::signal_interrupt_handler);
}

template <typename B, typename R>
bool Error_analyzer<B,R>
::fe_limit_achieved() const
{
    return (get_n_fe() >= get_fe_limit()) || Error_analyzer<B,R>::interrupt;
}

template <typename B, typename R>
void Error_analyzer<B,R>
::update_n_analyzed_frames()
{
	n_analyzed_frames += (unsigned long long)n_frames;
}

template <typename B, typename R>
void Error_analyzer<B,R>
::check_errors(const mipp::vector<B>& U, const mipp::vector<B>& V)
{
	last_frame_error = false;
	assert(U.size() == V.size());
	auto n = (int)U.size() / n_frames;

	for (auto i = 0; i < n_frames; i++)
	{
		auto bit_errors_count = 0;
		for (auto j = 0; j < n; j++)
			bit_errors_count += U[i * n + j] != V[i * n + j];

		last_frame_error = bit_errors_count != 0;

		if (bit_errors_count)
		{
			n_bit_errors += bit_errors_count;
			n_frame_errors++;
		}
	}

	this->update_n_analyzed_frames();
}

template <typename B, typename R>
int Error_analyzer<B,R>
::get_fe_limit() const
{
	return max_fe;
}

template <typename B, typename R>
unsigned long long Error_analyzer<B,R>
::get_n_analyzed_frames() const
{
	return n_analyzed_frames;
}

template <typename B, typename R>
int Error_analyzer<B,R>
::get_n_fe() const
{
	return n_frame_errors;
}

template <typename B, typename R>
int Error_analyzer<B,R>
::get_n_be() const
{
	return n_bit_errors;
}

template <typename B, typename R>
R Error_analyzer<B,R>
::get_fer_value() const
{
	auto t_fer = (R)get_n_fe() / (R)get_n_analyzed_frames();
	return t_fer;
}

template <typename B, typename R>
R Error_analyzer<B,R>
::get_ber_value() const
{
	auto t_ber = (R)get_n_be() / (R)get_n_analyzed_frames() / (R)K;
	return t_ber;
}

template <typename B, typename R>
int Error_analyzer<B,R>
::get_N() const
{
	return N;
}

template <typename B, typename R>
int Error_analyzer<B,R>
::get_K() const
{
	return K;
}

template <typename B, typename R>
int Error_analyzer<B,R>
::get_n_frames() const
{
	return n_frames;
}
template <typename B, typename R>
void Error_analyzer<B,R>
::set_n_frames(const int n_frames) 
{ 
	assert(n_frames > 0);
	this->n_frames = n_frames; 
}

template <typename B, typename R>
bool Error_analyzer<B,R>
::is_interrupt()
{
	return Error_analyzer<B,R>::interrupt;
}

template <typename B, typename R>
bool Error_analyzer<B,R>
::is_over()
{
	return Error_analyzer<B,R>::over;
}

template <typename B, typename R>
void Error_analyzer<B,R>
::signal_interrupt_handler(int signal)
{
	auto t_now = std::chrono::steady_clock::now();
	if (!Error_analyzer<B,R>::first_interrupt)
	{
		Error_analyzer<B,R>::d_delta_interrupt = t_now - Error_analyzer<B,R>::t_last_interrupt;
		if (Error_analyzer<B,R>::d_delta_interrupt < std::chrono::milliseconds(500))
			Error_analyzer<B,R>::over = true;
	}
	Error_analyzer<B,R>::t_last_interrupt  = t_now;

	Error_analyzer<B,R>::first_interrupt = false;
	Error_analyzer<B,R>::interrupt       = true;
}

// ==================================================================================== explicit template instantiation 
#include "../Tools/types.h"
#ifdef MULTI_PREC
template class Error_analyzer<B_8,R_8>;
template class Error_analyzer<B_16,R_16>;
template class Error_analyzer<B_32,R_32>;
template class Error_analyzer<B_64,R_64>;
#else
template class Error_analyzer<B,R>;
#endif
// ==================================================================================== explicit template instantiation