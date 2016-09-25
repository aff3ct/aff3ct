#include <string>
#include <vector>
#include <cassert>

#include "Monitor_std.hpp"

template <typename B>
Monitor_std<B>
::Monitor_std(const int& K, const int& N, const int& max_fe, const int& n_frames, const std::string name)
: Monitor<B>(K, N, n_frames, name.c_str()),
  max_fe(max_fe),
  n_bit_errors(0),
  n_frame_errors(0),
  n_analyzed_frames(0),
  last_frame_error(false)
{
	assert(n_frames > 0);
}

template <typename B>
bool Monitor_std<B>
::fe_limit_achieved() const
{
	return (get_n_fe() >= get_fe_limit()) || Monitor<B>::interrupt;
}

template <typename B>
void Monitor_std<B>
::update_n_analyzed_frames()
{
	n_analyzed_frames += (unsigned long long)this->n_frames;
}

template <typename B>
void Monitor_std<B>
::check_errors(const mipp::vector<B>& U, const mipp::vector<B>& V)
{
	last_frame_error = false;
	assert(U.size() == V.size());
	auto n = (int)U.size() / this->n_frames;

	for (auto i = 0; i < this->n_frames; i++)
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

template <typename B>
int Monitor_std<B>
::get_fe_limit() const
{
	return max_fe;
}

template <typename B>
unsigned long long Monitor_std<B>
::get_n_analyzed_fra() const
{
	return n_analyzed_frames;
}

template <typename B>
int Monitor_std<B>
::get_n_fe() const
{
	return n_frame_errors;
}

template <typename B>
int Monitor_std<B>
::get_n_be() const
{
	return n_bit_errors;
}

template <typename B>
float Monitor_std<B>
::get_fer() const
{
	auto t_fer = (float)get_n_fe() / (float)get_n_analyzed_fra();
	return t_fer;
}

template <typename B>
float Monitor_std<B>
::get_ber() const
{
	auto t_ber = (float)get_n_be() / (float)get_n_analyzed_fra() / (float)this->K;
	return t_ber;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Monitor_std<B_8>;
template class Monitor_std<B_16>;
template class Monitor_std<B_32>;
template class Monitor_std<B_64>;
#else
template class Monitor_std<B>;
#endif
// ==================================================================================== explicit template instantiation
