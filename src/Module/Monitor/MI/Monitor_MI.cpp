#include <string>
#include <vector>
#include <stdexcept>

#include "Monitor_MI.hpp"
#include "Tools/Perf/common/mutual_info.h"
#include "Tools/Perf/common/hamming_distance.h"
#include "Tools/Math/utils.h"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Monitor_MI<B,R>
::Monitor_MI(const int K, const int N, const unsigned max_n_trials, const int n_frames)
: Monitor(K, N, n_frames),
  max_n_trials(max_n_trials)
{
	const std::string name = "Monitor_MI";
	this->set_name(name);

	auto &p = this->create_task("get_mutual_info", (int)mnt::tsk::get_mutual_info);
	auto &ps_X = this->template create_socket_in<B>(p, "X", this->N * this->n_frames);
	auto &ps_Y = this->template create_socket_in<R>(p, "Y", this->N * this->n_frames);
	this->create_codelet(p, [this, &ps_X, &ps_Y]() -> int
	{
		return this->get_mutual_info(static_cast<B*>(ps_X.get_dataptr()),
		                             static_cast<R*>(ps_Y.get_dataptr()));
	});

	this->vals.reset();
}

template <typename B, typename R>
R Monitor_MI<B,R>
::get_mutual_info(const B *X, const R *Y, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	R loc_MI_sum = 0;
	for (auto f = f_start; f < f_stop; f++)
		loc_MI_sum += this->_get_mutual_info(X + f * this->N, Y + f * this->N, f);

	if (n_trials_limit_achieved() && frame_id == this->n_frames -1)
		for (auto& c : this->callbacks_n_trials_limit_achieved)
			c();

	return loc_MI_sum / (R)(f_stop - f_start) * 10000; // return the mut info computed now %10000 instead of %100
}

template <typename B, typename R>
R Monitor_MI<B,R>
::_get_mutual_info(const B *X, const R *Y, const int frame_id)
{
	throw tools::runtime_error(__FILE__, __LINE__, __func__, "The _get_mutual_info() function does not support this type.");
}

#include "Tools/types.h"
#if defined(MULTI_PREC) | defined (PREC_32_BIT)
namespace aff3ct
{
namespace module
{
template <>
Q_32 Monitor_MI<B_32,Q_32>
::_get_mutual_info(const B_32 *X, const Q_32 *Y, const int frame_id)
{
	auto mi = tools::mutual_info_histo(X, Y, this->N);
	this->vals.MI_sum += mi;

	this->vals.n_fra++;

	if (frame_id == this->n_frames -1)
		for (auto& c : this->callbacks_check)
			c();

	return mi;
}
}
}
#endif

#if defined(MULTI_PREC) | defined (PREC_64_BIT)
namespace aff3ct
{
namespace module
{
template <>
Q_64 Monitor_MI<B_64,Q_64>
::_get_mutual_info(const B_64 *X, const Q_64 *Y, const int frame_id)
{
	auto mi = tools::mutual_info_histo(X, Y, this->N);
	this->vals.MI_sum += mi;

	this->vals.n_fra++;

	if (frame_id == this->n_frames -1)
		for (auto c : this->callbacks_check)
			c();

	return mi;
}
}
}
#endif

template <typename B, typename R>
bool Monitor_MI<B,R>
::n_trials_limit_achieved()
{
	return (get_n_analyzed_fra() >= get_n_trials_limit()) || Monitor::interrupt;
}

template <typename B, typename R>
unsigned Monitor_MI<B,R>
::get_n_trials_limit() const
{
	return max_n_trials;
}

template <typename B, typename R>
unsigned long long Monitor_MI<B,R>
::get_n_analyzed_fra() const
{
	return this->vals.n_fra;
}

template <typename B, typename R>
R Monitor_MI<B,R>
::get_MI() const
{
	return this->get_MI_sum() / (R)this->get_n_analyzed_fra();
}

template <typename B, typename R>
R Monitor_MI<B,R>
::get_MI_sum() const
{
	return this->vals.MI_sum;
}

template <typename B, typename R>
void Monitor_MI<B,R>
::add_handler_check(std::function<void(void)> callback)
{
	this->callbacks_check.push_back(callback);
}

template <typename B, typename R>
void Monitor_MI<B,R>
::add_handler_n_trials_limit_achieved(std::function<void(void)> callback)
{
	this->callbacks_n_trials_limit_achieved.push_back(callback);
}

template <typename B, typename R>
void Monitor_MI<B,R>
::reset()
{
	Monitor::reset();

	this->vals.reset();
}

template <typename B, typename R>
void Monitor_MI<B,R>
::clear_callbacks()
{
	Monitor::clear_callbacks();

	this->callbacks_check.clear();
	this->callbacks_n_trials_limit_achieved.clear();
}

template <typename B, typename R>
void Monitor_MI<B,R>
::collect(const Monitor_MI<B,R>& m)
{
	Monitor::collect(m);

	this->vals += m.vals;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Monitor_MI<B_8, R_8>;
template class aff3ct::module::Monitor_MI<B_16,R_16>;
template class aff3ct::module::Monitor_MI<B_32,R_32>;
template class aff3ct::module::Monitor_MI<B_64,R_64>;
#else
template class aff3ct::module::Monitor_MI<B,R>;
#endif
// ==================================================================================== explicit template instantiation
