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

#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <map>
#include <functional>
#include <memory>

#include "Task.hpp"
#include "Socket.hpp"
#ifdef AFF3CT_SYSTEMC_MODULE
#include "SC_Module.hpp"
#endif

// convert strongly typed enum to integer
template <typename E>
constexpr typename std::underlying_type<E>::type operator+(E e) noexcept {
	return static_cast<typename std::underlying_type<E>::type>(e);
}

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
	std::vector<std::shared_ptr<Task>> tasks_with_nullptr;

public:
	std::vector<std::shared_ptr<Task>> tasks;
#ifdef AFF3CT_SYSTEMC_MODULE
	friend SC_Module_container;
	SC_Module_container sc;
#endif

	/*!
	 * \brief Constructor.
	 *
	 * \param n_frames: number of frames to process in this Module.
	 * \param name    : Module's name.
	 */
	explicit Module(const int n_frames = 1);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Module() = default;

	/*!
	 * \brief Get the number of frames.
	 *
	 * \return the number of frames to process in this Module.
	 */
	virtual int get_n_frames() const;

	void set_name(const std::string &name);

	const std::string& get_name() const;

	void set_short_name(const std::string &short_name);

	const std::string& get_short_name() const;

	Task& operator[](const int id);

protected:
	Task& create_task(const std::string &name, const int id = -1);

	template <typename T>
	Socket& create_socket_in(Task& task, const std::string &name, const size_t n_elmts);

	template <typename T>
	Socket& create_socket_in_out(Task& task, const std::string &name, const size_t n_elmts);

	template <typename T>
	Socket& create_socket_out(Task& task, const std::string &name, const size_t n_elmts);

	void create_codelet(Task& task, std::function<int(void)> codelet);

	void register_timer(Task& task, const std::string &key);
};
}
}
#include "Module.hxx"

#endif /* MODULE_HPP_ */
