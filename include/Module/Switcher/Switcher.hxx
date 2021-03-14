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

Switcher
::Switcher(const size_t n_data_sockets,
           const size_t n_elmts,
           const std::type_index datatype)
: Module(),
  n_data_sockets(n_data_sockets),
  n_elmts(n_elmts),
  n_bytes(tools::compute_bytes(n_elmts, datatype)),
  datatype(datatype)
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

	auto &p = this->create_task("commute");
	const auto ps_in_ctrl = this->create_socket_in(p, "in_ctrl", 1, typeid(int8_t));
	const auto ps_in_data = this->create_socket_in(p, "in_data", n_elmts, datatype);

	std::vector<size_t> ps_out_data;
	for (size_t s = 0; s < this->n_data_sockets; s++)
		ps_out_data.push_back(this->create_socket_in(p, "out_data" + std::to_string(s), n_elmts, datatype));

	this->create_codelet(p, [ps_in_ctrl, ps_in_data, ps_out_data](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &swi = static_cast<Switcher&>(m);

		const auto ctrl_socket_in  = static_cast<const int8_t*>(t[ps_in_ctrl       ].get_dataptr());
		const size_t ctrl = (size_t)ctrl_socket_in[0] % swi->get_n_data_sockets();
		const auto data_socket_in  = static_cast<const int8_t*>(t[ps_in_data       ].get_dataptr());
		      auto data_socket_out = static_cast<      int8_t*>(t[ps_out_data[ctrl]].get_dataptr());

		std::copy(data_socket_in,
		          data_socket_in + swi->get_n_frames() * swi->get_n_bytes(),
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
::get_n_calls() const
{
	return this->n_calls;
}

}
}
