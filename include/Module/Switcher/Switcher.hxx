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
           const size_t n_elmts,
           const std::type_index datatype)
: Module(),
  n_data_sockets(n_data_sockets),
  n_elmts(n_elmts),
  n_bytes(tools::compute_bytes(n_elmts, datatype)),
  datatype(datatype),
  path(n_data_sockets -1)
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

	if (n_elmts == 0)
	{
		std::stringstream message;
		message << "'n_elmts' has to be greater than 0 ('n_elmts' = " << n_elmts << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p1 = this->create_task("commute");
	const auto p1s_in_data = this->create_socket_in(p1, "data", n_elmts, datatype);
	const auto p1s_in_ctrl = this->create_socket_in(p1, "ctrl", 1, typeid(int8_t));

	std::vector<size_t> p1s_out_data;
	for (size_t s = 0; s < this->get_n_data_sockets(); s++)
		p1s_out_data.push_back(this->create_socket_out(p1, "data" + std::to_string(s), n_elmts, datatype));

	this->create_codelet(p1, [p1s_in_data, p1s_in_ctrl, p1s_out_data](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &swi = static_cast<Switcher&>(m);

		const auto ctrl_socket_in  = static_cast<const int8_t*>(t[p1s_in_ctrl].get_dataptr());
		swi.set_path((size_t)ctrl_socket_in[0]);
		const size_t path = swi.get_path();

		const auto data_socket_in  = static_cast<const int8_t*>(t[p1s_in_data       ].get_dataptr());
		      auto data_socket_out = static_cast<      int8_t*>(t[p1s_out_data[path]].get_dataptr());

		std::copy(data_socket_in,
		          data_socket_in + swi.get_n_frames() * swi.get_n_bytes(),
		          data_socket_out);

		return (int)path;
	});

	auto &p2 = this->create_task("select");
	std::vector<size_t> p2s_in_data;
	for (size_t s = 0; s < this->get_n_data_sockets(); s++)
		p2s_in_data.push_back(this->create_socket_in(p2, "data" + std::to_string(s), n_elmts, datatype));
	auto p2s_out_data = this->create_socket_out(p2, "data", n_elmts, datatype);

	this->create_codelet(p2, [p2s_in_data, p2s_out_data](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &swi = static_cast<Switcher&>(m);
		const size_t path = swi.get_path();

		const auto data_socket_in  = static_cast<const int8_t*>(t[p2s_in_data[path]].get_dataptr());
		      auto data_socket_out = static_cast<      int8_t*>(t[p2s_out_data     ].get_dataptr());

		std::copy(data_socket_in,
		          data_socket_in + swi.get_n_frames() * swi.get_n_bytes(),
		          data_socket_out);

		return status_t::SUCCESS;
	});
}

size_t Switcher
::get_n_data_sockets() const
{
	return this->n_data_sockets;
}

size_t Switcher
::get_n_elmts() const
{
	return this->n_elmts;
}

size_t Switcher
::get_n_bytes() const
{
	return this->n_bytes;
}

std::type_index Switcher
::get_datatype() const
{
	return this->datatype;
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
