#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Tools/Exception/exception.hpp"

#include "Monitor_BFER_reduction.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Monitor_BFER_reduction<B,R>
::Monitor_BFER_reduction(const std::vector<Monitor_BFER<B,R>*> &monitors)
: Monitor_BFER<B,R>((monitors.size() && monitors[0]) ? monitors[0]->get_K() : 1,
                    (monitors.size() && monitors[0]) ? monitors[0]->get_N() : 1,
                    (monitors.size() && monitors[0]) ? monitors[0]->get_fe_limit() : 1,
                    (monitors.size() && monitors[0]) ? monitors[0]->get_n_frames() : 1),
  n_analyzed_frames_historic(0),
  monitors(monitors)
{
	const std::string name = "Monitor_BFER_reduction";
	this->set_name(name);

	if (monitors.size() == 0)
	{
		std::stringstream message;
		message << "'monitors.size()' has to be greater than 0 ('monitors.size()' = " << monitors.size() << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	for (size_t m = 0; m < monitors.size(); m++)
	{
		if (monitors[m] == nullptr)
		{
			std::stringstream message;
			message << "'monitors[m]' can't be null ('m' = " << m << ").";
			throw tools::logic_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (monitors[0]->get_K() != monitors[m]->get_K())
		{
			std::stringstream message;
			message << "'monitors[0]->get_K()' and 'monitors[m]->get_K()' have to be equal ('m' = " << m
			        << ", 'monitors[0]->get_K()' = " << monitors[0]->get_K()
			        << ", 'monitors[m]->get_K()' = " << monitors[m]->get_K() << ").";
			throw tools::logic_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (monitors[0]->get_N() != monitors[m]->get_N())
		{
			std::stringstream message;
			message << "'monitors[0]->get_N()' and 'monitors[m]->get_N()' have to be equal ('m' = " << m
			        << ", 'monitors[0]->get_N()' = " << monitors[0]->get_N()
			        << ", 'monitors[m]->get_N()' = " << monitors[m]->get_N() << ").";
			throw tools::logic_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (monitors[0]->get_fe_limit() != monitors[m]->get_fe_limit())
		{
			std::stringstream message;
			message << "'monitors[0]->get_fe_limit()' and 'monitors[m]->get_fe_limit()' have to be equal ('m' = " << m
			        << ", 'monitors[0]->get_fe_limit()' = " << monitors[0]->get_fe_limit()
			        << ", 'monitors[m]->get_fe_limit()' = " << monitors[m]->get_fe_limit() << ").";
			throw tools::logic_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (monitors[0]->get_n_frames() != monitors[m]->get_n_frames())
		{
			std::stringstream message;
			message << "'monitors[0]->get_n_frames()' and 'monitors[m]->get_n_frames()' have to be equal ('m' = " << m
			        << ", 'monitors[0]->get_n_frames()' = " << monitors[0]->get_n_frames()
			        << ", 'monitors[m]->get_n_frames()' = " << monitors[m]->get_n_frames() << ").";
			throw tools::logic_error(__FILE__, __LINE__, __func__, message.str());
		}
	}
}

template <typename B, typename R>
unsigned long long Monitor_BFER_reduction<B,R>
::get_n_analyzed_fra() const
{
	unsigned long long cur_fra = this->n_analyzed_frames;
	for (unsigned i = 0; i < monitors.size(); i++)
		cur_fra += monitors[i]->get_n_analyzed_fra();

	return cur_fra;
}

template <typename B, typename R>
unsigned long long Monitor_BFER_reduction<B,R>
::get_n_analyzed_fra_historic() const
{
	return n_analyzed_frames_historic;
}

template <typename B, typename R>
unsigned long long Monitor_BFER_reduction<B,R>
::get_n_fe() const
{
	auto cur_fe = this->n_frame_errors;
	for (unsigned i = 0; i < monitors.size(); i++)
		cur_fe += monitors[i]->get_n_fe();

	return cur_fe;
}

template <typename B, typename R>
unsigned long long Monitor_BFER_reduction<B,R>
::get_n_be() const
{
	auto cur_be = this->n_bit_errors;
	for (unsigned i = 0; i < monitors.size(); i++)
		cur_be += monitors[i]->get_n_be();

	return cur_be;
}

template <typename B, typename R>
R Monitor_BFER_reduction<B,R>
::get_MI() const
{
	return this->get_MI_sum() / (R)this->get_n_analyzed_fra();
}

template <typename B, typename R>
R Monitor_BFER_reduction<B,R>
::get_MI_sum() const
{
	auto cur_mi = this->MI_sum;
	for (unsigned i = 0; i < monitors.size(); i++)
		cur_mi += monitors[i]->get_MI_sum();

	return cur_mi;
}

//template<typename B, typename R>
//const tools::Histogram<R> & Monitor_BFER_reduction<B, R>::get_llrs0()
//{
//	for (unsigned i = 0; i < monitors.size(); i++)
//		this->llrs0.add_values(monitors[i]->get_llrs0());
//
//	return this->llrs0;
//}
//
//template<typename B, typename R>
//const tools::Histogram<R>& Monitor_BFER_reduction<B, R>::get_llrs1()
//{
//	for (unsigned i = 0; i < monitors.size(); i++)
//		this->llrs1.add_values(monitors[i]->get_llrs1());
//
//	return this->llrs1;
//}

template <typename B, typename R>
void Monitor_BFER_reduction<B,R>
::reset()
{
	n_analyzed_frames_historic += this->get_n_analyzed_fra();
	Monitor_BFER<B,R>::reset();
	for (auto m : monitors)
		m->reset();
}

template <typename B, typename R>
void Monitor_BFER_reduction<B,R>
::clear_callbacks()
{
	Monitor_BFER<B,R>::clear_callbacks();
	for (auto m : monitors)
		m->clear_callbacks();
}

template<typename B, typename R>
tools::Histogram<int> Monitor_BFER_reduction<B, R>::get_err_hist() const
{
	auto err_hist_copy = this->err_hist;
	for (unsigned i = 0; i < monitors.size(); i++)
		err_hist_copy.add_values(monitors[i]->get_err_hist());

	return err_hist_copy;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Monitor_BFER_reduction<B_8, Q_8>;
template class aff3ct::module::Monitor_BFER_reduction<B_16,Q_16>;
template class aff3ct::module::Monitor_BFER_reduction<B_32,Q_32>;
template class aff3ct::module::Monitor_BFER_reduction<B_64,Q_64>;
#else
template class aff3ct::module::Monitor_BFER_reduction<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
