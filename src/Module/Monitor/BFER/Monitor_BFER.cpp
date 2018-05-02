#include <string>
#include <vector>
#include <stdexcept>

#include "Monitor_BFER.hpp"
#include "Tools/Perf/common/mutual_info.h"
#include "Tools/Perf/common/hamming_distance.h"
#include "Tools/Math/utils.h"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Monitor_BFER<B,R>
::Monitor_BFER(const int K, const int N, const unsigned max_fe, const int n_frames)
: Monitor(K, N, n_frames),
  max_fe(max_fe),
  n_bit_errors(0),
  n_frame_errors(0),
  n_analyzed_frames(0),
  MI_sum(0),
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

	auto &p2 = this->create_task("get_mutual_info", (int)mnt::tsk::get_mutual_info);
	auto &ps_X = this->template create_socket_in<B>(p2, "X", this->N * this->n_frames);
	auto &ps_Y = this->template create_socket_in<R>(p2, "Y", this->N * this->n_frames);
	this->create_codelet(p2, [this, &ps_X, &ps_Y]() -> int
	{
		return this->get_mutual_info(static_cast<B*>(ps_X.get_dataptr()),
		                             static_cast<R*>(ps_Y.get_dataptr()));
	});
}

template <typename B, typename R>
int Monitor_BFER<B,R>
::check_errors(const B *U, const B *V, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	int n_be = 0;
	for (auto f = f_start; f < f_stop; f++)
		n_be += this->_check_errors(U + f * this->K,
		                            V + f * this->K,
		                            f);

	return n_be;
}

template <typename B, typename R>
R Monitor_BFER<B,R>
::get_mutual_info(const B *X, const R *Y, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	R MI_sum = 0;
	for (auto f = f_start; f < f_stop; f++)
		MI_sum += this->_get_mutual_info(X + f * this->N,
		                                 Y + f * this->N,
		                                 f);

//	for(auto i = 0; i < this->n_frames*this->N ; i++)
//		if (X[i])
//			this->llrs1.add_value(Y[i]);
//		else
//			this->llrs0.add_value(Y[i]);

	return MI_sum / (R)this->n_frames * 10000; // return the mut info %10000 instead of %100
}

template <typename B, typename R>
int Monitor_BFER<B,R>
::_check_errors(const B *U, const B *V, const int frame_id)
{
	int bit_errors_count = tools::hamming_distance(U, V, this->K);

	if (bit_errors_count)
	{
		n_bit_errors += bit_errors_count;
		n_frame_errors++;

		err_hist.add_value(bit_errors_count);

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

template <typename B, typename R>
R Monitor_BFER<B,R>
::_get_mutual_info(const B *X, const R *Y, const int frame_id)
{
	throw tools::runtime_error(__FILE__, __LINE__, __func__, "The mutual_info_histo calculation does not support this type.");
}

#include "Tools/types.h"
#if defined(MULTI_PREC) | defined (PREC_32_BIT)
namespace aff3ct
{
namespace module
{
template <>
Q_32 Monitor_BFER<B_32,Q_32>
::_get_mutual_info(const B_32 *X, const Q_32 *Y, const int frame_id)
{
	auto mi = tools::mutual_info_histo(X, Y, this->N);
	MI_sum += mi;
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
Q_64 Monitor_BFER<B_64,Q_64>
::_get_mutual_info(const B_64 *X, const Q_64 *Y, const int frame_id)
{
	auto mi = tools::mutual_info_histo(X, Y, this->N);
	MI_sum += mi;
	return mi;
}
}
}
#endif

template <typename B, typename R>
bool Monitor_BFER<B,R>
::fe_limit_achieved()
{
	return (get_n_fe() >= get_fe_limit()) || Monitor::interrupt;
}

template <typename B, typename R>
unsigned Monitor_BFER<B,R>
::get_fe_limit() const
{
	return max_fe;
}

template <typename B, typename R>
unsigned long long Monitor_BFER<B,R>
::get_n_analyzed_fra() const
{
	return n_analyzed_frames;
}

template <typename B, typename R>
unsigned long long Monitor_BFER<B,R>
::get_n_fe() const
{
	return n_frame_errors;
}

template <typename B, typename R>
unsigned long long Monitor_BFER<B,R>
::get_n_be() const
{
	return n_bit_errors;
}

template <typename B, typename R>
float Monitor_BFER<B,R>
::get_fer() const
{
	auto t_fer = 0.f;
	if (this->get_n_be() != 0)
		t_fer = (float)this->get_n_fe() / (float)this->get_n_analyzed_fra();
	else
		t_fer = (1.f) / ((float)this->get_n_analyzed_fra());

	return t_fer;
}

template <typename B, typename R>
float Monitor_BFER<B,R>
::get_ber() const
{
	auto t_ber = 0.f;
	if (this->get_n_be() != 0)
		t_ber = (float)this->get_n_be() / (float)this->get_n_analyzed_fra() / (float)this->get_K();
	else
		t_ber = (1.f) / ((float)this->get_n_analyzed_fra()) / this->get_K();

	return t_ber;
}

template <typename B, typename R>
R Monitor_BFER<B,R>
::get_MI() const
{
	return this->MI_sum / (R)this->n_analyzed_frames;
}

template <typename B, typename R>
R Monitor_BFER<B,R>
::get_MI_sum() const
{
	return this->MI_sum;
}

template <typename B, typename R>
void Monitor_BFER<B,R>
::add_handler_fe(std::function<void(unsigned, int)> callback)
{
	this->callbacks_fe.push_back(callback);
}

template <typename B, typename R>
void Monitor_BFER<B,R>
::add_handler_check(std::function<void(void)> callback)
{
	this->callbacks_check.push_back(callback);
}

template <typename B, typename R>
void Monitor_BFER<B,R>
::add_handler_fe_limit_achieved(std::function<void(void)> callback)
{
	this->callbacks_fe_limit_achieved.push_back(callback);
}

template <typename B, typename R>
void Monitor_BFER<B,R>
::reset()
{
	Monitor::reset();

	this->n_bit_errors      = 0;
	this->n_frame_errors    = 0;
	this->n_analyzed_frames = 0;
	this->MI_sum            = 0;
//	this->llrs0.reset();
//	this->llrs1.reset();
	this->err_hist.reset();
}

template <typename B, typename R>
void Monitor_BFER<B,R>
::clear_callbacks()
{
	this->callbacks_fe               .clear();
	this->callbacks_check            .clear();
	this->callbacks_fe_limit_achieved.clear();
}

template<typename B, typename R>
tools::Histogram<int> Monitor_BFER<B, R>::get_err_hist() const
{
	return err_hist;
}

//template<typename B, typename R>
//const tools::Histogram<R>& Monitor_BFER<B, R>::get_llrs1()
//{
//	return llrs1;
//}
//
//template<typename B, typename R>
//const tools::Histogram<R>& Monitor_BFER<B, R>::get_llrs0()
//{
//	return llrs0;
//}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Monitor_BFER<B_8, Q_8>;
template class aff3ct::module::Monitor_BFER<B_16,Q_16>;
template class aff3ct::module::Monitor_BFER<B_32,Q_32>;
template class aff3ct::module::Monitor_BFER<B_64,Q_64>;
#else
template class aff3ct::module::Monitor_BFER<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
