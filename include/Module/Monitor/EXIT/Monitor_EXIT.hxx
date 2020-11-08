#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Module.hpp"
#include "Module/Monitor/EXIT/Monitor_EXIT.hpp"

namespace aff3ct
{
namespace module
{

template <typename B, typename R>
Task& Monitor_EXIT<B,R>
::operator[](const mnt::tsk t)
{
	return Module::operator[]((size_t)t);
}

template <typename B, typename R>
Socket& Monitor_EXIT<B,R>
::operator[](const mnt::sck::check_mutual_info s)
{
	return Module::operator[]((size_t)mnt::tsk::check_mutual_info)[(size_t)s];
}

template <typename B, typename R>
template <class AB, class AR>
void Monitor_EXIT<B,R>
::check_mutual_info(const std::vector<B,AB>& bits,
                    const std::vector<R,AR>& llrs_a,
                    const std::vector<R,AR>& llrs_e,
                    const int frame_id,
                    const bool managed_memory)
{
	(*this)[mnt::sck::check_mutual_info::bits].bind(bits);
	(*this)[mnt::sck::check_mutual_info::llrs_a].bind(llrs_a);
	(*this)[mnt::sck::check_mutual_info::llrs_e].bind(llrs_e);
	(*this)[mnt::tsk::check_mutual_info].exec(frame_id, managed_memory);
}
}
}
