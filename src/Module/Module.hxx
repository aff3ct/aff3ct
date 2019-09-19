#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
template <typename T>
inline Socket& Module
::create_socket_in(Task& task, const std::string &name, const size_t n_elmts)
{
	return task.template create_socket_in<T>(name, n_elmts);
}

template <typename T>
inline Socket& Module
::create_socket_in_out(Task& task, const std::string &name, const size_t n_elmts)
{
	return task.template create_socket_in_out<T>(name, n_elmts);
}

template <typename T>
inline Socket& Module
::create_socket_out(Task& task, const std::string &name, const size_t n_elmts)
{
	return task.template create_socket_out<T>(name, n_elmts);
}
}
}
