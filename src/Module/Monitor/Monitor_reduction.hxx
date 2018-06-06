#ifndef MONITOR_REDUCTION_HXX_
#define MONITOR_REDUCTION_HXX_

#include <sstream>
#include <numeric>
#include "Tools/Exception/exception.hpp"

#include "Monitor_reduction.hpp"

namespace aff3ct
{
namespace module
{

template <class M>
Monitor_reduction<M>
::Monitor_reduction(const std::vector<M*> &monitors)
: M((monitors.size() && monitors.front()) ? *monitors.front() : M(),
     monitors.size() ? std::accumulate(monitors.begin(), monitors.end(), 0,
                                       [](int tot, const M* m) { return tot + m->get_n_frames(); })
                     : 1
   ),
  monitors(monitors)
{
	if (monitors.size() == 0)
	{
		std::stringstream message;
		message << "'monitors.size()' has to be greater than 0 ('monitors.size()' = " << monitors.size() << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	const std::string name = "Monitor_reduction<" + monitors[0]->get_name() + ">";
	this->set_name(name);

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
	}
}

template <class M>
void Monitor_reduction<M>
::reset()
{
	M::reset();

	for (auto& m : monitors)
		m->reset();
}

template <class M>
void Monitor_reduction<M>
::clear_callbacks()
{
	M::clear_callbacks();

	for (auto& m : monitors)
		m->clear_callbacks();
}

template <class M>
void Monitor_reduction<M>
::reduce()
{
	M::reset();

	for (auto& m : monitors)
		M::collect(*m);
}

}
}
#endif // MONITOR_REDUCTION_HXX_
