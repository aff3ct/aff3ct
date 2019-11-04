#include <thread>

#include "Tools/Exception/exception.hpp"
#include "Module/Module.hpp"
#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#include "Module/Chain.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

Chain
::Chain(Task &bootstrap, const size_t n_threads)
: n_threads(n_threads)
{
	if (n_threads == 0)
	{
		std::stringstream message;
		message << "'n_threads' has to be strictly greater than 0.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	this->init_recursive(bootstrap);

	if (n_threads > 1)
	{
		this->tasks_sequences.resize(n_threads);
		this->duplicate();
	}
}

Chain
::~Chain()
{
	for (auto &s : this->tasks_sequences)
		for (auto &ta : s)
			delete ta;
}

void Chain
::init_recursive(Task& current_task)
{
	if (current_task.can_exec())
	{
		tasks_sequence.push_back(&current_task);
	}
	else
	{
		std::stringstream message;
		message << "'this->can_exec()' has to be true.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	for (auto &s : current_task.sockets)
	{
		if (current_task.get_socket_type(*s) == socket_t::SIN_SOUT ||
			current_task.get_socket_type(*s) == socket_t::SOUT)
		{
			auto bss = s->get_bound_sockets();
			for (auto &bs : bss)
			{
				if (bs != nullptr)
				{
					auto &t = bs->get_task();
					if (t.is_last_input_socket(*bs))
						this->init_recursive(t);
				}
			}
		}
		else if (current_task.get_socket_type(*s) == socket_t::SIN)
		{
			if (s->get_bound_sockets().size() > 1)
			{
				std::stringstream message;
				message << "'s->get_bound_sockets().size()' has to be smaller or equal to 1 ("
				        << "'s->get_bound_sockets().size()'"         << " = " << s->get_bound_sockets().size() << ", "
				        << "'get_socket_type(*s)'"                   << " = " << "socket_t::SIN"               << ", "
				        << "'s->get_name()'"                         << " = " << s->get_name()                 << ", "
				        << "'s->get_task().get_name()'"              << " = " << s->get_task().get_name()      << ", "
				        << "'s->get_task().get_module().get_name()'" << " = " << s->get_task().get_module().get_name()
				        << ").";
				throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
			}
		}
	}
}

void Chain
::_exec(std::function<bool(const std::vector<int>&)> &stop_condition, std::vector<Task*> &tasks_sequence)
{
	std::vector<int> statuses(tasks_sequence.size(), 0);
	while (!stop_condition(statuses))
		for (size_t ta = 0; ta < tasks_sequence.size(); ta++)
			statuses[ta] = tasks_sequence[ta]->exec();
}

void Chain
::exec(std::function<bool(const std::vector<int>&)> &stop_condition)
{
	if (this->get_n_threads() == 1)
		this->_exec(stop_condition, this->tasks_sequence);
	else
	{
		std::vector<std::thread> threads(n_threads);
		for (size_t tid = 0; tid < n_threads; tid++)
			threads[tid] = std::thread(&Chain::_exec, this, std::ref(stop_condition), std::ref(this->tasks_sequences[tid]));

		for (size_t tid = 0; tid < n_threads; tid++)
			threads[tid].join();
	}
}

void Chain
::duplicate()
{
	// clone the tasks
	for (size_t tid = 0; tid < this->n_threads; tid++)
	{
		auto &tasks_chain_cpy = this->tasks_sequences[tid];
		for (auto &ta : this->tasks_sequence)
		{
			auto ta_clone = ta->clone();
			ta_clone->set_autoalloc(true);
			tasks_chain_cpy.push_back(ta_clone);
		}
	}

	std::function<int(const Task&)> get_task_id = [this](const Task &ta) -> int {
		for (size_t t = 0; t < this->tasks_sequence.size(); t++)
			if (&ta == this->tasks_sequence[t])
				return t;
		return -1;
	};

	std::function<int(const Task&, const Socket&)> get_socket_id = [](const Task &ta, const Socket& so) {
		for (size_t s = 0; s < ta.sockets.size(); s++)
			if (&so == ta.sockets[s].get())
				return (int)s;
		return -1;
	};

	// bind the tasks
	for (size_t tout_id = 0; tout_id < this->tasks_sequence.size(); tout_id++)
	{
		auto &tout = this->tasks_sequence[tout_id];
		for (size_t sout_id = 0; sout_id < tout->sockets.size(); sout_id++)
		{
			auto &sout = tout->sockets[sout_id];
			if (tout->get_socket_type(*sout) == socket_t::SIN_SOUT ||
				tout->get_socket_type(*sout) == socket_t::SOUT)
			{
				for (auto &sin : sout->get_bound_sockets())
				{
					if (sin != nullptr)
					{
						auto &tin = sin->get_task();
						auto tin_id = get_task_id(tin);
						assert(tin_id != -1);

						auto sin_id = get_socket_id(tin, *sin);
						assert(sin_id != -1);

						for (size_t tid = 0; tid < this->n_threads; tid++)
						{
							auto &tasks_chain_cpy = this->tasks_sequences[tid];
							(*tasks_chain_cpy[tout_id])[sout_id].bind((*tasks_chain_cpy[tin_id])[sin_id]);
						}
					}
				}
			}
		}
	}
}