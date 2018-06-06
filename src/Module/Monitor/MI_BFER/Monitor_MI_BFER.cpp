#include <string>
#include <vector>
#include <stdexcept>

#include "Monitor_MI_BFER.hpp"
#include "Tools/Perf/common/mutual_info.h"
#include "Tools/Perf/common/hamming_distance.h"
#include "Tools/Math/utils.h"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Monitor_MI_BFER<B,R>
::Monitor_MI_BFER(const int K, const int N,
                  const unsigned mi_max_n_cf,
                  const unsigned er_max_fe, const bool er_count_unknown_values,
                  const int n_frames)
: Monitor        (                                       n_frames),
  Monitor_MI<B,R>(N, mi_max_n_cf,                        n_frames),
  Monitor_BFER<B>(K, er_max_fe, er_count_unknown_values, n_frames),
  vals(Monitor_MI<B,R>::vals, Monitor_BFER<B>::vals)
{
	const std::string name = "Monitor_MI_BFER";
	this->set_name(name);

	// auto &p1 = this->create_task("check_errors", (int)mnt::tsk::check_errors);
	// auto &ps1_U = this->template create_socket_in<B>(p1, "U", this->K * this->n_frames);
	// auto &ps1_V = this->template create_socket_in<B>(p1, "V", this->K * this->n_frames);
	// this->create_codelet(p1, [this, &ps1_U, &ps1_V]() -> int
	// {
	// 	return this->check_errors(static_cast<B*>(ps1_U.get_dataptr()),
	// 	                          static_cast<B*>(ps1_V.get_dataptr()));
	// });

	// auto &p2 = this->create_task("get_mutual_info", (int)mnt_mi::tsk::get_mutual_info);
	// auto &ps2_X = this->template create_socket_in<B>(p2, "X", this->N * this->n_frames);
	// auto &ps2_Y = this->template create_socket_in<R>(p2, "Y", this->N * this->n_frames);
	// this->create_codelet(p2, [this, &ps2_X, &ps2_Y]() -> int
	// {
	// 	return this->get_mutual_info(static_cast<B*>(ps2_X.get_dataptr()),
	// 	                             static_cast<R*>(ps2_Y.get_dataptr()));
	// });
}

template <typename B, typename R>
Monitor_MI_BFER<B,R>
::Monitor_MI_BFER(const Monitor_MI_BFER<B,R>& mon, const int n_frames)
: Monitor_MI_BFER<B,R>(mon.K, mon.N, mon.max_n_trials, mon.max_fe, mon.count_unknown_values,
                       n_frames == -1 ? mon.n_frames : n_frames)
{
}

template <typename B, typename R>
Monitor_MI_BFER<B,R>
::Monitor_MI_BFER()
: Monitor        (         1),
  Monitor_MI<B,R>(0, 0,    1),
  Monitor_BFER<B>(0, 0, 0, 1),
  vals(Monitor_MI<B,R>::vals, Monitor_BFER<B>::vals)
{
}

template <typename B, typename R>
void Monitor_MI_BFER<B,R>
::add_handler_check(std::function<void(void)> callback)
{
	Monitor_MI<B,R>::add_handler_check(callback);
	Monitor_BFER<B>::add_handler_check(callback);
}

template <typename B, typename R>
void Monitor_MI_BFER<B,R>
::reset()
{
	Monitor_MI<B,R>::reset();
	Monitor_BFER<B>::reset();
}

template <typename B, typename R>
void Monitor_MI_BFER<B,R>
::clear_callbacks()
{
	Monitor_MI<B,R>::clear_callbacks();
	Monitor_BFER<B>::clear_callbacks();
}

template <typename B, typename R>
void Monitor_MI_BFER<B,R>
::collect(const Monitor& m)
{
	collect(dynamic_cast<const Monitor_MI_BFER<B,R>&>(m));
}

template <typename B, typename R>
void Monitor_MI_BFER<B,R>
::collect(const Monitor_MI_BFER<B,R>& m)
{
	Monitor_MI<B,R>::collect(m);
	Monitor_BFER<B>::collect(m);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Monitor_MI_BFER<B_8, R_8>;
template class aff3ct::module::Monitor_MI_BFER<B_16,R_16>;
template class aff3ct::module::Monitor_MI_BFER<B_32,R_32>;
template class aff3ct::module::Monitor_MI_BFER<B_64,R_64>;
#else
template class aff3ct::module::Monitor_MI_BFER<B,R>;
#endif
// ==================================================================================== explicit template instantiation
