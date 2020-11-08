/*!
 * \file
 * \brief Class module::Module.
 */
#ifndef MODULE_HPP_
#define MODULE_HPP_

#include <type_traits>
#include <functional>
#include <cstddef>
#include <vector>
#include <memory>
#include <string>

#include "Tools/Interface/Interface_clone.hpp"
#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#ifdef AFF3CT_SYSTEMC_MODULE
#include "Module/SC_Module.hpp"
#endif

namespace aff3ct
{
namespace module
{
/*!
 * \class Module
 *
 * \brief A Module is an abstract concept. Basically, all the objects used in a Simulation are a Module.
 */
class Module : public tools::Interface_clone
{
protected:
	int         n_frames;     /*!< Number of frames to process in this Module */
	int         n_frames_per_wave;
	int         n_waves;
	int         n_frames_per_wave_rest;
	bool        single_wave;
	std::string name;         /*!< Name of the Module. */
	std::string short_name;   /*!< Short name of the Module. */
	std::string custom_name;  /*!< Custom name of the Module. */
	std::vector<std::shared_ptr<Task>> tasks_with_nullptr;

public:
	std::vector<std::shared_ptr<Task>> tasks;
#ifdef AFF3CT_SYSTEMC_MODULE
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	friend SC_Module_container;
#endif
	SC_Module_container sc;
#endif

	/*!
	 * \brief Constructor.
	 */
	explicit Module();

	/*!
	 * \brief Destructor.
	 */
	virtual ~Module() = default;

	virtual Module* clone() const;

	/*!
	 * \brief Get the number of frames.
	 *
	 * \return the number of frames to process in this Module.
	 */
	inline int get_n_frames() const;

	virtual void set_n_frames(const int n_frames);

	inline int get_n_frames_per_wave() const;

	inline int get_n_waves() const;

	inline int get_n_frames_per_wave_rest() const;

	inline bool is_single_wave() const;

	const std::string& get_name() const;

	const std::string& get_short_name() const;

	void set_custom_name(const std::string &custom_name);

	const std::string& get_custom_name() const;

	void remove_custom_name();

	inline Task& operator[](const size_t id);

protected:
	virtual void deep_copy(const Module &m);

	void set_name(const std::string &name);

	void set_short_name(const std::string &short_name);

	Task& create_task(const std::string &name, const int id = -1);

	template <typename T>
	size_t create_socket_in(Task& task, const std::string &name, const size_t n_elmts);

	template <typename T>
	size_t create_socket_in_out(Task& task, const std::string &name, const size_t n_elmts);

	template <typename T>
	size_t create_socket_out(Task& task, const std::string &name, const size_t n_elmts);

	void create_codelet(Task& task, std::function<int(Module &m, Task &t, const int frame_id)> codelet);

	void register_timer(Task& task, const std::string &key);

	virtual void set_n_frames_per_wave(const int n_frames_per_wave);

	void set_single_wave(const bool enable_single_wave);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Module.hxx"
#endif

#endif /* MODULE_HPP_ */
