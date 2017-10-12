#ifdef SYSTEMC_MODULE

#ifndef SC_MODULE_HPP_
#define SC_MODULE_HPP_

#include <map>
#include <string>

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

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
	std::map<std::string, tlm_utils::simple_target_socket<C>*> &sockets;
	const Task &task;
	SC_Socket_in(std::map<std::string, tlm_utils::simple_target_socket<C>*> &sockets, const Task &task)
	: sockets(sockets), task(task) {}
public:
	virtual ~SC_Socket_in() {}
	inline tlm_utils::simple_target_socket<C>& operator[](const std::string key)
	{
		if (sockets.find(key) != sockets.end())
		{
			return *sockets.find(key)->second;
		}
		else
		{
			std::stringstream message;
			message << "The input socket does not exist ("
			        << "'key' = " << key
			        << ", 'task.name' = " << task.get_name()
			        << ", 'task.module.name' = " << task.get_module_name() << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}
	}
};

template <class C>
class SC_Socket_out
{
	friend SC_Module;
protected:
	std::map<std::string, tlm_utils::simple_initiator_socket<C>*> &sockets;
	const Task &task;
	SC_Socket_out(std::map<std::string, tlm_utils::simple_initiator_socket<C>*> &sockets, const Task &task)
	: sockets(sockets), task(task) {}
public:
	virtual ~SC_Socket_out() {}
	inline tlm_utils::simple_initiator_socket<C>& operator[](const std::string key)
	{
		if (sockets.find(key) != sockets.end())
		{
			return *sockets.find(key)->second;
		}
		else
		{
			std::stringstream message;
			message << "The output socket does not exist ("
			        << "'key' = " << key
			        << ", 'task.name' = " << task.get_name()
			        << ", 'task.module.name' = " << task.get_module_name() << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}
	}
};

class SC_Module : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Module); // required because of the "SC_THREAD" call in the constructor

protected:
	Task &task;

	std::map<std::string, tlm_utils::simple_target_socket   <SC_Module>*> sockets_in;
	std::map<std::string, tlm_utils::simple_initiator_socket<SC_Module>*> sockets_out;

	std::vector<Socket*> ptr_input_sockets;

public:
	SC_Socket_in <SC_Module> s_in;
	SC_Socket_out<SC_Module> s_out;

	SC_Module(Task &task, sc_core::sc_module_name sc_name);
	virtual ~SC_Module();

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
	std::map<std::string, SC_Module*> sc_modules;

public:
	SC_Module_container(Module &module);
	virtual ~SC_Module_container();

	void create_module   (const std::string name);
	void erase_module    (const std::string name);
	SC_Module& operator[](const std::string name);
};
}
}

#endif /* SC_MODULE_HPP_ */

#endif
