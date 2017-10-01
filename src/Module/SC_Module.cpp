#ifdef SYSTEMC_MODULE

#include "Module.hpp"
#include "SC_Module.hpp"

#include "Tools/general_utils.h"

using namespace aff3ct;
using namespace aff3ct::module;

SC_Module::SC_Module(Task &task, sc_core::sc_module_name sc_name)
: sc_module(sc_name),
  task(task),
  s_in (sockets_in, task),
  s_out(sockets_out, task)
{
	task.set_autoexec (false);
	task.set_autoalloc(true );

	auto is_inputs = false;
	for (auto &s : task.socket)
		if (task.get_socket_type(s) == IN || task.get_socket_type(s) == IN_OUT)
		{
			is_inputs = true;
			break;
		}

	for (auto &s : task.socket)
	{
		auto name = s.get_name();
		switch (task.get_socket_type(s))
		{
		case IN:
			sockets_in[name] = new tlm_utils::simple_target_socket<SC_Module>(name.c_str());
			ptr_input_sockets.push_back(&s);
			switch (ptr_input_sockets.size())
			{
				case 1: sockets_in[name]->register_b_transport(this, &SC_Module::b_transport1); break;
				case 2: sockets_in[name]->register_b_transport(this, &SC_Module::b_transport2); break;
				case 3: sockets_in[name]->register_b_transport(this, &SC_Module::b_transport3); break;
				case 4: sockets_in[name]->register_b_transport(this, &SC_Module::b_transport4); break;
				case 5: sockets_in[name]->register_b_transport(this, &SC_Module::b_transport5); break;
				case 6: sockets_in[name]->register_b_transport(this, &SC_Module::b_transport6); break;
				case 7: sockets_in[name]->register_b_transport(this, &SC_Module::b_transport7); break;
				case 8: sockets_in[name]->register_b_transport(this, &SC_Module::b_transport8); break;
				default: throw tools::runtime_error(__FILE__, __LINE__, __func__, "This should never happen."); break;
			}
			break;

		case OUT:
			sockets_out[name] = new tlm_utils::simple_initiator_socket<SC_Module>(name.c_str());
			if (!is_inputs)
				SC_THREAD(start_sc_thread);
			break;

		case IN_OUT:
			sockets_in[name] = new tlm_utils::simple_target_socket<SC_Module>(name.c_str());
			ptr_input_sockets.push_back(&s);
			switch (ptr_input_sockets.size())
			{
				case 1: sockets_in[name]->register_b_transport(this, &SC_Module::b_transport1); break;
				case 2: sockets_in[name]->register_b_transport(this, &SC_Module::b_transport2); break;
				case 3: sockets_in[name]->register_b_transport(this, &SC_Module::b_transport3); break;
				case 4: sockets_in[name]->register_b_transport(this, &SC_Module::b_transport4); break;
				case 5: sockets_in[name]->register_b_transport(this, &SC_Module::b_transport5); break;
				case 6: sockets_in[name]->register_b_transport(this, &SC_Module::b_transport6); break;
				case 7: sockets_in[name]->register_b_transport(this, &SC_Module::b_transport7); break;
				case 8: sockets_in[name]->register_b_transport(this, &SC_Module::b_transport8); break;
				default: throw tools::runtime_error(__FILE__, __LINE__, __func__, "This should never happen."); break;
			}
			sockets_out[name] = new tlm_utils::simple_initiator_socket<SC_Module>(name.c_str());
			break;

		default:
			throw tools::runtime_error(__FILE__, __LINE__, __func__, "This should never happen.");
			break;
		}
	}
}

SC_Module::~SC_Module()
{
	for (auto s : sockets_in ) delete s.second;
	for (auto s : sockets_out) delete s.second;
}

void SC_Module::b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t, Socket &socket)
{
	if (trans.get_data_length() != socket.get_databytes())
	{
		std::stringstream message;
		message << "'trans.get_data_length()' has to be equal to 'socket.->databytes' ("
		        << "'trans.get_data_length()' = " << trans.get_data_length()
		        << ", 'socket.->databytes' = " << socket.get_databytes()
		        << ", 'socket.->name' = " << socket.get_name()
		        << ", 'task.name' = " << task.get_name()
		        << ", 'task.module.name' = " << task.get_module().get_name() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (trans.get_data_ptr() == nullptr)
	{
		std::stringstream message;
		message << "'trans.get_data_ptr()' can't be NULL.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	socket.bind((void*)trans.get_data_ptr());
}

void SC_Module::b_transport_and_exec(tlm::tlm_generic_payload& trans, sc_core::sc_time& t, Socket &socket)
{
	b_transport(trans, t, socket);

	task.exec();

	for (auto &s : sockets_out)
	{
		auto name = tools::string_split(s.second->name(), '.')[1];
		auto &ts = task[name];

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((uint8_t*)ts.get_dataptr());
		payload.set_data_length(ts.get_databytes());

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		auto &ss = *s.second;
		ss->b_transport(payload, zero_time);
	}
}

void SC_Module::b_transport1(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
	if (ptr_input_sockets.size() == 1) b_transport_and_exec(trans, t, *ptr_input_sockets[0]);
	else                               b_transport         (trans, t, *ptr_input_sockets[0]);
}

void SC_Module::b_transport2(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
	if (ptr_input_sockets.size() == 2) b_transport_and_exec(trans, t, *ptr_input_sockets[1]);
	else                               b_transport         (trans, t, *ptr_input_sockets[1]);
}

void SC_Module::b_transport3(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
	if (ptr_input_sockets.size() == 3) b_transport_and_exec(trans, t, *ptr_input_sockets[2]);
	else                               b_transport         (trans, t, *ptr_input_sockets[2]);
}

void SC_Module::b_transport4(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
	if (ptr_input_sockets.size() == 4) b_transport_and_exec(trans, t, *ptr_input_sockets[3]);
	else                               b_transport         (trans, t, *ptr_input_sockets[3]);
}

void SC_Module::b_transport5(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
	if (ptr_input_sockets.size() == 5) b_transport_and_exec(trans, t, *ptr_input_sockets[4]);
	else                               b_transport         (trans, t, *ptr_input_sockets[4]);
}

void SC_Module::b_transport6(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
	if (ptr_input_sockets.size() == 6) b_transport_and_exec(trans, t, *ptr_input_sockets[5]);
	else                               b_transport         (trans, t, *ptr_input_sockets[5]);
}

void SC_Module::b_transport7(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
	if (ptr_input_sockets.size() == 7) b_transport_and_exec(trans, t, *ptr_input_sockets[6]);
	else                               b_transport         (trans, t, *ptr_input_sockets[6]);
}

void SC_Module::b_transport8(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
	if (ptr_input_sockets.size() == 8) b_transport_and_exec(trans, t, *ptr_input_sockets[7]);
	else                               b_transport         (trans, t, *ptr_input_sockets[7]);
}

void SC_Module::start_sc_thread()
{
	// infinite loop, can be stopped by calling 'sc_core::sc_stop()'
	while (true)
	{
		if (task.is_debug())
		{
			if (!task.get_n_calls())
				std::cout << "#" << std::endl;

			std::cout << "# -------------------------------" << std::endl;
			std::cout << "# New communication (n°" << task.get_n_calls() << ")" << std::endl;
			std::cout << "# -------------------------------" << std::endl;
			std::cout << "#" << std::endl;
		}

		task.exec();

		for (auto &s : sockets_out)
		{
			auto name = tools::string_split(s.second->name(), '.')[1];
			auto &ts = task[name];

			tlm::tlm_generic_payload payload;
			payload.set_data_ptr((uint8_t*)ts.get_dataptr());
			payload.set_data_length(ts.get_databytes());

			sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
			auto &ss = *s.second;
			ss->b_transport(payload, zero_time);
		}

		// required to give the hand to the SystemC scheduler (yield)
		sc_core::wait(0, sc_core::SC_MS);
	}
}

SC_Module_container::SC_Module_container(Module &module)
: module(module)
{
}

SC_Module_container::~SC_Module_container()
{
	for (auto m : sc_modules) delete m.second;
}

void SC_Module_container::create_module(const std::string name)
{
	if (module.tasks.find(name) != module.tasks.end())
	{
		sc_modules[name] = new SC_Module(*module.tasks[name], (module.get_name() + "::" +
		                                                       module.tasks[name]->get_name()).c_str());
	}
	else
	{
		std::stringstream message;
		message << "'name' does not exist ('name' = " << name << ", 'module.name' = " << module.get_name() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

void SC_Module_container::erase_module(const std::string name)
{
	if (sc_modules.find(name) != sc_modules.end())
	{
		delete sc_modules[name]; sc_modules[name] = nullptr;
		sc_modules.erase(sc_modules.find(name));
	}
	else
	{
		std::stringstream message;
		message << "'name' does not exist ('name' = " << name << ", 'module.name' = " << module.get_name() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

SC_Module& SC_Module_container::operator[](const std::string name)
{
	if (sc_modules.find(name) != sc_modules.end())
	{
		return *sc_modules[name];
	}
	else
	{
		std::stringstream message;
		message << "'name' does not exist ('name' = " << name << ", 'module.name' = " << module.get_name() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

#endif
