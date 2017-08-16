#include <iostream>
#include <iomanip>

#include "Tools/Display/bash_tools.h"
#include "Tools/Display/Frame_trace/Frame_trace.hpp"

#include "Module.hpp"
#include "Process.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

Process::Process(const Module &module, const std::string name, const bool autostart, const bool stats, const bool debug)
: module(module),
  name(name),
  autostart(autostart),
  stats(stats),
  debug(debug),
  debug_limit(-1),
  debug_precision(2),
  codelet([]() { throw tools::unimplemented_error(__FILE__, __LINE__, __func__); }),
  n_calls(0),
  duration_total(std::chrono::nanoseconds(0)),
  duration_min(std::chrono::nanoseconds(0)),
  duration_max(std::chrono::nanoseconds(0))
{
}

std::string Process::get_name()
{
	return this->name;
}

void Process::set_autostart(const bool autostart)
{
	this->autostart = autostart;
}

bool Process::is_autostart()
{
	return this->autostart;
}

void Process::set_stats(const bool stats)
{
	this->stats = stats;
}

bool Process::is_stats()
{
	return this->stats;
}

void Process::set_debug(const bool debug)
{
	this->debug = debug;
}

void Process::set_debug_limit(const uint32_t limit)
{
	this->debug_limit = (int32_t)limit;
}

void Process::set_debug_precision(const uint8_t prec)
{
	this->debug_precision = prec;
}

bool Process::is_debug()
{
	return this->debug;
}

template <typename T>
static inline void display_data(const T *data, const size_t n_elmts, const uint8_t p)
{
	for (auto i = 0; i < (int)n_elmts; i++)
		std::cout << std::fixed << std::setprecision(p) << std::setw(p +3) << +data[i]
		          << (i < (int)n_elmts -1 ? ", " : "");
}

void Process::exec()
{
	if (can_exec())
	{
		size_t max_n_chars = 0;
		if (debug)
		{
			const auto sty_type   = tools::Style::BOLD | tools::FG::Color::MAGENTA | tools::FG::INTENSE;
			const auto sty_class  = tools::Style::BOLD;
			const auto sty_method = tools::Style::BOLD | tools::FG::Color::GREEN;

			std::cout << tools::format(module.get_name(), sty_class) << "::" << tools::format(get_name(), sty_method)
			          << "(";
			for (auto i = 0; i < (int)in.size(); i++)
			{
				auto n_elmts = in[i].get_databytes() / (size_t)in[i].get_datatype_size();
				std::cout << tools::format("const ", sty_type)
				          << tools::format(in[i].get_datatype_string(), sty_type)
				          << " " << in[i].get_name() << "[" << n_elmts << "]"
				          << (i < (int)in.size() -1 || out.size() > 0 ? ", " : "");

				max_n_chars = std::max(in[i].get_name().size(), max_n_chars);
			}
			for (auto i = 0; i < (int)out.size(); i++)
			{
				auto n_elmts = out[i].get_databytes() / (size_t)out[i].get_datatype_size();
				std::cout << tools::format(out[i].get_datatype_string(), sty_type)
				          << " " << out[i].get_name() << "[" << n_elmts << "]"
				          << (i < (int)out.size() -1 ? ", " : "");

				max_n_chars = std::max(out[i].get_name().size(), max_n_chars);
			}
			std::cout << ")" << std::endl;

			for (auto &i : in)
			{
				std::string spaces; for (size_t s = 0; s < max_n_chars - i.get_name().size(); s++) spaces += " ";

				auto n_elmts = i.get_databytes() / (size_t)i.get_datatype_size();
				auto limit = debug_limit != -1 ? std::min(n_elmts, (size_t)debug_limit) : n_elmts;
				auto p = debug_precision;
				std::cout << "{IN}  " << i.get_name() << spaces << " = [";
				     if (i.get_datatype() == typeid(int8_t )) display_data((int8_t *)i.get_dataptr(), limit, p);
				else if (i.get_datatype() == typeid(int16_t)) display_data((int16_t*)i.get_dataptr(), limit, p);
				else if (i.get_datatype() == typeid(int32_t)) display_data((int32_t*)i.get_dataptr(), limit, p);
				else if (i.get_datatype() == typeid(int64_t)) display_data((int64_t*)i.get_dataptr(), limit, p);
				else if (i.get_datatype() == typeid(float  )) display_data((float  *)i.get_dataptr(), limit, p);
				else if (i.get_datatype() == typeid(double )) display_data((double *)i.get_dataptr(), limit, p);
				std::cout << (limit < n_elmts ? ", ..." : "") << "]" << std::endl;
			}
		}

		if (stats)
		{
			auto t_start = std::chrono::steady_clock::now();
			this->codelet();
			auto duration = std::chrono::steady_clock::now() - t_start;

			this->duration_total += duration;
			if (n_calls)
			{
				this->duration_min = std::min(this->duration_min, duration);
				this->duration_max = std::max(this->duration_max, duration);
			}
			else
			{
				this->duration_min = duration;
				this->duration_max = duration;
			}
		}
		else
			this->codelet();
		this->n_calls++;

		if (debug)
		{
			for (auto &o : out)
			{
				std::string spaces; for (size_t s = 0; s < max_n_chars - o.get_name().size(); s++) spaces += " ";

				auto n_elmts = o.get_databytes() / (size_t)o.get_datatype_size();
				auto limit = debug_limit != -1 ? std::min(n_elmts, (size_t)debug_limit) : n_elmts;
				std::cout << "{OUT} " << o.get_name() << spaces << " = [";
				auto p = debug_precision;
				     if (o.get_datatype() == typeid(int8_t )) display_data((int8_t *)o.get_dataptr(), limit, p);
				else if (o.get_datatype() == typeid(int16_t)) display_data((int16_t*)o.get_dataptr(), limit, p);
				else if (o.get_datatype() == typeid(int32_t)) display_data((int32_t*)o.get_dataptr(), limit, p);
				else if (o.get_datatype() == typeid(int64_t)) display_data((int64_t*)o.get_dataptr(), limit, p);
				else if (o.get_datatype() == typeid(float  )) display_data((float  *)o.get_dataptr(), limit, p);
				else if (o.get_datatype() == typeid(double )) display_data((double *)o.get_dataptr(), limit, p);
				std::cout << (limit < n_elmts ? ", ..." : "") << "]" << std::endl;
			}
			std::cout << std::endl;
		}
	}
	else
	{
		std::stringstream message;
		message << "The process cannot be executed because some of the inputs are not fed ('process.name' = "
		        << this->get_name() << ", 'module.name' = " << module.get_name() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename T>
void Process::create_socket_in(const std::string name, const size_t n_elmts)
{
	in.push_back(Socket(*this, name, typeid(T), n_elmts * sizeof(T)));
}

template <typename T>
void Process::create_socket_out(const std::string name, const size_t n_elmts)
{
	out.push_back(Socket(*this, name, typeid(T), n_elmts * sizeof(T)));

	// memory allocation
	out_buffers.push_back(std::vector<uint8_t>(out.back().databytes));
	out.back().dataptr = out_buffers.back().data();
}

void Process::create_codelet(std::function<void(void)> codelet)
{
	this->codelet = codelet;
}

Socket& Process::operator[](const std::string name)
{
	for (auto &s : in ) if (s.get_name() == name) return s;
	for (auto &s : out) if (s.get_name() == name) return s;

	std::stringstream message;
	message << "The socket does not exist ('socket.name' = " << name << ", 'process.name' = " << this->get_name()
	        << ", 'module.name' = " << module.get_name() << ").";
	throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
}

bool Process::last_input_socket(Socket &s_in)
{
	return &s_in == &in.back();
}

bool Process::can_exec()
{
	for (auto &s : this->in)
		if (s.dataptr == nullptr)
			return false;
	return true;
}

uint32_t Process::get_n_calls()
{
	return this->n_calls;
}

std::chrono::nanoseconds Process::get_duration_total()
{
	return this->duration_total;
}

std::chrono::nanoseconds Process::get_duration_avg()
{
	return this->duration_total / this->n_calls;
}

std::chrono::nanoseconds Process::get_duration_min()
{
	return this->duration_min;
}

std::chrono::nanoseconds Process::get_duration_max()
{
	return this->duration_max;
}

const std::vector<std::string>& Process::get_registered_duration()
{
	return this->registered_duration;
}

uint32_t Process::get_registered_n_calls(const std::string key)
{
	return this->registered_n_calls[key];
}

std::chrono::nanoseconds Process::get_registered_duration_total(const std::string key)
{
	return this->registered_duration_total[key];
}

std::chrono::nanoseconds Process::get_registered_duration_avg(const std::string key)
{
	return this->registered_duration_total[key] / this->n_calls;
}

std::chrono::nanoseconds Process::get_registered_duration_min(const std::string key)
{
	return this->registered_duration_min[key];
}

std::chrono::nanoseconds Process::get_registered_duration_max(const std::string key)
{
	return this->registered_duration_max[key];
}

void Process::register_duration(const std::string key)
{
	this->registered_duration.push_back(key);
	this->registered_duration_total[key] = std::chrono::nanoseconds(0);
	this->registered_duration_max  [key] = std::chrono::nanoseconds(0);
	this->registered_duration_min  [key] = std::chrono::nanoseconds(0);
}

void Process::update_duration(const std::string key, const std::chrono::nanoseconds &duration)
{
	this->registered_n_calls[key]++;
	this->registered_duration_total[key] += duration;
	if (this->n_calls)
	{
		this->registered_duration_max[key] = std::max(this->registered_duration_max[key], duration);
		this->registered_duration_min[key] = std::min(this->registered_duration_min[key], duration);
	}
	else
	{
		this->registered_duration_max[key] = duration;
		this->registered_duration_min[key] = duration;
	}
}

void Process::reset_stats()
{
	this->n_calls        =                          0;
	this->duration_total = std::chrono::nanoseconds(0);
	this->duration_min   = std::chrono::nanoseconds(0);
	this->duration_max   = std::chrono::nanoseconds(0);

	for (auto &x : this->registered_n_calls       ) x.second =                          0;
	for (auto &x : this->registered_duration_total) x.second = std::chrono::nanoseconds(0);
	for (auto &x : this->registered_duration_min  ) x.second = std::chrono::nanoseconds(0);
	for (auto &x : this->registered_duration_max  ) x.second = std::chrono::nanoseconds(0);
}

// ==================================================================================== explicit template instantiation
template void Process::create_socket_in<int8_t >(const std::string, const size_t);
template void Process::create_socket_in<int16_t>(const std::string, const size_t);
template void Process::create_socket_in<int32_t>(const std::string, const size_t);
template void Process::create_socket_in<int64_t>(const std::string, const size_t);
template void Process::create_socket_in<float  >(const std::string, const size_t);
template void Process::create_socket_in<double >(const std::string, const size_t);

template void Process::create_socket_out<int8_t >(const std::string, const size_t);
template void Process::create_socket_out<int16_t>(const std::string, const size_t);
template void Process::create_socket_out<int32_t>(const std::string, const size_t);
template void Process::create_socket_out<int64_t>(const std::string, const size_t);
template void Process::create_socket_out<float  >(const std::string, const size_t);
template void Process::create_socket_out<double >(const std::string, const size_t);
// ==================================================================================== explicit template instantiation
