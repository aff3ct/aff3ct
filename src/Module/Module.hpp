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
#include "Socket.hpp"
#ifdef SYSTEMC_MODULE
#include "SC_Module.hpp"
#endif

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
	std::vector<Task*> tasks_with_nullptr;

public:
	std::vector<Task*> tasks;
#ifdef SYSTEMC_MODULE
	friend SC_Module_container;
	SC_Module_container sc;
#endif

	/*!
	 * \brief Constructor.
	 *
	 * \param n_frames: number of frames to process in this Module.
	 * \param name    : Module's name.
	 */
	Module(const int n_frames = 1)
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

	/*!
	 * \brief Destructor.
	 */
	virtual ~Module()
	{
		for (auto t : tasks) delete t;
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

	inline void set_name(const std::string &name)
	{
		this->name = name;
	}

	inline const std::string& get_name() const
	{
		return this->name;
	}

	inline void set_short_name(const std::string &short_name)
	{
		this->short_name = short_name;
	}

	inline const std::string& get_short_name() const
	{
		return this->short_name;
	}

	inline Task& operator[](const int id)
	{
		return *tasks_with_nullptr[id];
	}

protected:
	Task& create_task(const std::string name, const int id = -1)
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

	template <typename T>
	Socket& create_socket_in(Task& task, const std::string name, const size_t n_elmts)
	{
		return task.template create_socket_in<T>(name, n_elmts);
	}

	template <typename T>
	Socket& create_socket_in_out(Task& task, const std::string name, const size_t n_elmts)
	{
		return task.template create_socket_in_out<T>(name, n_elmts);
	}

	template <typename T>
	Socket& create_socket_out(Task& task, const std::string name, const size_t n_elmts)
	{
		return task.template create_socket_out<T>(name, n_elmts);
	}

	void create_codelet(Task& task, std::function<int(void)> codelet)
	{
		task.create_codelet(codelet);
	}

	void register_timer(Task& task, const std::string key)
	{
		task.register_timer(key);
	}
};
}
}

#endif /* MODULE_HPP_ */
