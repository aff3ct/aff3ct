#include <string>
#include <vector>
#include <stdexcept>

#include "Monitor_BFER.hpp"
#include "Tools/Perf/common/mutual_info.h"
#include "Tools/Perf/common/hamming_distance.h"
#include "Tools/Math/utils.h"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Monitor_BFER<B>
::Monitor_BFER(const int K, const int N, const unsigned max_fe, const bool count_unknown_values, const int n_frames)
: Monitor(K, N, n_frames),
  max_fe(max_fe),
  count_unknown_values(count_unknown_values),
  err_hist(0)
{
	const std::string name = "Monitor_BFER";
	this->set_name(name);

	auto &p1 = this->create_task("check_errors", (int)mnt::tsk::check_errors);
	auto &ps_U = this->template create_socket_in<B>(p1, "U", this->K * this->n_frames);
	auto &ps_V = this->template create_socket_in<B>(p1, "V", this->K * this->n_frames);
	this->create_codelet(p1, [this, &ps_U, &ps_V]() -> int
	{
		return this->check_errors(static_cast<B*>(ps_U.get_dataptr()),
		                          static_cast<B*>(ps_V.get_dataptr()));
	});

	this->vals.reset();
}

template <typename B>
int Monitor_BFER<B>
::check_errors(const B *U, const B *V, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	int n_be = 0;
	for (auto f = f_start; f < f_stop; f++)
		n_be += this->_check_errors(U + f * this->K,
		                            V + f * this->K,
		                            f);

	if (this->fe_limit_achieved())
		for (auto& c : this->callbacks_fe_limit_achieved)
			c();

	return n_be;
}

template <typename B>
int Monitor_BFER<B>
::_check_errors(const B *U, const B *V, const int frame_id)
{
	int bit_errors_count;

	if (count_unknown_values)
		bit_errors_count = (int)tools::hamming_distance_unk(U, V, this->K);
	else
		bit_errors_count = (int)tools::hamming_distance(U, V, this->K);

	if (bit_errors_count)
	{
		vals.n_be += bit_errors_count;
		vals.n_fe++;

		err_hist.add_value(bit_errors_count);

		for (auto& c : this->callbacks_fe)
			c(bit_errors_count, frame_id);

	}

	vals.n_fra++;

	if (frame_id == this->n_frames -1)
		for (auto& c : this->callbacks_check)
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
	return vals.n_fra;
}

template <typename B>
unsigned long long Monitor_BFER<B>
::get_n_fe() const
{
	return vals.n_fe;
}

template <typename B>
unsigned long long Monitor_BFER<B>
::get_n_be() const
{
	return vals.n_be;
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
		t_ber = (float)this->get_n_be() / (float)this->get_n_analyzed_fra() / (float)this->get_K();
	else
		t_ber = (1.f) / ((float)this->get_n_analyzed_fra()) / this->get_K();

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

	this->vals    .reset();
	this->err_hist.reset();
}

template <typename B>
void Monitor_BFER<B>
::clear_callbacks()
{
	Monitor::clear_callbacks();

	this->callbacks_fe               .clear();
	this->callbacks_check            .clear();
	this->callbacks_fe_limit_achieved.clear();
}

template<typename B>
tools::Histogram<int> Monitor_BFER<B>::get_err_hist() const
{
	return err_hist;
}

template<typename B>
bool Monitor_BFER<B>
::get_count_unknown_values() const
{
	return count_unknown_values;
}

template <typename B>
void Monitor_BFER<B>
::collect(const Monitor_BFER<B>& m)
{
	Monitor::collect(m);

	this->vals += m.vals;
	err_hist.add_values(m.err_hist);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Monitor_BFER<B_8 >;
template class aff3ct::module::Monitor_BFER<B_16>;
template class aff3ct::module::Monitor_BFER<B_32>;
template class aff3ct::module::Monitor_BFER<B_64>;
#else
template class aff3ct::module::Monitor_BFER<B>;
#endif
// ==================================================================================== explicit template instantiation
