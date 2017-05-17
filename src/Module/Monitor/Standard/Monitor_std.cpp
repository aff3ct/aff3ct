#include <string>
#include <vector>
#include <stdexcept>

#include "Monitor_std.hpp"

using namespace aff3ct::module;

template <typename B>
Monitor_std<B>
::Monitor_std(const int size, const unsigned max_fe, const int n_frames, const std::string name)
: Monitor<B>(size, n_frames, name.c_str()),
  max_fe(max_fe),
  n_bit_errors(0),
  n_frame_errors(0),
  n_analyzed_frames(0)
{
}

template <typename B>
bool Monitor_std<B>
::fe_limit_achieved()
{
	return (get_n_fe() >= get_fe_limit()) || Monitor<B>::interrupt;
}

template <typename B>
void Monitor_std<B>
::_check_errors(const B *U, const B *V)
{
	auto bit_errors_count = 0;
	for (auto b = 0; b < this->size; b++)
		bit_errors_count += !U[b] != !V[b];

	if (bit_errors_count)
	{
		n_bit_errors += bit_errors_count;
		n_frame_errors++;

		for (auto c : this->callbacks_fe)
			c(n_analyzed_frames % this->n_frames);

		if (this->fe_limit_achieved())
			for (auto c : this->callbacks_fe_limit_achieved)
				c();
	}

	n_analyzed_frames++;

	for (auto c : this->callbacks_check)
		c();
}

template <typename B>
unsigned Monitor_std<B>
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
unsigned long long Monitor_std<B>
::get_n_fe() const
{
	return n_frame_errors;
}

template <typename B>
unsigned long long Monitor_std<B>
::get_n_be() const
{
	return n_bit_errors;
}

template <typename B>
float Monitor_std<B>
::get_fer() const
{
	auto t_fer = 0.f;
	if (this->get_n_be() != 0)
		t_fer = (float)this->get_n_fe() / (float)this->get_n_analyzed_fra();
	else
		t_fer = (1.f) / ((float)this->get_n_analyzed_fra());

	return t_fer;
}

template <typename B>
float Monitor_std<B>
::get_ber() const
{
	auto t_ber = 0.f;
	if (this->get_n_be() != 0)
		t_ber = (float)this->get_n_be() / (float)this->get_n_analyzed_fra() / (float)this->get_size();
	else
		t_ber = (1.f) / ((float)this->get_n_analyzed_fra()) / this->get_size();

	return t_ber;
}

template <typename B>
void Monitor_std<B>
::add_handler_fe(std::function<void(int)> callback)
{
	this->callbacks_fe.push_back(callback);
}
template <typename B>
void Monitor_std<B>
::add_handler_check(std::function<void(void)> callback)
{
	this->callbacks_check.push_back(callback);
}

template <typename B>
void Monitor_std<B>
::add_handler_fe_limit_achieved(std::function<void(void)> callback)
{
	this->callbacks_fe_limit_achieved.push_back(callback);
}

template <typename B>
void Monitor_std<B>
::reset()
{
	this->n_bit_errors      = 0;
	this->n_frame_errors    = 0;
	this->n_analyzed_frames = 0;

	this->callbacks_fe               .clear();
	this->callbacks_check            .clear();
	this->callbacks_fe_limit_achieved.clear();
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Monitor_std<B_8>;
template class aff3ct::module::Monitor_std<B_16>;
template class aff3ct::module::Monitor_std<B_32>;
template class aff3ct::module::Monitor_std<B_64>;
#else
template class aff3ct::module::Monitor_std<B>;
#endif
// ==================================================================================== explicit template instantiation
