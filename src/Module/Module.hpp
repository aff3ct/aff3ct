/*!
 * \file
 * \brief A Module is an abstract concept. Basically, all the objects used in a Simulation are a Module.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef MODULE_HPP_
#define MODULE_HPP_

#include <string>
#include <sstream>

#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <map>
#include <functional>

#include "Task.hpp"
#include "Tools/Exception/exception.hpp"

namespace aff3ct
{
namespace module
{
/*!
 * \class Module
 *
 * \brief A Module is an abstract concept. Basically, all the objects used in a Simulation are a Module.
 */
class Module
{
protected:
	int         n_frames;    /*!< Number of frames to process in this Module */
	std::string name;        /*!< Name of the Module. */
	std::string short_name;  /*!< Short name of the Module. */

public:
	std::map<std::string, Task*> tasks;

	/*!
	 * \brief Constructor.
	 *
	 * \param n_frames: number of frames to process in this Module.
	 * \param name    : Module's name.
	 */
	Module(const int n_frames = 1, const std::string name = "Module", const std::string short_name = "Module")
	: n_frames(n_frames), name(name), short_name(short_name)
	{
		if (n_frames <= 0)
		{
			std::stringstream message;
			message << "'n_frames' has to be greater than 0 ('n_frames' = " << n_frames << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Module()
	{
		for (auto t : tasks) delete t.second;
	}

	/*!
	 * \brief Get the number of frames.
	 *
	 * \return the number of frames to process in this Module.
	 */
	virtual int get_n_frames() const
	{
		return n_frames;
	}

	/*!
	 * \brief Rename the Module.
	 *
	 * \param name: the new Module name.
	 */
	void rename(const std::string name)
	{
		this->name = name;
	}

	std::string get_name() const
	{
		return this->name;
	}

	std::string get_short_name() const
	{
		return this->short_name;
	}

	Task& operator[](const std::string name)
	{
		if (tasks.find(name) != tasks.end())
		{
			return *tasks[name];
		}
		else
		{
			std::stringstream message;
			message << "'name' does not exist ('name' = " << name << ", 'module.name' = " << this->name << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}

protected:
	Task& create_task(const std::string name)
	{
		bool autoalloc = false, autoexec = false, stats = false, debug = false;
		tasks[name] = new Task(*this, name, autoalloc, autoexec, stats, debug);
		return *tasks[name];
	}

	template <typename T>
	void create_socket_in(Task& task, const std::string name, const size_t n_elmts)
	{
		task.template create_socket_in<T>(name, n_elmts);
	}

	template <typename T>
	void create_socket_in_out(Task& task, const std::string name, const size_t n_elmts)
	{
		task.template create_socket_in_out<T>(name, n_elmts);
	}

	template <typename T>
	void create_socket_out(Task& task, const std::string name, const size_t n_elmts)
	{
		task.template create_socket_out<T>(name, n_elmts);
	}

	void create_codelet(Task& task, std::function<int(void)> codelet)
	{
		task.create_codelet(codelet);
	}

	void register_duration(Task& task, const std::string key)
	{
		task.register_duration(key);
	}

	void update_duration(const std::string task_key, const std::string key, const std::chrono::nanoseconds &duration)
	{
		tasks[task_key]->update_duration(key, duration);
	}
};
}
}

#endif /* MODULE_HPP_ */
