#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Module.hpp"
#include "Module/Monitor/MI/Monitor_MI.hpp"

namespace aff3ct
{
namespace module
{

template <typename B, typename R>
Task& Monitor_MI<B,R>
::operator[](const mnt::tsk t)
{
	return Module::operator[]((size_t)t);
}

template <typename B, typename R>
Socket& Monitor_MI<B,R>
::operator[](const mnt::sck::get_mutual_info s)
{
	return Module::operator[]((size_t)mnt::tsk::get_mutual_info)[(size_t)s];
}

template <typename B, typename R>
template <class AB, class AR>
R Monitor_MI<B,R>
::get_mutual_info(const std::vector<B,AB>& X, const std::vector<R,AR>& Y, const int frame_id, const bool managed_memory)
{
	(*this)[mnt::sck::get_mutual_info::X].bind(X);
	(*this)[mnt::sck::get_mutual_info::Y].bind(Y);
	const auto &status = (*this)[mnt::tsk::get_mutual_info].exec(frame_id, managed_memory);

	return (R)status[0];
}
}
}
