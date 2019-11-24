#ifdef AFF3CT_SYSTEMC_MODULE

#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/general_utils.h"
#include "Module/Module.hpp"
#include "Module/SC_Module.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

SC_Module
::SC_Module(Task &task, sc_core::sc_module_name sc_name)
: sc_module(sc_name),
  task(task),
  indirect_sockets_in (task.sockets.size()),
  indirect_sockets_out(task.sockets.size()),
  s_in (sockets_in,  task, indirect_sockets_in),
  s_out(sockets_out, task, indirect_sockets_out)
{
	task.set_autoexec (false);
	task.set_autoalloc(true );

	auto is_inputs = false;
	for (auto& s : task.sockets)
		if (task.get_socket_type(*s) == socket_t::SIN || task.get_socket_type(*s) == socket_t::SIN_SOUT)
		{
			is_inputs = true;
			break;
		}

	for (size_t i = 0; i < task.sockets.size(); i++)
	{
		auto& s = task.sockets[i];
		auto name = s->get_name();

		const auto id_in  = (int)sockets_in.size();
		const auto id_out = (int)sockets_out.size();
		switch (task.get_socket_type(*s))
		{
		case socket_t::SIN:
			indirect_sockets_in[i] = id_in;
			indirect_sockets_in_rev.push_back(i);
			sockets_in.push_back(std::unique_ptr<tlm_utils::simple_target_socket<SC_Module>>(
			                          new tlm_utils::simple_target_socket<SC_Module>(name.c_str())));
			ptr_input_sockets.push_back(s);
			switch (ptr_input_sockets.size())
			{
				case 1: sockets_in[id_in]->register_b_transport(this, &SC_Module::b_transport1); break;
				case 2: sockets_in[id_in]->register_b_transport(this, &SC_Module::b_transport2); break;
				case 3: sockets_in[id_in]->register_b_transport(this, &SC_Module::b_transport3); break;
				case 4: sockets_in[id_in]->register_b_transport(this, &SC_Module::b_transport4); break;
				case 5: sockets_in[id_in]->register_b_transport(this, &SC_Module::b_transport5); break;
				case 6: sockets_in[id_in]->register_b_transport(this, &SC_Module::b_transport6); break;
				case 7: sockets_in[id_in]->register_b_transport(this, &SC_Module::b_transport7); break;
				case 8: sockets_in[id_in]->register_b_transport(this, &SC_Module::b_transport8); break;
				default:
					throw tools::runtime_error(__FILE__, __LINE__, __func__, "This should never happen.");
					break;
			}
			break;

		case socket_t::SOUT:
			indirect_sockets_out[i] = id_out;
			indirect_sockets_out_rev.push_back(i);
			sockets_out.push_back(std::unique_ptr<tlm_utils::simple_initiator_socket<SC_Module>>(
			                          new tlm_utils::simple_initiator_socket<SC_Module>(name.c_str())));
			if (!is_inputs)
				SC_THREAD(start_sc_thread);
			break;

		case socket_t::SIN_SOUT:
			indirect_sockets_in[i] = id_in;
			indirect_sockets_in_rev.push_back(i);
			sockets_in.push_back(std::unique_ptr<tlm_utils::simple_target_socket<SC_Module>>(
			                          new tlm_utils::simple_target_socket<SC_Module>(name.c_str())));
			ptr_input_sockets.push_back(s);
			switch (ptr_input_sockets.size())
			{
				case 1: sockets_in[id_in]->register_b_transport(this, &SC_Module::b_transport1); break;
				case 2: sockets_in[id_in]->register_b_transport(this, &SC_Module::b_transport2); break;
				case 3: sockets_in[id_in]->register_b_transport(this, &SC_Module::b_transport3); break;
				case 4: sockets_in[id_in]->register_b_transport(this, &SC_Module::b_transport4); break;
				case 5: sockets_in[id_in]->register_b_transport(this, &SC_Module::b_transport5); break;
				case 6: sockets_in[id_in]->register_b_transport(this, &SC_Module::b_transport6); break;
				case 7: sockets_in[id_in]->register_b_transport(this, &SC_Module::b_transport7); break;
				case 8: sockets_in[id_in]->register_b_transport(this, &SC_Module::b_transport8); break;
				default:
					throw tools::runtime_error(__FILE__, __LINE__, __func__, "This should never happen.");
					break;
			}
			indirect_sockets_out[i] = id_out;
			indirect_sockets_out_rev.push_back(i);
			sockets_out.push_back(std::unique_ptr<tlm_utils::simple_initiator_socket<SC_Module>>(
			                          new tlm_utils::simple_initiator_socket<SC_Module>(name.c_str())));
			break;

		default:
			throw tools::runtime_error(__FILE__, __LINE__, __func__, "This should never happen.");
			break;
		}
	}
}

void SC_Module
::b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t, Socket &socket)
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

void SC_Module
::b_transport_and_exec(tlm::tlm_generic_payload& trans, sc_core::sc_time& t, Socket &socket)
{
	b_transport(trans, t, socket);

	task.exec();

	for (size_t i = 0; i < sockets_out.size(); i++)
	{
		auto &s = *sockets_out[i];
		auto &ts = task[indirect_sockets_out_rev[i]];

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((uint8_t*)ts.get_dataptr());
		payload.set_data_length(ts.get_databytes());

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s->b_transport(payload, zero_time);
	}
}

void SC_Module
::b_transport1(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
	if (ptr_input_sockets.size() == 1) b_transport_and_exec(trans, t, *ptr_input_sockets[0]);
	else                               b_transport         (trans, t, *ptr_input_sockets[0]);
}

void SC_Module
::b_transport2(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
	if (ptr_input_sockets.size() == 2) b_transport_and_exec(trans, t, *ptr_input_sockets[1]);
	else                               b_transport         (trans, t, *ptr_input_sockets[1]);
}

void SC_Module
::b_transport3(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
	if (ptr_input_sockets.size() == 3) b_transport_and_exec(trans, t, *ptr_input_sockets[2]);
	else                               b_transport         (trans, t, *ptr_input_sockets[2]);
}

void SC_Module
::b_transport4(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
	if (ptr_input_sockets.size() == 4) b_transport_and_exec(trans, t, *ptr_input_sockets[3]);
	else                               b_transport         (trans, t, *ptr_input_sockets[3]);
}

void SC_Module
::b_transport5(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
	if (ptr_input_sockets.size() == 5) b_transport_and_exec(trans, t, *ptr_input_sockets[4]);
	else                               b_transport         (trans, t, *ptr_input_sockets[4]);
}

void SC_Module
::b_transport6(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
	if (ptr_input_sockets.size() == 6) b_transport_and_exec(trans, t, *ptr_input_sockets[5]);
	else                               b_transport         (trans, t, *ptr_input_sockets[5]);
}

void SC_Module
::b_transport7(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
	if (ptr_input_sockets.size() == 7) b_transport_and_exec(trans, t, *ptr_input_sockets[6]);
	else                               b_transport         (trans, t, *ptr_input_sockets[6]);
}

void SC_Module
::b_transport8(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
	if (ptr_input_sockets.size() == 8) b_transport_and_exec(trans, t, *ptr_input_sockets[7]);
	else                               b_transport         (trans, t, *ptr_input_sockets[7]);
}

void SC_Module
::start_sc_thread()
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

		for (size_t i = 0; i < sockets_out.size(); i++)
		{
			auto &s = *sockets_out[i];
			auto &ts = task[indirect_sockets_out_rev[i]];

			tlm::tlm_generic_payload payload;
			payload.set_data_ptr((uint8_t*)ts.get_dataptr());
			payload.set_data_length(ts.get_databytes());

			sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
			s->b_transport(payload, zero_time);
		}

		// required to give the hand to the SystemC scheduler (yield)
		sc_core::wait(0, sc_core::SC_MS);
	}
}



SC_Module_container
::SC_Module_container(Module &module)
: module(&module), sc_modules()
{
}

void SC_Module_container
::create_module(const int id)
{
	if (module->tasks_with_nullptr.size() != sc_modules.size())
	{
		sc_modules.resize(module->tasks_with_nullptr.size());
		fill(sc_modules.begin(), sc_modules.end(), nullptr);
	}

	if ((size_t)id < sc_modules.size())
	{
		if (sc_modules[id] != nullptr)
			erase_module(id);

		const std::string module_name = module->get_custom_name().empty() ? module->get_name() : module->get_custom_name();
		sc_modules[id] = std::shared_ptr<SC_Module>(new SC_Module((*module)[id], (module_name + "::" + (*module)[id].get_name()).c_str()));
	}
	else
	{
		std::stringstream message;
		message << "'id' does not exist ('id' = " << id << ", 'module.name' = " << module->get_name() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

void SC_Module_container
::erase_module(const int id)
{
	if ((size_t)id < sc_modules.size() && sc_modules[id] != nullptr)
	{
		sc_modules[id] = nullptr;
	}
	else
	{
		std::stringstream message;
		message << "'id' does not exist ('id' = " << id << ", 'module.name' = " << module->get_name() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

#endif
