#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Router/Router.hpp"

namespace aff3ct
{
namespace module
{

Task& Router
::operator[](const rtr::tsk t)
{
	return Module::operator[]((size_t)t);
}

Socket& Router
::operator[](const rtr::sck::route s)
{
	return Module::operator[]((size_t)rtr::tsk::route)[(size_t)s];
}

Router
::Router(const size_t n_bytes_in, const size_t n_bytes_out, const size_t n_outputs, const int n_frames)
: Module(n_frames),
  n_bytes_in(n_bytes_in),
  n_bytes_out(n_bytes_out),
  n_outputs(n_outputs)
{
	const std::string name = "Router";
	this->set_name(name);
	this->set_short_name(name);

	if (n_bytes_in == 0)
	{
		std::stringstream message;
		message << "'n_bytes_in' has to be greater than 0 ('n_bytes_in' = " << n_bytes_in << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (n_bytes_out == 0)
	{
		std::stringstream message;
		message << "'n_bytes_out' has to be greater than 0 ('n_bytes_out' = " << n_bytes_out << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (n_outputs == 0)
	{
		std::stringstream message;
		message << "'n_outputs' has to be greater than 0 ('n_outputs' = " << n_outputs << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p = this->create_task("route");
	auto ps_in = this->template create_socket_in<int8_t>(p, "in", this->n_bytes_in);
	for (size_t o = 0; o < n_outputs; o++)
		this->template create_socket_in_out<int8_t>(p, "in_out" + std::to_string(o), this->n_bytes_out);

	this->create_codelet(p, [ps_in](Module &m, Task &t) -> int
	{
		return (int)static_cast<Router&>(m).route(static_cast<int8_t*>(t[ps_in].get_dataptr()));
	});
}

size_t Router
::get_n_bytes_in() const
{
	return this->n_bytes_in;
}

size_t Router
::get_n_bytes_out() const
{
	return this->n_bytes_out;
}

size_t Router
::get_n_outputs() const
{
	return this->n_outputs;
}

template <class A>
size_t Router
::route(const std::vector<int8_t,A>& in, const int frame_id)
{
	if (this->n_bytes_in * this->n_frames != (int)in.size())
	{
		std::stringstream message;
		message << "'in.size()' has to be equal to 'n_bytes_in' * 'n_frames' ('in.size()' = " << in.size()
		        << ", 'n_bytes_in' = " << this->n_bytes_in << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->route(in.data(), frame_id);
}

}
}
