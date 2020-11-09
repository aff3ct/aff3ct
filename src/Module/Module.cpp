#include <cmath>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Module.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

Module
::Module()
: n_frames(1),
  n_frames_per_wave(1),
  n_waves(1),
  n_frames_per_wave_rest(0),
  single_wave(false),
  name("Module"),
  short_name("Module")
#ifdef AFF3CT_SYSTEMC_MODULE
, sc(*this)
#endif
{
}

void Module
::deep_copy(const Module &m)
{
	this->tasks_with_nullptr.clear();
	this->tasks.clear();

	for (auto &t : m.tasks_with_nullptr)
	{
		if (t == nullptr)
			this->tasks_with_nullptr.push_back(nullptr);
		else
		{
			auto t_new = std::shared_ptr<Task>(t->clone());
			t_new->module = this;
			this->tasks_with_nullptr.push_back(t_new);
			this->tasks.push_back(std::move(t_new));
		}
	}

#ifdef AFF3CT_SYSTEMC_MODULE
	this->sc.module = this;
	this->sc.sc_modules.clear();
	for (size_t i = 0; i < m.sc.sc_modules.size(); i++)
		if (m.sc.sc_modules[i] != nullptr)
			this->sc.create_module(i);
#endif
}

Module* Module
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

void Module
::_set_n_frames(const int n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		if (n_frames <= 0)
		{
			std::stringstream message;
			message << "'n_frames' has to be greater than 0 ('n_frames' = " << n_frames << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		for (auto &t : tasks)
			t->update_n_frames((size_t)old_n_frames, (size_t)n_frames);
		this->n_frames = n_frames;
		this->n_frames_per_wave_rest = this->get_n_frames() % this->get_n_frames_per_wave();
		this->n_waves = (int)std::ceil((float)this->get_n_frames() / (float)this->get_n_frames_per_wave());
	}
}

void Module
::_set_n_frames_per_wave(const int n_frames_per_wave)
{
	const auto old_n_frames_per_wave = this->get_n_frames_per_wave();
	if (old_n_frames_per_wave != n_frames_per_wave)
	{
		if (n_frames_per_wave <= 0)
		{
			std::stringstream message;
			message << "'n_frames_per_wave' has to be greater than 0 ('n_frames_per_wave' = " << n_frames_per_wave
			        << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		for (auto &t : tasks)
			t->update_n_frames_per_wave((size_t)old_n_frames_per_wave, (size_t)n_frames_per_wave);
		this->n_frames_per_wave = n_frames_per_wave;
		this->n_frames_per_wave_rest = this->get_n_frames() % this->get_n_frames_per_wave();
		this->n_waves = (int)std::ceil((float)this->get_n_frames() / (float)this->get_n_frames_per_wave());
	}
}

void Module
::set_n_frames(const int n_frames)
{
#ifdef AFF3CT_SYSTEMC_MODULE
	std::stringstream message;
	message << "'This feature is not supported with the SystemC/TLM interface.";
	throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
#endif

	this->_set_n_frames(n_frames);
	if (this->is_single_wave())
		this->_set_n_frames_per_wave(n_frames);
}

void Module
::set_n_frames_per_wave(const int n_frames_per_wave)
{
#ifdef AFF3CT_SYSTEMC_MODULE
	std::stringstream message;
	message << "'This feature is not supported with the SystemC/TLM interface.";
	throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
#endif

	this->_set_n_frames_per_wave(n_frames_per_wave);
	if (this->is_single_wave())
		this->_set_n_frames(n_frames_per_wave);
}

void Module
::set_single_wave(const bool enable_single_wave)
{
#ifdef AFF3CT_SYSTEMC_MODULE
	std::stringstream message;
	message << "'This feature is not supported with the SystemC/TLM interface.";
	throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
#endif

	this->single_wave = enable_single_wave;
	this->set_n_frames         (this->n_frames);
	this->set_n_frames_per_wave(this->n_frames);
}

void Module
::set_name(const std::string &name)
{
	this->name = name;
}

const std::string& Module
::get_name() const
{
	return this->name;
}

void Module
::set_short_name(const std::string &short_name)
{
	this->short_name = short_name;
}

const std::string& Module
::get_short_name() const
{
	return this->short_name;
}

void Module
::set_custom_name(const std::string &custom_name)
{
	this->custom_name = custom_name;
}

const std::string& Module
::get_custom_name() const
{
	return this->custom_name;
}

void Module
::remove_custom_name()
{
	this->custom_name = "";
}

Task& Module
::create_task(const std::string &name, const int id)
{
	auto t = std::make_shared<Task>(*this, name);

	if (id < 0)
	{
		tasks_with_nullptr.push_back(t);
	}
	else if (tasks_with_nullptr.size() > (size_t)id && tasks_with_nullptr[id] == nullptr)
	{
		tasks_with_nullptr[id] = t;
	}
	else
	{
		std::stringstream message;
		message << "Impossible to create the task ('task.name' = " << name
		        << ", 'task.id' = " << id
		        << ", 'tasks.size()' = " << tasks.size()
		        << ", 'module.name' = " << this->get_name() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	tasks.push_back(std::move(t));
	return *tasks.back();
}

void Module
::create_codelet(Task& task, std::function<int(Module &m, Task &t, const int frame_id)> codelet)
{
	task.create_codelet(codelet);
}

void Module
::register_timer(Task& task, const std::string &key)
{
	task.register_timer(key);
}
