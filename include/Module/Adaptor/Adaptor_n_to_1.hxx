#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Adaptor/Adaptor_n_to_1.hpp"

namespace aff3ct
{
namespace module
{

Task& Adaptor_n_to_1
::operator[](const adp::tsk t)
{
	return Module::operator[]((size_t)t);
}

Socket& Adaptor_n_to_1
::operator[](const adp::sck::put_n s)
{
	return Module::operator[]((size_t)adp::tsk::put_n)[(size_t)s];
}

Socket& Adaptor_n_to_1
::operator[](const adp::sck::pull_1 s)
{
	return Module::operator[]((size_t)adp::tsk::pull_1)[(size_t)s];
}

Adaptor_n_to_1
::Adaptor_n_to_1(const size_t n_elmts,
                 const std::type_index datatype,
                 const size_t buffer_size,
                 const int n_frames)
: Adaptor(n_elmts, datatype, buffer_size, n_frames)
{
	const std::string name = "Adaptor_n_to_1";
	this->set_name(name);
	this->set_short_name(name);

	std::function<size_t(Task&, const size_t, const std::type_index&)> create_socket_in =
		[this](Task& p, const size_t n_elmts, const std::type_index& datatype)
		{
			std::string n = "in";
			     if (datatype == typeid(int8_t )) return this->template create_socket_in<int8_t >(p, n, n_elmts);
			else if (datatype == typeid(int16_t)) return this->template create_socket_in<int16_t>(p, n, n_elmts);
			else if (datatype == typeid(int32_t)) return this->template create_socket_in<int32_t>(p, n, n_elmts);
			else if (datatype == typeid(int64_t)) return this->template create_socket_in<int64_t>(p, n, n_elmts);
			else if (datatype == typeid(float  )) return this->template create_socket_in<float  >(p, n, n_elmts);
			else if (datatype == typeid(double )) return this->template create_socket_in<double >(p, n, n_elmts);
			else
			{
				std::stringstream message;
				message << "This should never happen.";
				throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
			}
		};

	std::function<size_t(Task&, const size_t, const std::type_index&)> create_socket_out =
		[this](Task& p, const size_t n_elmts, const std::type_index& datatype)
		{
			std::string n = "out";
			     if (datatype == typeid(int8_t )) return this->template create_socket_out<int8_t >(p, n, n_elmts);
			else if (datatype == typeid(int16_t)) return this->template create_socket_out<int16_t>(p, n, n_elmts);
			else if (datatype == typeid(int32_t)) return this->template create_socket_out<int32_t>(p, n, n_elmts);
			else if (datatype == typeid(int64_t)) return this->template create_socket_out<int64_t>(p, n, n_elmts);
			else if (datatype == typeid(float  )) return this->template create_socket_out<float  >(p, n, n_elmts);
			else if (datatype == typeid(double )) return this->template create_socket_out<double >(p, n, n_elmts);
			else
			{
				std::stringstream message;
				message << "This should never happen.";
				throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
			}
		};

	auto &p1 = this->create_task("put_n", (int)adp::tsk::put_n);
	auto p1s_in = create_socket_in(p1, n_elmts, datatype);
	this->create_codelet(p1, [p1s_in](Module &m, Task &t) -> int
	{
		static_cast<Adaptor_n_to_1&>(m).put_n(static_cast<int8_t*>(t[p1s_in].get_dataptr()));
		return 0;
	});

	auto &p2 = this->create_task("pull_1", (int)adp::tsk::pull_1);
	auto p2s_out = create_socket_out(p2, n_elmts, datatype);
	this->create_codelet(p2, [p2s_out](Module &m, Task &t) -> int
	{
		static_cast<Adaptor_n_to_1&>(m).pull_1(static_cast<int8_t*>(t[p2s_out].get_dataptr()));
		return 0;
	});
}

template <class A>
void Adaptor_n_to_1
::put_n(const std::vector<int8_t,A>& in, const int frame_id)
{
	if (this->n_bytes * (size_t)this->n_frames != in.size())
	{
		std::stringstream message;
		message << "'in.size()' has to be equal to 'n_bytes' * 'n_frames' ('in.size()' = " << in.size()
		        << ", 'n_bytes' = " << this->n_bytes << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->put_n(in.data(), frame_id);
}

template <class A>
void Adaptor_n_to_1
::pull_1(std::vector<int8_t,A>& out, const int frame_id)
{
	if (this->n_bytes * (size_t)this->n_frames != out.size())
	{
		std::stringstream message;
		message << "'out.size()' has to be equal to 'n_bytes' * 'n_frames' ('out.size()' = " << out.size()
		        << ", 'n_bytes' = " << this->n_bytes << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->pull_1(out.data(), frame_id);
}

}
}
