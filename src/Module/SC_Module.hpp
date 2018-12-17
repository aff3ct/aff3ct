#ifdef AFF3CT_SYSTEMC_MODULE

#ifndef SC_MODULE_HPP_
#define SC_MODULE_HPP_

#include <map>
#include <string>

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <type_traits>

#include "Task.hpp"

namespace aff3ct
{
namespace module
{
class SC_Module;

template <class C>
class SC_Socket_in
{
	friend SC_Module;
protected:
	std::vector<std::unique_ptr<tlm_utils::simple_target_socket<C>>> &sockets;
	const Task &task;
	const std::vector<int> &indirect_sockets;
	SC_Socket_in(std::vector<std::unique_ptr<tlm_utils::simple_target_socket<C>>> &sockets,
	             const Task &task, const std::vector<int> &indirect_sockets)
	: sockets(sockets), task(task), indirect_sockets(indirect_sockets) {}
public:
	virtual ~SC_Socket_in() = default;
	inline tlm_utils::simple_target_socket<C>& operator[](const int id)
	{
		return *sockets[indirect_sockets[id]];
	}
};

template <class C>
class SC_Socket_out
{
	friend SC_Module;
protected:
	std::vector<std::unique_ptr<tlm_utils::simple_initiator_socket<C>>> &sockets;
	const Task &task;
	const std::vector<int> &indirect_sockets;
	SC_Socket_out(std::vector<std::unique_ptr<tlm_utils::simple_initiator_socket<C>>> &sockets,
	              const Task &task, const std::vector<int> &indirect_sockets)
	: sockets(sockets), task(task), indirect_sockets(indirect_sockets) {}
public:
	virtual ~SC_Socket_out() = default;
	inline tlm_utils::simple_initiator_socket<C>& operator[](const int id)
	{
		return *sockets[indirect_sockets[id]];
	}
};

class SC_Module : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Module); // required because of the "SC_THREAD" call in the constructor

protected:
	Task &task;

	std::vector<std::unique_ptr<tlm_utils::simple_target_socket   <SC_Module>>> sockets_in;
	std::vector<std::unique_ptr<tlm_utils::simple_initiator_socket<SC_Module>>> sockets_out;

	std::vector<std::shared_ptr<Socket>> ptr_input_sockets;

	std::vector<int> indirect_sockets_in;
	std::vector<int> indirect_sockets_out;

	std::vector<int> indirect_sockets_in_rev;
	std::vector<int> indirect_sockets_out_rev;

public:
	SC_Socket_in <SC_Module> s_in;
	SC_Socket_out<SC_Module> s_out;

	SC_Module(Task &task, sc_core::sc_module_name sc_name);
	virtual ~SC_Module() = default;

protected:
	void b_transport         (tlm::tlm_generic_payload& trans, sc_core::sc_time& t, Socket &socket);
	void b_transport_and_exec(tlm::tlm_generic_payload& trans, sc_core::sc_time& t, Socket &socket);
	void start_sc_thread();

	void b_transport1(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);
	void b_transport2(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);
	void b_transport3(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);
	void b_transport4(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);
	void b_transport5(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);
	void b_transport6(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);
	void b_transport7(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);
	void b_transport8(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);
};

class SC_Module_container
{
protected:
	Module &module;
	std::vector<std::unique_ptr<SC_Module>> sc_modules;

public:
	explicit SC_Module_container(Module &module);
	virtual ~SC_Module_container() = default;

	void create_module(const int id);
	void erase_module(const int id);
	inline SC_Module& operator[](const int id)
	{
		return *sc_modules[id];
	}
};
}
}

#endif /* SC_MODULE_HPP_ */

#endif
