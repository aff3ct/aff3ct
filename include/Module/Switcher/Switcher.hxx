#include <string>
#include <sstream>
#include <algorithm>

#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"
#include "Module/Switcher/Switcher.hpp"

namespace aff3ct
{
namespace module
{

Task& Switcher
::operator[](const swi::tsk t)
{
	return Module::operator[]((size_t)t);
}

const Task& Switcher
::operator[](const swi::tsk t) const
{
	return Module::operator[]((size_t)t);
}

Switcher
::Switcher(const size_t n_data_sockets,
           const size_t n_elmts_commute,
           const std::type_index datatype_commute,
           const size_t n_elmts_select,
           const std::type_index datatype_select)
: Module(),
  n_data_sockets(n_data_sockets),
  n_elmts_commute(n_elmts_commute),
  n_elmts_select(n_elmts_select),
  n_bytes_commute(tools::compute_bytes(n_elmts_commute, datatype_commute)),
  n_bytes_select(tools::compute_bytes(n_elmts_select, datatype_select)),
  datatype_commute(datatype_commute),
  datatype_select(datatype_select),
  path(n_data_sockets -1),
  no_copy_commute(false),
  no_copy_select (false)
{
	const std::string name = "Switcher";
	this->set_name(name);
	this->set_short_name(name);
	this->set_single_wave(true);

	if (n_data_sockets == 0)
	{
		std::stringstream message;
		message << "'n_data_sockets' has to be greater than 0 ('n_data_sockets' = " << n_data_sockets << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (n_elmts_commute == 0)
	{
		std::stringstream message;
		message << "'n_elmts_commute' has to be greater than 0 ('n_elmts_commute' = " << n_elmts_commute << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (n_elmts_select == 0)
	{
		std::stringstream message;
		message << "'n_elmts_select' has to be greater than 0 ('n_elmts_select' = " << n_elmts_select << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p1 = this->create_task("commute");
	const auto p1s_in_data = this->create_socket_in(p1, "data", n_elmts_commute, datatype_commute);
	const auto p1s_in_ctrl = this->create_socket_in(p1, "ctrl", 1, typeid(int8_t));

	std::vector<size_t> p1s_out_data;
	for (size_t s = 0; s < this->get_n_data_sockets(); s++)
		p1s_out_data.push_back(this->create_socket_out(p1, "data" + std::to_string(s), n_elmts_commute, datatype_commute));

	this->create_codelet(p1, [p1s_in_data, p1s_in_ctrl, p1s_out_data](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &swi = static_cast<Switcher&>(m);

		const auto ctrl_socket_in  = static_cast<const int8_t*>(t[p1s_in_ctrl].get_dataptr());
		swi.set_path((size_t)ctrl_socket_in[0]);
		const size_t path = swi.get_path();

		if (!swi.is_no_copy_commute())
		{
			const auto data_socket_in  = static_cast<const int8_t*>(t[p1s_in_data       ].get_dataptr());
			      auto data_socket_out = static_cast<      int8_t*>(t[p1s_out_data[path]].get_dataptr());

			std::copy(data_socket_in,
			          data_socket_in + swi.get_n_frames() * swi.get_n_bytes_commute(),
			          data_socket_out);
		}

		return (int)path;
	});

	auto &p2 = this->create_task("select");
	std::vector<size_t> p2s_in_data;
	for (size_t s = 0; s < this->get_n_data_sockets(); s++)
		p2s_in_data.push_back(this->create_socket_in(p2, "data" + std::to_string(s), n_elmts_select, datatype_select));
	auto p2s_out_data = this->create_socket_out(p2, "data", n_elmts_select, datatype_select);

	this->create_codelet(p2, [p2s_in_data, p2s_out_data](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &swi = static_cast<Switcher&>(m);

		if (!swi.is_no_copy_select())
		{
			const size_t path = swi.get_path();

			const auto data_socket_in  = static_cast<const int8_t*>(t[p2s_in_data[path]].get_dataptr());
			      auto data_socket_out = static_cast<      int8_t*>(t[p2s_out_data     ].get_dataptr());

			std::copy(data_socket_in,
			          data_socket_in + swi.get_n_frames() * swi.get_n_bytes_select(),
			          data_socket_out);
		}

		return status_t::SUCCESS;
	});
}

Switcher
::Switcher(const size_t n_data_sockets,
           const size_t n_elmts,
           const std::type_index datatype)
: Switcher(n_data_sockets, n_elmts, datatype, n_elmts, datatype)
{
}

void Switcher
::set_no_copy_commute(const bool no_copy_commute)
{
	this->no_copy_commute = no_copy_commute;
}

void Switcher
::set_no_copy_select(const bool no_copy_select)
{
	this->no_copy_select = no_copy_select;
}

bool Switcher
::is_no_copy_commute() const
{
	return this->no_copy_commute;
}

bool Switcher
::is_no_copy_select () const
{
	return this->no_copy_select;
}

size_t Switcher
::get_n_data_sockets() const
{
	return this->n_data_sockets;
}

size_t Switcher
::get_n_elmts_commute() const
{
	return this->n_elmts_commute;
}

size_t Switcher
::get_n_elmts_select() const
{
	return this->n_elmts_select;
}

size_t Switcher
::get_n_bytes_commute() const
{
	return this->n_bytes_commute;
}

size_t Switcher
::get_n_bytes_select() const
{
	return this->n_bytes_select;
}

std::type_index Switcher
::get_datatype_commute() const
{
	return this->datatype_commute;
}

std::type_index Switcher
::get_datatype_select() const
{
	return this->datatype_select;
}

size_t Switcher
::get_path() const
{
	return this->path;
}

void Switcher
::set_path(const size_t path)
{
	this->path = path % this->get_n_data_sockets();
}

}
}
