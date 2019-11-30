#include <thread>
#include <utility>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Module.hpp"
#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#include "Tools/Chain/Chain.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

Chain
::Chain(const module::Task &first, const module::Task &last, const size_t n_threads)
: n_threads(n_threads), tasks_sequences(n_threads), modules(n_threads)
{
	if (n_threads == 0)
	{
		std::stringstream message;
		message << "'n_threads' has to be strictly greater than 0.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	std::vector<const module::Task*> tasks_sequence;
	this->init_recursive(tasks_sequence, first, first, &last);
	this->duplicate(tasks_sequence);
}

Chain
::Chain(const module::Task &first, const size_t n_threads)
: n_threads(n_threads), tasks_sequences(n_threads), modules(n_threads)
{
	if (n_threads == 0)
	{
		std::stringstream message;
		message << "'n_threads' has to be strictly greater than 0.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	std::vector<const module::Task*> tasks_sequence;
	this->init_recursive(tasks_sequence, first, first);
	this->duplicate(tasks_sequence);
}

Chain* Chain
::clone() const
{
	auto c = new Chain(*this);
	std::vector<const module::Task*> tasks_sequence;
	for (size_t t = 0; t < this->tasks_sequences[0].size(); t++)
		tasks_sequence.push_back(this->tasks_sequences[0][t]);
	c->duplicate(tasks_sequence);
	return c;
}

std::vector<std::vector<const module::Module*>> Chain
::get_modules_per_threads() const
{
	std::vector<std::vector<const module::Module*>> modules_per_threads(modules.size());
	size_t tid = 0;
	for (auto &e : modules)
	{
		for (auto &ee : e)
			modules_per_threads[tid].push_back(ee.get());
		tid++;
	}
	return modules_per_threads;
}

std::vector<std::vector<const module::Module*>> Chain
::get_modules_per_types() const
{

	std::vector<std::vector<const module::Module*>> modules_per_types(modules[0].size());
	for (auto &e : modules)
	{
		size_t mid = 0;
		for (auto &ee : e)
			modules_per_types[mid++].push_back(ee.get());
	}
	return modules_per_types;
}

void Chain
::init_recursive(std::vector<const module::Task*> &tasks_sequence,
                 const module::Task& first,
                 const module::Task& current_task,
                 const module::Task *last)
{
	if (&current_task == &first)
	{
		for (auto &s : current_task.sockets)
			if ((current_task.get_socket_type(*s) == module::socket_t::SOUT ||
				 current_task.get_socket_type(*s) == module::socket_t::SIN_SOUT) && s->get_dataptr() == nullptr)
			{
				std::stringstream message;
				message << "Output sockets of the first task has to be filled.";
				throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
			}
	}
	else if (&current_task == last)
	{
		for (auto &s : current_task.sockets)
			if ((current_task.get_socket_type(*s) == module::socket_t::SIN ||
				 current_task.get_socket_type(*s) == module::socket_t::SIN_SOUT) && s->get_dataptr() == nullptr)
			{
				std::stringstream message;
				message << "Input sockets of the last task has to be filled.";
				throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
			}
	}
	else
	{
		if (!current_task.can_exec())
		{
			std::stringstream message;
			message << "'this->can_exec()' has to be true.";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}
	}

	tasks_sequence.push_back(&current_task);

	if (&current_task != last)
	{
		for (auto &s : current_task.sockets)
		{
			if (current_task.get_socket_type(*s) == module::socket_t::SIN_SOUT ||
				current_task.get_socket_type(*s) == module::socket_t::SOUT)
			{
				auto bss = s->get_bound_sockets();
				for (auto &bs : bss)
				{
					if (bs != nullptr)
					{
						auto &t = bs->get_task();
						if (t.is_last_input_socket(*bs))
							Chain::init_recursive(tasks_sequence, first, t, last);
					}
				}
			}
			else if (current_task.get_socket_type(*s) == module::socket_t::SIN)
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
}

void Chain
::_exec(std::function<bool(const std::vector<int>&)> &stop_condition, std::vector<module::Task*> &tasks_sequence)
{
	std::vector<int> statuses(tasks_sequence.size(), 0);
	while (!stop_condition(statuses))
		for (size_t ta = 0; ta < tasks_sequence.size(); ta++)
			statuses[ta] = tasks_sequence[ta]->exec();
}

void Chain
::_exec_without_statuses(std::function<bool()> &stop_condition, std::vector<module::Task*> &tasks_sequence)
{
	while (!stop_condition())
		for (size_t ta = 0; ta < tasks_sequence.size(); ta++)
			tasks_sequence[ta]->exec();
}

void Chain
::exec(std::function<bool(const std::vector<int>&)> stop_condition)
{
	std::vector<std::thread> threads(n_threads);
	for (size_t tid = 1; tid < n_threads; tid++)
		threads[tid] = std::thread(&Chain::_exec, this, std::ref(stop_condition), std::ref(this->tasks_sequences[tid]));

	this->_exec(stop_condition, this->tasks_sequences[0]);

	for (size_t tid = 1; tid < n_threads; tid++)
		threads[tid].join();
}

void Chain
::exec(std::function<bool()> stop_condition)
{
	std::vector<std::thread> threads(n_threads);
	for (size_t tid = 1; tid < n_threads; tid++)
		threads[tid] = std::thread(&Chain::_exec_without_statuses, this, std::ref(stop_condition),
		                           std::ref(this->tasks_sequences[tid]));

	this->_exec_without_statuses(stop_condition, this->tasks_sequences[0]);

	for (size_t tid = 1; tid < n_threads; tid++)
		threads[tid].join();
}

int Chain
::exec(const int tid)
{
	int ret = 0;
	for (size_t ta = 0; ta < this->tasks_sequences[tid].size(); ta++)
		ret += this->tasks_sequences[tid][ta]->exec();
	return ret;
}

void Chain
::duplicate(const std::vector<const module::Task*> &tasks_sequence)
{
	std::vector<std::pair<const module::Module*, std::vector<const module::Task*>>> modules_to_tasks;
	std::map<const module::Task*, size_t> task_to_module_id;

	auto exist_module = [](const std::vector<std::pair<const module::Module*, std::vector<const module::Task*>>>
	                       &modules_to_tasks,
	                       const module::Module *m) -> int
	{
		for (int i = 0; i < (int)modules_to_tasks.size(); i++)
			if (modules_to_tasks[i].first == m)
				return i;
		return -1;
	};

	for (auto &ta : tasks_sequence)
	{
		auto m = &ta->get_module();
		auto id = exist_module(modules_to_tasks, m);
		if (id == -1)
		{
			std::vector<const module::Task*> vt = {ta};
			task_to_module_id[ta] = modules_to_tasks.size();
			modules_to_tasks.push_back(std::make_pair(m, vt));
		}
		else
		{
			modules_to_tasks[id].second.push_back(ta);
			task_to_module_id[ta] = id;
		}
	}

	// clone the modules
	for (size_t tid = 0; tid < this->n_threads; tid++)
	{
		this->modules[tid].resize(modules_to_tasks.size());
		size_t m = 0;
		for (auto &mtt : modules_to_tasks)
			this->modules[tid][m++].reset(mtt.first->clone());
	}

	auto get_task_id = [this, &tasks_sequence](const module::Task &ta) -> int
	{
		for (size_t t = 0; t < tasks_sequence.size(); t++)
			if (&ta == tasks_sequence[t])
				return t;
		return -1;
	};

	auto get_socket_id = [](const module::Task &ta, const module::Socket& so) -> int
	{
		for (size_t s = 0; s < ta.sockets.size(); s++)
			if (&so == ta.sockets[s].get())
				return (int)s;
		return -1;
	};

	auto get_task_id_in_module = [](const module::Task* t) -> int
	{
		auto &m = t->get_module();

		for (size_t tt = 0; tt < m.tasks.size(); tt++)
			if (m.tasks[tt].get() == t)
				return tt;
		return -1;
	};

	auto get_task_cpy = [this, &task_to_module_id, &get_task_id_in_module, &tasks_sequence](const size_t tid,
		const size_t id) -> module::Task&
	{
		const auto module_id = task_to_module_id[tasks_sequence[id]];
		const auto task_id_in_module = get_task_id_in_module(tasks_sequence[id]);
		assert(task_id_in_module != -1);
		return *modules[tid][module_id]->tasks[task_id_in_module].get();
	};

	// create the n tasks sequences
	for (size_t tid = 0; tid < this->n_threads; tid++)
	{
		auto &tasks_chain_cpy = this->tasks_sequences[tid];
		tasks_chain_cpy.clear();
		for (size_t taid = 0; taid < tasks_sequence.size(); taid++)
		{
			auto &task_cpy = get_task_cpy(tid, taid);
			task_cpy.set_autoalloc(true);
			tasks_chain_cpy.push_back(&task_cpy);
		}
	}

	// bind the tasks of the n sequences
	for (size_t tout_id = 0; tout_id < tasks_sequence.size(); tout_id++)
	{
		auto &tout = tasks_sequence[tout_id];
		for (size_t sout_id = 0; sout_id < tout->sockets.size(); sout_id++)
		{
			auto &sout = tout->sockets[sout_id];
			if (tout->get_socket_type(*sout) == module::socket_t::SIN_SOUT ||
				tout->get_socket_type(*sout) == module::socket_t::SOUT)
			{
				for (auto &sin : sout->get_bound_sockets())
				{
					if (sin != nullptr)
					{
						auto &tin = sin->get_task();
						auto tin_id = get_task_id(tin);

						if (tin_id != -1)
						{
							auto sin_id = get_socket_id(tin, *sin);
							assert(sin_id != -1);

							for (size_t tid = 0; tid < this->n_threads; tid++)
							{
								auto &tasks_chain_cpy = this->tasks_sequences[tid];
								(*tasks_chain_cpy[tin_id])[sin_id].bind((*tasks_chain_cpy[tout_id])[sout_id]);
							}
						}
					}
				}
			}
		}
	}
}