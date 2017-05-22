#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>
#include <fstream>
#include <exception>

#include "Monitor_reduction.hpp"

using namespace aff3ct::module;

template <typename B>
Monitor_reduction<B>
::Monitor_reduction(const int size, const unsigned max_fe, std::vector<Monitor<B>*> monitors, const int n_frames,
                    const std::string name)
: Monitor_std<B>            (size, max_fe, n_frames, name),
  n_analyzed_frames_historic(0                           ),
  monitors                  (monitors                    )
{
	if (monitors.size() == 0)
		throw std::length_error("aff3ct::module::Monitor_reduction: \"monitors.size()\" has to be greater than 0.");

	for (size_t i = 0; i < monitors.size(); ++i)
		if (monitors[i] == nullptr)
			throw std::logic_error("aff3ct::module::Monitor_reduction: \"monitors[i]\" can't be null.");
}

template <typename B>
Monitor_reduction<B>
::~Monitor_reduction()
{
}

template <typename B>
unsigned long long Monitor_reduction<B>
::get_n_analyzed_fra() const
{
	unsigned long long cur_fra = this->n_analyzed_frames;
	for (unsigned i = 0; i < monitors.size(); i++)
		cur_fra += monitors[i]->get_n_analyzed_fra();

	return cur_fra;
}

template <typename B>
unsigned long long Monitor_reduction<B>
::get_n_analyzed_fra_historic() const
{
	return n_analyzed_frames_historic;
}

template <typename B>
unsigned long long Monitor_reduction<B>
::get_n_fe() const
{
	auto cur_fe = this->n_frame_errors;
	for (unsigned i = 0; i < monitors.size(); i++)
		cur_fe += monitors[i]->get_n_fe();

	return cur_fe;
}

template <typename B>
unsigned long long Monitor_reduction<B>
::get_n_be() const
{
	auto cur_be = this->n_bit_errors;
	for (unsigned i = 0; i < monitors.size(); i++)
		cur_be += monitors[i]->get_n_be();

	return cur_be;
}

template <typename B>
void Monitor_reduction<B>
::reset()
{
	n_analyzed_frames_historic += this->get_n_analyzed_fra();
	Monitor_std<B>::reset();
	for (auto m : monitors)
		m->reset();
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Monitor_reduction<B_8>;
template class aff3ct::module::Monitor_reduction<B_16>;
template class aff3ct::module::Monitor_reduction<B_32>;
template class aff3ct::module::Monitor_reduction<B_64>;
#else
template class aff3ct::module::Monitor_reduction<B>;
#endif
// ==================================================================================== explicit template instantiation
