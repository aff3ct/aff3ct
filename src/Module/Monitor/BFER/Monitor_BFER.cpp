#include <string>
#include <vector>
#include <stdexcept>

#include "Monitor_BFER.hpp"
#include "Tools/Perf/common/mutual_info.h"
#include "Tools/Perf/distance/hamming_distance.h"
#include "Tools/Math/utils.h"
#include "Tools/general_utils.h"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Monitor_BFER<B>
::Monitor_BFER(const int K, const unsigned max_fe, const unsigned max_n_frames,
               const bool count_unknown_values, const int n_frames)
: Monitor(n_frames), K(K), max_fe(max_fe), max_n_frames(max_n_frames),
  count_unknown_values(count_unknown_values), err_hist(0)
{
	const std::string name = "Monitor_BFER";
	this->set_name(name);

	if (K <= 0)
	{
		std::stringstream message;
		message << "'K' has to be greater than 0 ('K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p = this->create_task("check_errors", (int)mnt::tsk::check_errors);
	auto &ps_U = this->template create_socket_in<B>(p, "U", get_K() * get_n_frames());
	auto &ps_V = this->template create_socket_in<B>(p, "V", get_K() * get_n_frames());
	this->create_codelet(p, [this, &ps_U, &ps_V]() -> int
	{
		return this->check_errors(static_cast<B*>(ps_U.get_dataptr()),
		                          static_cast<B*>(ps_V.get_dataptr()));
	});

	reset();
}

template <typename B>
Monitor_BFER<B>
::Monitor_BFER(const Monitor_BFER<B>& mon, const int n_frames)
: Monitor_BFER<B>(mon.get_K(), mon.get_max_fe(), mon.get_max_n_frames(), mon.get_count_unknown_values(),
                  n_frames == -1 ? mon.get_n_frames() : n_frames)
{
}

template <typename B>
Monitor_BFER<B>
::Monitor_BFER()
: Monitor_BFER<B>(1, 0)
{
}


template <typename B>
bool Monitor_BFER<B>
::equivalent(const Monitor_BFER<B>& m, bool do_throw) const
{
	if (get_K() != m.get_K())
	{
		std::stringstream message;
		message << "'get_K()' is different than 'm.get_K()' ('get_K()' = " << get_K() << ", 'm.get_K()' = "
		        << m.get_K() <<").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (get_max_fe() != m.get_max_fe())
	{
		if (!do_throw)
			return false;

		std::stringstream message;
		message << "'get_max_fe()' is different than 'm.get_max_fe()' ('this->get_max_fe()' = " << get_max_fe() << ", 'm.get_max_fe()' = "
		        << m.get_max_fe() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (get_max_n_frames() != m.get_max_n_frames())
	{
		if (!do_throw)
			return false;

		std::stringstream message;
		message << "'get_max_n_frames()' is different than 'm.get_max_n_frames()' ('this->get_max_n_frames()' = " << get_max_n_frames() << ", 'm.get_max_n_frames()' = "
		        << m.get_max_n_frames() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (get_count_unknown_values() != m.get_count_unknown_values())
	{
		if (!do_throw)
			return false;

		std::stringstream message;
		message << "'get_count_unknown_values()' is different than 'm.get_count_unknown_values()' ('get_count_unknown_values()' = " << get_count_unknown_values() << ", 'm.get_count_unknown_values()' = "
		        << m.get_count_unknown_values() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	return true;
}

template <typename B>
int Monitor_BFER<B>
::check_errors(const B *U, const B *V, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % get_n_frames();
	const auto f_stop  = (frame_id < 0) ? get_n_frames() : f_start +1;

	int n_be = 0;
	for (auto f = f_start; f < f_stop; f++)
		n_be += this->_check_errors(U + f * get_K(),
		                            V + f * get_K(),
		                            f);

	for (auto& c : this->callbacks_check)
		c();

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

	if (get_count_unknown_values())
		bit_errors_count = (int)tools::hamming_distance_unk(U, V, get_K());
	else
		bit_errors_count = (int)tools::hamming_distance(U, V, get_K());

	if (bit_errors_count)
	{
		vals.n_be += bit_errors_count;
		vals.n_fe ++;

		err_hist.add_value(bit_errors_count);

		for (auto& c : this->callbacks_fe)
			c(bit_errors_count, frame_id);
	}

	vals.n_fra++;

	return bit_errors_count;
}

template <typename B>
bool Monitor_BFER<B>
::fe_limit_achieved() const
{
	return get_max_fe() != 0 && get_n_fe() >= get_max_fe();
}

template <typename B>
bool Monitor_BFER<B>
::frame_limit_achieved() const
{
	return get_max_n_frames() != 0 && get_n_analyzed_fra() >= get_max_n_frames();
}

template <typename B>
bool Monitor_BFER<B>
::is_done() const
{
	return fe_limit_achieved() || frame_limit_achieved();
}



template <typename B>
const typename Monitor_BFER<B>::Attributes& Monitor_BFER<B>
::get_attributes() const
{
	return vals;
}

template <typename B>
int Monitor_BFER<B>
::get_K() const
{
	return K;
}

template <typename B>
unsigned Monitor_BFER<B>
::get_max_n_frames() const
{
	return max_n_frames;
}

template <typename B>
unsigned Monitor_BFER<B>
::get_max_fe() const
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
	if (this->get_n_fe() != 0)
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

template<typename B>
bool Monitor_BFER<B>
::get_count_unknown_values() const
{
	return count_unknown_values;
}

template<typename B>
tools::Histogram<int> Monitor_BFER<B>::get_err_hist() const
{
	return err_hist;
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
	vals.reset();

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

template <typename B>
void Monitor_BFER<B>
::collect(const Monitor& m, bool fully)
{
	collect(dynamic_cast<const Monitor_BFER<B>&>(m), fully);
}

template <typename B>
void Monitor_BFER<B>
::collect(const Monitor_BFER<B>& m, bool fully)
{
	equivalent(m, true);

	collect(m.get_attributes());

	if (fully)
		this->err_hist.add_values(m.err_hist);
}

template <typename B>
void Monitor_BFER<B>
::collect(const Attributes& v)
{
	vals += v;
}

template <typename B>
Monitor_BFER<B>& Monitor_BFER<B>
::operator+=(const Monitor_BFER<B>& m)
{
	collect(m, false);
	return *this;
}



template <typename B>
void Monitor_BFER<B>
::copy(const Monitor& m, bool fully)
{
	copy(dynamic_cast<const Monitor_BFER<B>&>(m), fully);
}

template <typename B>
void Monitor_BFER<B>
::copy(const Monitor_BFER<B>& m, bool fully)
{
	equivalent(m, true);

	copy(m.get_attributes());

	if (fully)
		this->err_hist = m.err_hist;
}

template <typename B>
void Monitor_BFER<B>
::copy(const Attributes& v)
{
	vals = v;
}

template <typename B>
Monitor_BFER<B>& Monitor_BFER<B>
::operator=(const Monitor_BFER<B>& m)
{
	copy(m, false);
	return *this;
}


template <typename B>
typename Monitor_BFER<B>::Attributes& Monitor_BFER<B>::Attributes
::operator+=(const Attributes& a)
{
	n_be  += a.n_be;
	n_fe  += a.n_fe;
	n_fra += a.n_fra;

	return *this;
}

template <typename B>
void Monitor_BFER<B>::Attributes
::reset()
{
	n_be  = 0;
	n_fe  = 0;
	n_fra = 0;
}

template <typename B>
Monitor_BFER<B>::Attributes
::Attributes()
{
	reset();
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
