#include <sstream>
#include <numeric>

#include "Tools/Exception/exception.hpp"
#include "Module/Monitor/Monitor_reduction.hpp"

namespace aff3ct
{
namespace module
{

template <class M>
Monitor_reduction_M<M>
::Monitor_reduction_M(const std::vector<std::unique_ptr<M>>& _monitors)
: Monitor_reduction(),
  M((_monitors.size() && _monitors.front()) ? *_monitors.front() : M(),
	std::accumulate(_monitors.begin(), _monitors.end(), 0,
                    [](int tot, const std::unique_ptr<M>& m) { return tot + m->get_n_frames(); })),
  monitors(_monitors),
  collecter(*this)
{
	if (this->monitors.size() == 0)
	{
		std::stringstream message;
		message << "'monitors.size()' has to be greater than 0 ('monitors.size()' = " << monitors.size() << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	const std::string name = "Monitor_reduction_M<" + monitors[0]->get_name() + ">";
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
void Monitor_reduction_M<M>
::reset()
{
	M::reset();

	for (auto& m : this->monitors)
		m->reset();
}

template <class M>
void Monitor_reduction_M<M>
::reset_mr()
{
	this->reset();
}

template <class M>
bool Monitor_reduction_M<M>
::is_done_mr()
{
	return M::is_done();
}

template <class M>
void Monitor_reduction_M<M>
::clear_callbacks()
{
	M::clear_callbacks();

	for (auto& m : monitors)
		m->clear_callbacks();
}

template <class M>
void Monitor_reduction_M<M>
::_reduce(bool fully)
{
	// Old slow way to collect data (with object allocation)
	// M collecter(*this);

	// for (auto& m : this->monitors)
	// 	collecter.collect(*m, fully);

	// M::copy(collecter, fully);

	// New way to collect data (without object allocation)
	collecter.reset();

	for (auto& m : this->monitors)
		collecter.collect(*m, fully);

	M::copy(collecter, fully);
}

}
}