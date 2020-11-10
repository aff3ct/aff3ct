#include <algorithm>
#include <sstream>
#include <string>

#include "Tools/Exception/exception.hpp"
#include "Tools/Perf/common/mutual_info.h"
#include "Module/Monitor/MI/Monitor_MI.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Monitor_MI<B,R>
::Monitor_MI(const int N, const unsigned max_n_trials)
: Monitor(), N(N), max_n_trials(max_n_trials),
  mutinfo_hist(1), mutinfo_hist_activated(false)
{
	const std::string name = "Monitor_MI";
	this->set_name(name);
	this->set_single_wave(true);

	if (N <= 0)
	{
		std::stringstream message;
		message << "'N' has to be greater than 0 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p = this->create_task("get_mutual_info", (int)mnt::tsk::get_mutual_info);
	auto ps_X = this->template create_socket_in<B>(p, "X", get_N());
	auto ps_Y = this->template create_socket_in<R>(p, "Y", get_N());
	this->create_codelet(p, [ps_X, ps_Y](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &mnt = static_cast<Monitor_MI<B,R>&>(m);

		auto ret = mnt._get_mutual_info(static_cast<B*>(t[ps_X].get_dataptr()),
		                                static_cast<R*>(t[ps_Y].get_dataptr()),
		                                frame_id);

		return (int)ret;
	});

	reset();
}

template <typename B, typename R>
Monitor_MI<B,R>* Monitor_MI<B,R>
::clone() const
{
	if (this->callback_check.size())
	{
		std::stringstream message;
		message << "'callback_check.size()' has to be equal to 0 ('callback_check.size()' = "
		        << this->callback_check.size() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->callback_n_trials_limit_achieved.size())
	{
		std::stringstream message;
		message << "'callback_n_trials_limit_achieved.size()' has to be equal to 0 "
		        << "('callback_n_trials_limit_achieved.size()' = "
		        << this->callback_n_trials_limit_achieved.size() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	auto m = new Monitor_MI(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R>
bool Monitor_MI<B,R>
::equivalent(const Monitor_MI<B,R>& m, bool do_throw) const
{
	if (get_N() != m.get_N())
	{
		if (!do_throw)
			return false;

		std::stringstream message;
		message << "'get_N()' is different than 'm.get_N()' ('get_N()' = " << get_N() << ", 'm.get_N()' = "
		        << m.get_N() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (get_max_n_trials() != m.get_max_n_trials())
	{
		if (!do_throw)
			return false;

		std::stringstream message;
		message << "'get_max_n_trials()' is different than 'm.get_max_n_trials()' ('get_max_n_trials()' = "
		        << get_max_n_trials() << ", 'm.get_max_n_trials()' = " << m.get_max_n_trials() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	return true;
}

template <typename B, typename R>
R Monitor_MI<B,R>
::get_mutual_info(const B *X, const R *Y, const int frame_id, const bool managed_memory)
{
	(*this)[mnt::sck::get_mutual_info::X].bind(X);
	(*this)[mnt::sck::get_mutual_info::Y].bind(Y);
	const auto &status = (*this)[mnt::tsk::get_mutual_info].exec(frame_id, managed_memory);

	return (R)status[0];
}

template <typename B, typename R>
R Monitor_MI<B,R>
::_get_mutual_info(const B *X, const R *Y, const size_t frame_id)
{
	R loc_MI_sum = 0;
	for (size_t f = 0; f < this->get_n_frames(); f++)
		loc_MI_sum += this->__get_mutual_info(X + f * get_N(), Y + f * get_N(), f);

	this->callback_check.notify();

	if (this->n_trials_limit_achieved())
		this->callback_n_trials_limit_achieved.notify();

	return loc_MI_sum / (R)(this->get_n_frames()) * 10000; // return the mut info computed now %10000 instead of %100
}

template <typename B, typename R>
R Monitor_MI<B,R>
::__get_mutual_info(const B *X, const R *Y, const size_t frame_id)
{
	throw tools::runtime_error(__FILE__, __LINE__, __func__, "The _get_mutual_info() function does not support this "
	                                                         "type.");
}

#include "Tools/types.h"
namespace aff3ct
{
namespace module
{
#if defined(AFF3CT_MULTI_PREC) | defined (AFF3CT_32BIT_PREC)
template <>
R_32 Monitor_MI<B_32,R_32>
::__get_mutual_info(const B_32 *X, const R_32 *Y, const size_t frame_id)
{
	auto mi = tools::mutual_info_histo(X, Y, get_N());
	this->add_MI_value(mi);
	return mi;
}
#endif

#if defined(AFF3CT_MULTI_PREC) | defined (AFF3CT_64BIT_PREC)
template <>
R_64 Monitor_MI<B_64,R_64>
::__get_mutual_info(const B_64 *X, const R_64 *Y, const size_t frame_id)
{
	auto mi = tools::mutual_info_histo(X, Y, get_N());
	this->add_MI_value(mi);
	return mi;
}
#endif
}
}

template <typename B, typename R>
void Monitor_MI<B,R>
::add_MI_value(const R mi)
{
	vals.n_trials++;
	vals.MI += (mi - vals.MI) / (R)vals.n_trials;

	vals.MI_max = std::max(vals.MI_max, mi);
	vals.MI_min = std::min(vals.MI_min, mi);

	if (mutinfo_hist_activated)
		this->mutinfo_hist.add_value(mi);
}

template <typename B, typename R>
bool Monitor_MI<B,R>
::n_trials_limit_achieved() const
{
	return get_max_n_trials() != 0 && get_n_trials() >= get_max_n_trials();
}

template <typename B, typename R>
bool Monitor_MI<B,R>
::is_done() const
{
	return n_trials_limit_achieved();
}

template <typename B, typename R>
const typename Monitor_MI<B,R>::Attributes& Monitor_MI<B,R>
::get_attributes() const
{
	return vals;
}

template <typename B, typename R>
int Monitor_MI<B,R>
::get_N() const
{
	return N;
}

template <typename B, typename R>
unsigned Monitor_MI<B,R>
::get_max_n_trials() const
{
	return max_n_trials;
}

template <typename B, typename R>
unsigned long long Monitor_MI<B,R>
::get_n_trials() const
{
	return vals.n_trials;
}

template <typename B, typename R>
R Monitor_MI<B,R>
::get_MI() const
{
	return vals.MI;
}

template <typename B, typename R>
R Monitor_MI<B,R>
::get_MI_min() const
{
	return vals.MI_min;
}

template <typename B, typename R>
R Monitor_MI<B,R>
::get_MI_max() const
{
	return vals.MI_max;
}

template<typename B, typename R>
tools::Histogram<R> Monitor_MI<B,R>
::get_mutinfo_hist() const
{
	return this->mutinfo_hist;
}

template<typename B, typename R>
void  Monitor_MI<B,R>
::activate_mutinfo_histogram(bool val)
{
	mutinfo_hist_activated = val;
}


template <typename B, typename R>
uint32_t Monitor_MI<B,R>
::record_callback_check(std::function<void(void)> callback)
{
	return this->callback_check.record(callback);
}

template <typename B, typename R>
uint32_t Monitor_MI<B,R>
::record_callback_n_trials_limit_achieved(std::function<void(void)> callback)
{
	return this->callback_n_trials_limit_achieved.record(callback);
}

template <typename B, typename R>
bool Monitor_MI<B,R>
::unrecord_callback_check(const uint32_t id)
{
	return this->callback_check.unrecord(id);
}

template <typename B, typename R>
bool Monitor_MI<B,R>
::unrecord_callback_n_trials_limit_achieved(const uint32_t id)
{
	return this->callback_n_trials_limit_achieved.unrecord(id);
}

template <typename B, typename R>
void Monitor_MI<B,R>
::reset()
{
	Monitor::reset();
	vals.reset();

	this->mutinfo_hist.reset();
}

template <typename B, typename R>
void Monitor_MI<B,R>
::clear_callbacks()
{
	this->callback_check.clear();
	this->callback_n_trials_limit_achieved.clear();
}

template <typename B, typename R>
void Monitor_MI<B,R>
::collect(const Monitor& m, bool fully)
{
	collect(static_cast<const Monitor_MI<B,R>&>(m), fully);
}

template <typename B, typename R>
void Monitor_MI<B,R>
::collect(const Monitor_MI<B,R>& m, bool fully)
{
	collect(m.get_attributes());
	if (fully)
		this->mutinfo_hist.add_values(m.mutinfo_hist);
}

template <typename B, typename R>
void Monitor_MI<B,R>
::collect(const Attributes& v)
{
	vals += v;
}

template <typename B, typename R>
Monitor_MI<B,R>& Monitor_MI<B,R>
::operator+=(const Monitor_MI<B,R>& m)
{
	collect(m, false);
	return *this;
}

template <typename B, typename R>
void Monitor_MI<B,R>
::copy(const Monitor& m, bool fully)
{
	copy(static_cast<const Monitor_MI<B,R>&>(m), fully);
}

template <typename B, typename R>
void Monitor_MI<B,R>
::copy(const Monitor_MI<B,R>& m, bool fully)
{
	copy(m.get_attributes());
	if (fully)
		this->mutinfo_hist = m.mutinfo_hist;
}

template <typename B, typename R>
void Monitor_MI<B,R>
::copy(const Attributes& v)
{
	vals = v;
}

template <typename B, typename R>
Monitor_MI<B,R>& Monitor_MI<B,R>
::operator=(const Monitor_MI<B,R>& m)
{
	copy(m, false);
	return *this;
}

template <typename B, typename R>
typename Monitor_MI<B,R>::Attributes& Monitor_MI<B,R>::Attributes
::operator+=(const Attributes& v)
{
	n_trials += v.n_trials;

	if (n_trials != 0)
		MI += (v.MI - MI) * ((R)v.n_trials / (R)n_trials);
	else
		MI = (R)0.;

	MI_max = std::max(MI_max, v.MI_max);
	MI_min = std::min(MI_min, v.MI_min);
	return *this;
}

template <typename B, typename R>
void Monitor_MI<B,R>::Attributes
::reset()
{
	n_trials  = 0;
	MI        = 0.;
	MI_max    = 0.;
	MI_min    = 1.;
}

template <typename B, typename R>
Monitor_MI<B,R>::Attributes
::Attributes()
{
	reset();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Monitor_MI<B_8, R_8>;
template class aff3ct::module::Monitor_MI<B_16,R_16>;
template class aff3ct::module::Monitor_MI<B_32,R_32>;
template class aff3ct::module::Monitor_MI<B_64,R_64>;
#else
template class aff3ct::module::Monitor_MI<B,R>;
#endif
// ==================================================================================== explicit template instantiation
