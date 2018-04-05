#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Module.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

Module::
Module(const int n_frames)
: n_frames(n_frames), name("Module"), short_name("Module")
#ifdef SYSTEMC_MODULE
, sc(*this)
#endif
{
	if (n_frames <= 0)
	{
		std::stringstream message;
		message << "'n_frames' has to be greater than 0 ('n_frames' = " << n_frames << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

Module::
~Module()
{
	for (auto t : tasks) delete t;
}

int Module::
get_n_frames() const
{
	return n_frames;
}

void Module::
set_name(const std::string &name)
{
	this->name = name;
}

const std::string& Module::
get_name() const
{
	return this->name;
}

void Module::
set_short_name(const std::string &short_name)
{
	this->short_name = short_name;
}

const std::string& Module::
get_short_name() const
{
	return this->short_name;
}

Task& Module::
operator[](const int id)
{
	return *tasks_with_nullptr[id];
}

Task& Module::
create_task(const std::string &name, const int id)
{
	bool autoalloc = false, autoexec = false, stats = false, fast = false, debug = false;
	auto t = new Task(*this, name, autoalloc, autoexec, stats, fast, debug);
	if (id < 0)
	{
		tasks.push_back(t);
		tasks_with_nullptr.push_back(t);
	}
	else
	{
		if (tasks_with_nullptr.size() > (size_t)id && tasks_with_nullptr[id] == nullptr)
		{
			tasks.push_back(t);
			tasks_with_nullptr[id] = t;

		}else
		{
			delete t;

			std::stringstream message;
			message << "Impossible to create the task ('task.name' = " << name
			        << ", 'task.id' = " << id
			        << ", 'tasks.size()' = " << tasks.size()
			        << ", 'module.name' = " << this->get_name() << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}
	}
	return *t;
}

void Module::
create_codelet(Task& task, std::function<int(void)> codelet)
{
	task.create_codelet(codelet);
}

void Module::
register_timer(Task& task, const std::string &key)
{
	task.register_timer(key);
}