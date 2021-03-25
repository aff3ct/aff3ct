#include "Module/Incrementer/Incrementer.hpp"

namespace aff3ct
{
namespace module
{

template <typename T>
Task& Incrementer<T>
::operator[](const inc::tsk t)
{
	return Module::operator[]((size_t)t);
}

template <typename T>
Socket& Incrementer<T>
::operator[](const inc::sck::increment s)
{
	return Module::operator[]((size_t)inc::tsk::increment)[(size_t)s];
}

template <typename T>
template <class A>
void Incrementer<T>
::increment(const std::vector<T,A>& in, std::vector<T,A>& out, const int frame_id, const bool managed_memory)
{
	(*this)[inc::sck::increment::in ].bind(in );
	(*this)[inc::sck::increment::out].bind(out);
	(*this)[inc::tsk::increment].exec(frame_id, managed_memory);
}

}
}
