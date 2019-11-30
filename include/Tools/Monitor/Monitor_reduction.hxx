#include <sstream>
#include <numeric>

#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Monitor/Monitor_reduction.hpp"

namespace aff3ct
{
namespace tools
{

template <class M>
Monitor_reduction<M>
::Monitor_reduction(const std::vector<M*>& _monitors)
: Monitor_reduction_static(),
  M((_monitors.size() && _monitors.front()) ? *_monitors.front() : M(),
	std::accumulate(_monitors.begin(), _monitors.end(), 0,
                    [](int tot, const M *m) { return tot + m->get_n_frames(); })),
  monitors(_monitors),
  collecter(*this)
{
	if (this->monitors.size() == 0)
	{
		std::stringstream message;
		message << "'monitors.size()' has to be greater than 0 ('monitors.size()' = " << monitors.size() << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	const std::string name = "Monitor_reduction<" + monitors[0]->get_name() + ">";
	this->set_name(name);

	for (size_t m = 0; m < this->monitors.size(); m++)
	{
		if (this->monitors[m] == nullptr)
		{
			std::stringstream message;
			message << "'monitors[" << m << "]' can't be null.";
			throw tools::logic_error(__FILE__, __LINE__, __func__, message.str());
		}

		try
		{
			this->monitors[0]->equivalent(*this->monitors[m], true);
		}
		catch(tools::exception& e)
		{
			std::stringstream message;

			auto save = tools::exception::no_backtrace;

			tools::exception::no_backtrace = true;
			message << "'monitors[0]' and 'monitors[" << m << "]' have to be equivalent: " << e.what();
			tools::exception::no_backtrace = save;

			throw tools::logic_error(__FILE__, __LINE__, __func__, message.str());

		}
	}
}

template <class M>
Monitor_reduction<M>
::Monitor_reduction(const std::vector<std::unique_ptr<M>>& _monitors)
: Monitor_reduction(convert_to_ptr<M>(_monitors))
{
}

template <class M>
Monitor_reduction<M>
::Monitor_reduction(const std::vector<std::shared_ptr<M>>& _monitors)
: Monitor_reduction(convert_to_ptr<M>(_monitors))
{
}

template <class M>
void Monitor_reduction<M>
::reset()
{
	M::reset();
	for (auto& m : this->monitors)
		m->reset();
}

template <class M>
bool Monitor_reduction<M>
::_is_done()
{
	// only the master thread can do this
	if ((std::this_thread::get_id() == Monitor_reduction_static::master_thread_id &&
	    (std::chrono::steady_clock::now() - Monitor_reduction_static::t_last_reduction) >=
	     Monitor_reduction_static::d_reduce_frequency))
	{
		this->reduce(false);
		Monitor_reduction_static::t_last_reduction = std::chrono::steady_clock::now();
	}

	return M::is_done();
}

template <class M>
void Monitor_reduction<M>
::reduce(bool fully)
{
	collecter.reset();
	for (auto& m : this->monitors)
		collecter.collect(*m, fully);
	M::copy(collecter, fully);
}

}
}
