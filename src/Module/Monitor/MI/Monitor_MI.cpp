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
::Monitor_MI(const int N, const unsigned max_n_trials, const int n_frames)
: Monitor(n_frames), N(N),
  max_n_trials(max_n_trials)
{
	const std::string name = "Monitor_MI";
	this->set_name(name);

	if (N <= 0)
	{
		std::stringstream message;
		message << "'N' has to be greater than 0 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p = this->create_task("get_mutual_info", (int)mnt::tsk::get_mutual_info);
	auto &ps_X = this->template create_socket_in<B>(p, "X", this->N * this->n_frames);
	auto &ps_Y = this->template create_socket_in<R>(p, "Y", this->N * this->n_frames);
	this->create_codelet(p, [this, &ps_X, &ps_Y]() -> int
	{
		return this->get_mutual_info(static_cast<B*>(ps_X.get_dataptr()),
		                             static_cast<R*>(ps_Y.get_dataptr()));
	});

	reset();
}

template <typename B, typename R>
Monitor_MI<B,R>
::Monitor_MI(const Monitor_MI<B,R>& mon, const int n_frames)
: Monitor_MI<B,R>(mon.N, mon.max_n_trials, n_frames == -1 ? mon.n_frames : n_frames)
{
}

template <typename B, typename R>
Monitor_MI<B,R>
::Monitor_MI()
: Monitor_MI<B,R>(1, 0)
{
}

template <typename B, typename R>
int Monitor_MI<B,R>
::get_N() const
{
	return N;
}

template <typename B, typename R>
bool Monitor_MI<B,R>
::equivalent(const Monitor_MI<B,R>& m, bool do_throw) const
{
	if (this->N != m.N)
	{
		if (!do_throw)
			return false;

		std::stringstream message;
		message << "'this->N' is different than 'm.N' ('this->N' = " << this->N << ", 'm.N' = "
		        << m.N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->max_n_trials != m.max_n_trials)
	{
		if (!do_throw)
			return false;

		std::stringstream message;
		message << "'this->max_n_trials' is different than 'm.max_n_trials' ('this->max_n_trials' = " << this->max_n_trials << ", 'm.max_n_trials' = "
		        << m.max_n_trials << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	return true;
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

	for (auto& c : this->callbacks_check)
		c();

	if (this->n_trials_limit_achieved())
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
namespace aff3ct
{
namespace module
{
#if defined(MULTI_PREC) | defined (PREC_32_BIT)
template <>
R_32 Monitor_MI<B_32,R_32>
::_get_mutual_info(const B_32 *X, const R_32 *Y, const int frame_id)
{
	auto mi = tools::mutual_info_histo(X, Y, this->N);
	this->add_MI_value(mi);
	return mi;
}
#endif

#if defined(MULTI_PREC) | defined (PREC_64_BIT)
template <>
R_64 Monitor_MI<B_64,R_64>
::_get_mutual_info(const B_64 *X, const R_64 *Y, const int frame_id)
{
	auto mi = tools::mutual_info_histo(X, Y, this->N);
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
	this->n_trials++;
	this->MI += (mi - this->MI) / (R)this->n_trials;

	this->MI_max = std::max(this->MI_max, mi);
	this->MI_min = std::min(this->MI_min, mi);

	this->mutinfo_hist.add_value(mi);
}

template <typename B, typename R>
bool Monitor_MI<B,R>
::n_trials_limit_achieved()
{
	return this->get_n_trials() >= this->get_n_trials_limit();
}

template <typename B, typename R>
unsigned Monitor_MI<B,R>
::get_n_trials_limit() const
{
	return max_n_trials;
}

template <typename B, typename R>
unsigned long long Monitor_MI<B,R>
::get_n_trials() const
{
	return this->n_trials;
}

template <typename B, typename R>
R Monitor_MI<B,R>
::get_MI() const
{
	return this->MI;
}

template <typename B, typename R>
R Monitor_MI<B,R>
::get_MI_min() const
{
	return this->MI_min;
}

template <typename B, typename R>
R Monitor_MI<B,R>
::get_MI_max() const
{
	return this->MI_max;
}

template<typename B, typename R>
tools::Histogram<R> Monitor_MI<B,R>::get_mutinfo_hist() const
{
	return this->mutinfo_hist;
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

	n_trials  = 0;
	MI        = 0.;
	MI_max    = 0.;
	MI_min    = 1.;

	this->mutinfo_hist.reset();
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
::collect(const Monitor& m, bool fully)
{
	collect(dynamic_cast<const Monitor_MI<B,R>&>(m), fully);
}

template <typename B, typename R>
void Monitor_MI<B,R>
::collect(const Monitor_MI<B,R>& m, bool fully)
{
	equivalent(m, true);

	n_trials += m.n_trials;
	MI       += (m.MI - MI) * ((R)m.n_trials / (R)n_trials);

	MI_max = std::max(MI_max, m.MI_max);
	MI_min = std::min(MI_min, m.MI_min);

	if (fully)
		this->mutinfo_hist.add_values(m.mutinfo_hist);
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
	copy(dynamic_cast<const Monitor_MI<B,R>&>(m), fully);
}

template <typename B, typename R>
void Monitor_MI<B,R>
::copy(const Monitor_MI<B,R>& m, bool fully)
{
	equivalent(m, true);

	n_trials = m.n_trials;
	MI       = m.MI;
	MI_max   = m.MI_max;
	MI_min   = m.MI_min;

	if (fully)
		this->mutinfo_hist = m.mutinfo_hist;
}

template <typename B, typename R>
Monitor_MI<B,R>& Monitor_MI<B,R>
::operator=(const Monitor_MI<B,R>& m)
{
	copy(m, false);
	return *this;
}

#ifdef ENABLE_MPI
template <typename B, typename R>
void Monitor_MI<B,R>
::create_MPI_struct(int          blen         [n_MPI_attributes],
                    MPI_Aint     displacements[n_MPI_attributes],
                    MPI_Datatype oldtypes     [n_MPI_attributes])
{
	using MPI_R_type = typename std::conditional<std::is_same<R,double>::value, MPI_DOUBLE, MPI_FLOAT>;

	blen[0] = 1; displacements[0] = offsetof(Monitor_MI<B,R>, n_trials); oldtypes[0] = MPI_UNSIGNED_LONG_LONG;
	blen[1] = 1; displacements[1] = offsetof(Monitor_MI<B,R>, MI      ); oldtypes[1] = MPI_R_type;
	blen[2] = 1; displacements[2] = offsetof(Monitor_MI<B,R>, MI_min  ); oldtypes[2] = MPI_R_type;
	blen[3] = 1; displacements[3] = offsetof(Monitor_MI<B,R>, MI_max  ); oldtypes[3] = MPI_R_type;
}
#endif

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
