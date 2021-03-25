#include <cassert>
#include <sstream>
#include <algorithm>
#include <exception>

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
Task& Module
::operator[](const size_t id)
{
	assert(id < this->tasks_with_nullptr.size());
	assert(this->tasks_with_nullptr[id] != nullptr);

	return *this->tasks_with_nullptr[id];
}

const Task& Module
::operator[](const size_t id) const
{
	assert(id < this->tasks_with_nullptr.size());
	assert(this->tasks_with_nullptr[id] != nullptr);

	return *this->tasks_with_nullptr[id];
}

Socket& Module
::operator[](const std::string &tsk_sck)
{
	size_t pos = tsk_sck.find("::", 0);
	if ((int)pos < 0)
	{
		std::stringstream message;
		message << "Invalid socket name, it should be of the form task::socket.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
	std::string tsk_name = tsk_sck.substr(0, pos);
	tsk_name.erase(remove(tsk_name.begin(), tsk_name.end(), ' '), tsk_name.end());
	std::string sck_name = tsk_sck.substr(pos+2, tsk_sck.size());
	sck_name.erase(remove(sck_name.begin(), sck_name.end(), ' '), sck_name.end());
	auto &cur_tsk = this->operator()(tsk_name);

	auto it = find_if(cur_tsk.sockets.begin(), cur_tsk.sockets.end(),
	                  [sck_name](std::shared_ptr<Socket> s){return s->get_name()==sck_name;});

	if (it == cur_tsk.sockets.end())
	{
		std::stringstream message;
		message << "Socket '" << sck_name << "' not found for task '" << tsk_name << "'.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	return *it->get();
}

Task& Module
::operator()(const std::string &tsk_name)
{
	auto it = find_if(this->tasks.begin(), this->tasks.end(),
					  [tsk_name](std::shared_ptr<Task> t){return t->get_name()==tsk_name;});

	if (it == this->tasks.end())
	{
		std::stringstream message;
		message << "Task '" << tsk_name << "' not found.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	return *it->get();
}

template <typename T>
inline size_t Module
::create_socket_in(Task& task, const std::string &name, const size_t n_elmts)
{
	return task.template create_socket_in<T>(name, n_elmts * this->n_frames);
}

template <typename T>
inline size_t Module
::create_socket_out(Task& task, const std::string &name, const size_t n_elmts)
{
	return task.template create_socket_out<T>(name, n_elmts * this->n_frames);
}

size_t Module
::get_n_frames() const
{
	return this->n_frames;
}

size_t Module
::get_n_frames_per_wave() const
{
	return this->n_frames_per_wave;
}

size_t Module
::get_n_waves() const
{
	return this->n_waves;
}

size_t Module
::get_n_frames_per_wave_rest() const
{
	return this->n_frames_per_wave_rest;
}

bool Module
::is_single_wave() const
{
	return this->single_wave;
}

}
}
