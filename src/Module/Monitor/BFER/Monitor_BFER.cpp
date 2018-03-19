#include <string>
#include <vector>
#include <stdexcept>

#include "Monitor_BFER.hpp"
#include "Tools/Perf/common.h"

using namespace aff3ct::module;

template <typename B>
Monitor_BFER<B>
::Monitor_BFER(const int size, const unsigned max_fe, const int n_frames)
: Monitor(size, n_frames),
  max_fe(max_fe),
  n_bit_errors(0),
  n_frame_errors(0),
  n_analyzed_frames(0)
{
	const std::string name = "Monitor_BFER";
	this->set_name(name);

	auto &p = this->create_task("check_errors", mnt::tsk::check_errors);
	auto &ps_U = this->template create_socket_in<B>(p, "U", this->size * this->n_frames);
	auto &ps_V = this->template create_socket_in<B>(p, "V", this->size * this->n_frames);
	this->create_codelet(p, [this, &ps_U, &ps_V]() -> int
	{
		return this->check_errors(static_cast<B*>(ps_U.get_dataptr()),
		                          static_cast<B*>(ps_V.get_dataptr()));
	});
}

template <typename B>
int Monitor_BFER<B>
::check_errors(const B *U, const B *V, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	int n_be = 0;
	for (auto f = f_start; f < f_stop; f++)
		n_be += this->_check_errors(U + f * this->size,
		                            V + f * this->size,
		                            f);

	return n_be;
}

template <typename B>
int Monitor_BFER<B>
::_check_errors(const B *U, const B *V, const int frame_id)
{
	int bit_errors_count = tools::hamming_distance(U, V, this->size);

	if (bit_errors_count)
	{
		n_bit_errors += bit_errors_count;
		n_frame_errors++;

		for (auto c : this->callbacks_fe)
			c(bit_errors_count, frame_id);

		if (this->fe_limit_achieved() && frame_id == this->n_frames -1)
			for (auto c : this->callbacks_fe_limit_achieved)
				c();
	}

	n_analyzed_frames++;

	if (frame_id == this->n_frames -1)
		for (auto c : this->callbacks_check)
			c();

	return bit_errors_count;
}

template <typename B>
bool Monitor_BFER<B>
::fe_limit_achieved()
{
	return (get_n_fe() >= get_fe_limit()) || Monitor::interrupt;
}

template <typename B>
unsigned Monitor_BFER<B>
::get_fe_limit() const
{
	return max_fe;
}

template <typename B>
unsigned long long Monitor_BFER<B>
::get_n_analyzed_fra() const
{
	return n_analyzed_frames;
}

template <typename B>
unsigned long long Monitor_BFER<B>
::get_n_fe() const
{
	return n_frame_errors;
}

template <typename B>
unsigned long long Monitor_BFER<B>
::get_n_be() const
{
	return n_bit_errors;
}

template <typename B>
float Monitor_BFER<B>
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
float Monitor_BFER<B>
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
void Monitor_BFER<B>
::add_handler_fe(std::function<void(unsigned, int)> callback)
{
	this->callbacks_fe.push_back(callback);
}

template <typename B>
void Monitor_BFER<B>
::add_handler_check(std::function<void(void)> callback)
{
	this->callbacks_check.push_back(callback);
}

template <typename B>
void Monitor_BFER<B>
::add_handler_fe_limit_achieved(std::function<void(void)> callback)
{
	this->callbacks_fe_limit_achieved.push_back(callback);
}

template <typename B>
void Monitor_BFER<B>
::reset()
{
	Monitor::reset();

	this->n_bit_errors      = 0;
	this->n_frame_errors    = 0;
	this->n_analyzed_frames = 0;
}

template <typename B>
void Monitor_BFER<B>
::clear_callbacks()
{
	this->callbacks_fe               .clear();
	this->callbacks_check            .clear();
	this->callbacks_fe_limit_achieved.clear();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Monitor_BFER<B_8>;
template class aff3ct::module::Monitor_BFER<B_16>;
template class aff3ct::module::Monitor_BFER<B_32>;
template class aff3ct::module::Monitor_BFER<B_64>;
#else
template class aff3ct::module::Monitor_BFER<B>;
#endif
// ==================================================================================== explicit template instantiation
