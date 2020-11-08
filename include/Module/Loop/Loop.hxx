#include <string>
#include <sstream>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Module/Loop/Loop.hpp"

namespace aff3ct
{
namespace module
{

Task& Loop
::operator[](const lop::tsk t)
{
	return Module::operator[]((size_t)t);
}

Socket& Loop
::operator[](const lop::sck::stop s)
{
	return Module::operator[]((size_t)lop::tsk::stop)[(size_t)s];
}

Loop
::Loop(const size_t n_elmts_in,
       const std::type_index datatype_in,
       const size_t n_elmts_out,
       const std::type_index datatype_out)
: Module(),
  n_elmts_in(n_elmts_in),
  n_bytes_in(compute_bytes(n_elmts_in, datatype_in)),
  datatype_in(datatype_in),
  n_elmts_out(n_elmts_out),
  n_bytes_out(compute_bytes(n_elmts_out, datatype_out)),
  datatype_out(datatype_out),
  n_calls(0)
{
	const std::string name = "Loop";
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

	auto &p = this->create_task("stop");

	for (auto i = 1; i <= 2; i++)
	{
		std::string n = "in"+std::to_string(i);
		     if (datatype_in == typeid(int8_t )) this->template create_socket_in<int8_t >(p, n, this->n_elmts_in);
		else if (datatype_in == typeid(int16_t)) this->template create_socket_in<int16_t>(p, n, this->n_elmts_in);
		else if (datatype_in == typeid(int32_t)) this->template create_socket_in<int32_t>(p, n, this->n_elmts_in);
		else if (datatype_in == typeid(int64_t)) this->template create_socket_in<int64_t>(p, n, this->n_elmts_in);
		else if (datatype_in == typeid(float  )) this->template create_socket_in<float  >(p, n, this->n_elmts_in);
		else if (datatype_in == typeid(double )) this->template create_socket_in<double >(p, n, this->n_elmts_in);
		else
		{
			std::stringstream message;
			message << "This should never happen.";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}
	}

	for (auto io = 1; io <= 2; io++)
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

	this->create_codelet(p, [](Module &m, Task &t, const int frame_id) -> int
	{
		auto &lop = static_cast<Loop&>(m);

		auto n_calls = lop.get_n_calls();
		auto stop = (int)lop._stop(static_cast<int8_t*>(t[n_calls ? 1 : 0].get_dataptr()),
		                           frame_id);

		// TODO: find a trick to avoid those copies (tips: use processes in the sequence)
		if (stop && n_calls == 0)
			std::copy((int8_t*)t[2].get_dataptr(),
			          (int8_t*)t[2].get_dataptr() + t[2].get_databytes(),
			          (int8_t*)t[3].get_dataptr());
		if (!stop && n_calls > 0)
			std::copy((int8_t*)t[3].get_dataptr(),
			          (int8_t*)t[3].get_dataptr() + t[3].get_databytes(),
			          (int8_t*)t[2].get_dataptr());

		return stop;
	});
}

size_t Loop
::get_n_elmts_in() const
{
	return this->n_elmts_in;
}

size_t Loop
::get_n_bytes_in() const
{
	return this->n_bytes_in;
}

std::type_index Loop
::get_datatype_in() const
{
	return this->datatype_in;
}

size_t Loop
::get_n_elmts_out() const
{
	return this->n_elmts_out;
}

size_t Loop
::get_n_bytes_out() const
{
	return this->n_bytes_out;
}

std::type_index Loop
::get_datatype_out() const
{
	return this->datatype_out;
}

size_t Loop
::get_n_calls() const
{
	return this->n_calls;
}

// template <class A>
// bool Loop
// ::stop(const std::vector<int8_t,A>& in, const int frame_id, const bool managed_memory)
// {
// 	(*this)[lop::sck::stop::in1].bind(in);
// 	return (*this)[lop::tsk::stop].exec(frame_id, managed_memory);
// }

}
}
