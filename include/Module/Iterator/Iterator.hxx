#include <chrono>
#include <thread>
#include <string>
#include <sstream>

#include "Module/Iterator/Iterator.hpp"

namespace aff3ct
{
namespace module
{

Task& Iterator
::operator[](const ite::tsk t)
{
	return Module::operator[]((size_t)t);
}

Socket& Iterator
::operator[](const ite::sck::iterate s)
{
	return Module::operator[]((size_t)ite::tsk::iterate)[(size_t)s];
}

template <class A>
void Iterator
::iterate(std::vector<int8_t,A>& out, const int frame_id, const bool managed_memory)
{
	(*this)[ite::sck::iterate::out].bind(out);
	(*this)[ite::tsk::iterate].exec(frame_id, managed_memory);
}

}
}
