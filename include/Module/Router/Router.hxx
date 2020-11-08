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
::Router(const size_t n_elmts_in,
         const std::type_index datatype_in,
         const size_t n_elmts_out,
         const std::type_index datatype_out,
         const size_t n_outputs)
: Module(),
  n_elmts_in(n_elmts_in),
  n_bytes_in(compute_bytes(n_elmts_in, datatype_in)),
  datatype_in(datatype_in),
  n_elmts_out(n_elmts_out),
  n_bytes_out(compute_bytes(n_elmts_out, datatype_out)),
  datatype_out(datatype_out),
  n_outputs(n_outputs)
{
	const std::string name = "Router";
	this->set_name(name);
	this->set_short_name(name);
	this->set_single_wave(true);

	if (n_elmts_in == 0)
	{
		std::stringstream message;
		message << "'n_elmts_in' has to be greater than 0 ('n_elmts_in' = " << n_elmts_in << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (n_elmts_out == 0)
	{
		std::stringstream message;
		message << "'n_elmts_out' has to be greater than 0 ('n_elmts_out' = " << n_elmts_out << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (n_outputs == 0)
	{
		std::stringstream message;
		message << "'n_outputs' has to be greater than 0 ('n_outputs' = " << n_outputs << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p = this->create_task("route");

	size_t ps_in = 0;
	     if (datatype_in == typeid(int8_t )) ps_in = this->template create_socket_in<int8_t >(p, "in", this->n_elmts_in);
	else if (datatype_in == typeid(int16_t)) ps_in = this->template create_socket_in<int16_t>(p, "in", this->n_elmts_in);
	else if (datatype_in == typeid(int32_t)) ps_in = this->template create_socket_in<int32_t>(p, "in", this->n_elmts_in);
	else if (datatype_in == typeid(int64_t)) ps_in = this->template create_socket_in<int64_t>(p, "in", this->n_elmts_in);
	else if (datatype_in == typeid(float  )) ps_in = this->template create_socket_in<float  >(p, "in", this->n_elmts_in);
	else if (datatype_in == typeid(double )) ps_in = this->template create_socket_in<double >(p, "in", this->n_elmts_in);
	else
	{
		std::stringstream message;
		message << "This should never happen.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	for (size_t io = 0; io < n_outputs; io++)
	{
		std::string n = "in_out"+std::to_string(io);
		     if (datatype_out == typeid(int8_t )) this->template create_socket_in_out<int8_t >(p, n, this->n_elmts_out);
		else if (datatype_out == typeid(int16_t)) this->template create_socket_in_out<int16_t>(p, n, this->n_elmts_out);
		else if (datatype_out == typeid(int32_t)) this->template create_socket_in_out<int32_t>(p, n, this->n_elmts_out);
		else if (datatype_out == typeid(int64_t)) this->template create_socket_in_out<int64_t>(p, n, this->n_elmts_out);
		else if (datatype_out == typeid(float  )) this->template create_socket_in_out<float  >(p, n, this->n_elmts_out);
		else if (datatype_out == typeid(double )) this->template create_socket_in_out<double >(p, n, this->n_elmts_out);
		else
		{
			std::stringstream message;
			message << "This should never happen.";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}
	}

	this->create_codelet(p, [ps_in](Module &m, Task &t, const int frame_id) -> int
	{
		auto &rtr = static_cast<Router&>(m);

		auto ret = rtr._route(static_cast<int8_t*>(t[ps_in].get_dataptr()),
		                      frame_id);

		return (int)ret;
	});
}

size_t Router
::get_n_bytes_in() const
{
	return this->n_bytes_in;
}

size_t Router
::get_n_elmts_in() const
{
	return this->n_elmts_in;
}

std::type_index Router
::get_datatype_in() const
{
	return this->datatype_in;
}

size_t Router
::get_n_bytes_out() const
{
	return this->n_bytes_out;
}

size_t Router
::get_n_elmts_out() const
{
	return this->n_elmts_out;
}

size_t Router
::get_n_outputs() const
{
	return this->n_outputs;
}

std::type_index Router
::get_datatype_out() const
{
	return this->datatype_out;
}

template <class A>
size_t Router
::route(const std::vector<int8_t,A>& in, const int frame_id, const bool managed_memory)
{
	(*this)[rtr::sck::route::in].bind(in);
	return (size_t)(*this)[rtr::tsk::route].exec(frame_id, managed_memory);
}

}
}
