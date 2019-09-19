#include "Module/SC_Module.hpp"

// convert strongly typed enum to integer
template <typename E>
constexpr typename std::underlying_type<E>::type operator+(E e) noexcept {
	return static_cast<typename std::underlying_type<E>::type>(e);
}

namespace aff3ct
{
namespace module
{
template <class C>
SC_Socket_in<C>
::SC_Socket_in(std::vector<std::unique_ptr<tlm_utils::simple_target_socket<C>>> &sockets, const Task &task,
               const std::vector<int> &indirect_sockets)
: sockets(sockets), task(task), indirect_sockets(indirect_sockets)
{
}

template <class C>
tlm_utils::simple_target_socket<C>& SC_Socket_in<C>
::operator[](const int id)
{
	return *sockets[indirect_sockets[id]];
}

template <class C>
SC_Socket_out<C>
::SC_Socket_out(std::vector<std::unique_ptr<tlm_utils::simple_initiator_socket<C>>> &sockets, const Task &task,
                const std::vector<int> &indirect_sockets)
: sockets(sockets), task(task), indirect_sockets(indirect_sockets)
{
}

template <class C>
tlm_utils::simple_initiator_socket<C>& SC_Socket_out<C>
::operator[](const int id)
{
	return *sockets[indirect_sockets[id]];
}

SC_Module& SC_Module_container
::operator[](const int id)
{
	return *sc_modules[id];
}
}
}
