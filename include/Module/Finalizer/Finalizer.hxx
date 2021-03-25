#include "Module/Finalizer/Finalizer.hpp"

namespace aff3ct
{
namespace module
{

template <typename T>
Task& Finalizer<T>
::operator[](const fin::tsk t)
{
	return Module::operator[]((size_t)t);
}

template <typename T>
Socket& Finalizer<T>
::operator[](const fin::sck::finalize s)
{
	return Module::operator[]((size_t)fin::tsk::finalize)[(size_t)s];
}

template <typename T>
template <class A>
void Finalizer<T>
::finalize(const std::vector<T,A>& in, const int frame_id, const bool managed_memory)
{
	(*this)[fin::sck::finalize::in].bind(in);
	(*this)[fin::tsk::finalize].exec(frame_id, managed_memory);
}

}
}
