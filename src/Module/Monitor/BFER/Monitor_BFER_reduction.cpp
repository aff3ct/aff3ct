#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>
#include <fstream>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Monitor_BFER_reduction.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Monitor_BFER_reduction<B>
::Monitor_BFER_reduction(const int size, const unsigned max_fe, std::vector<Monitor_BFER<B>*> monitors,
                         const int n_frames)
: Monitor_BFER<B>           (size, max_fe, n_frames),
  n_analyzed_frames_historic(0                     ),
  monitors                  (monitors              )
{
	const std::string name = "Monitor_BFER_reduction";
	this->set_name(name);
	
	if (monitors.size() == 0)
	{
		std::stringstream message;
		message << "'monitors.size()' has to be greater than 0 ('monitors.size()' = " << monitors.size() << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	for (size_t i = 0; i < monitors.size(); ++i)
		if (monitors[i] == nullptr)
			throw tools::logic_error(__FILE__, __LINE__, __func__, "'monitors[i]' can't be null.");
}

template <typename B>
Monitor_BFER_reduction<B>
::~Monitor_BFER_reduction()
{
}

template <typename B>
unsigned long long Monitor_BFER_reduction<B>
::get_n_analyzed_fra() const
{
	unsigned long long cur_fra = this->n_analyzed_frames;
	for (unsigned i = 0; i < monitors.size(); i++)
		cur_fra += monitors[i]->get_n_analyzed_fra();

	return cur_fra;
}

template <typename B>
unsigned long long Monitor_BFER_reduction<B>
::get_n_analyzed_fra_historic() const
{
	return n_analyzed_frames_historic;
}

template <typename B>
unsigned long long Monitor_BFER_reduction<B>
::get_n_fe() const
{
	auto cur_fe = this->n_frame_errors;
	for (unsigned i = 0; i < monitors.size(); i++)
		cur_fe += monitors[i]->get_n_fe();

	return cur_fe;
}

template <typename B>
unsigned long long Monitor_BFER_reduction<B>
::get_n_be() const
{
	auto cur_be = this->n_bit_errors;
	for (unsigned i = 0; i < monitors.size(); i++)
		cur_be += monitors[i]->get_n_be();

	return cur_be;
}

template <typename B>
void Monitor_BFER_reduction<B>
::reset()
{
	n_analyzed_frames_historic += this->get_n_analyzed_fra();
	Monitor_BFER<B>::reset();
	for (auto m : monitors)
		m->reset();
}

template <typename B>
void Monitor_BFER_reduction<B>
::clear_callbacks()
{
	Monitor_BFER<B>::clear_callbacks();
	for (auto m : monitors)
		m->clear_callbacks();
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Monitor_BFER_reduction<B_8>;
template class aff3ct::module::Monitor_BFER_reduction<B_16>;
template class aff3ct::module::Monitor_BFER_reduction<B_32>;
template class aff3ct::module::Monitor_BFER_reduction<B_64>;
#else
template class aff3ct::module::Monitor_BFER_reduction<B>;
#endif
// ==================================================================================== explicit template instantiation
