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
::Monitor_MI_BFER(const int K, const int N, const unsigned max_n_cf,
                  const unsigned max_fe, const bool count_unknown_values, const int n_frames)
: Monitor        (K, N, n_frames),
  Monitor_MI<B,R>(K, N, max_n_cf, n_frames),
  Monitor_BFER<B>(K, N, max_fe, count_unknown_values, n_frames),
  vals(Monitor_MI<B,R>::vals, Monitor_BFER<B>::vals)
{
	const std::string name = "Monitor_MI_BFER";
	this->set_name(name);

	this->vals.reset();
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
