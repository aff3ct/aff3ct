#include <string>

#include "Module/Controller/Controller.hpp"

namespace aff3ct
{
namespace module
{

Task& Controller
::operator[](const ctr::tsk t)
{
	return Module::operator[]((size_t)t);
}

Socket& Controller
::operator[](const ctr::sck::control s)
{
	return Module::operator[]((size_t)ctr::tsk::control)[(size_t)s];
}

template <class A>
void Controller
::control(std::vector<int8_t,A>& out, const int frame_id, const bool managed_memory)
{
	(*this)[ctr::sck::control::out].bind(out);
	(*this)[ctr::tsk::control].exec(frame_id, managed_memory);
}

}
}
