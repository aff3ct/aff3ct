#include <string>
#include <sstream>

#include "Process.hpp"
#include "Socket.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

std::unordered_map<std::type_index,std::string> type_to_string = {{typeid(int8_t ), "int8"   },
                                                                  {typeid(int16_t), "int16"  },
                                                                  {typeid(int32_t), "int32"  },
                                                                  {typeid(int64_t), "int64"  },
                                                                  {typeid(float  ), "float32"},
                                                                  {typeid(double ), "float64"}};

Socket::Socket(Process &attached_process, const std::string name, const std::type_index datatype,
               const size_t databytes, void *dataptr)
: attached_process(attached_process), name(name), datatype(datatype), databytes(databytes), dataptr(dataptr)
{
}

std::string Socket::get_name()
{
	return this->name;
}

std::type_index Socket::get_datatype()
{
	return this->datatype;
}

size_t Socket::get_databytes()
{
	return this->databytes;
}

void* Socket::get_dataptr()
{
	return this->dataptr;
}

void Socket::bind(Socket &s_in)
{
	if (s_in.datatype != this->datatype)
	{
		std::stringstream message;
		message << "'s_in.datatype' has to be equal to 'datatype' ('s_in.datatype' = " << type_to_string[s_in.datatype]
		        << ", 'datatype' = " << type_to_string[this->datatype]
		        << ", 'process_out.name' = " << attached_process.name
		        << ", 'process_in.name' = " << s_in.attached_process.name << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (s_in.databytes != this->databytes)
	{
		std::stringstream message;
		message << "'s_in.databytes' has to be equal to 'databytes' ('s_in.databytes' = " << s_in.databytes
		        << ", 'databytes' = " << this->databytes
		        << ", 'process_out.name' = " << attached_process.name
		        << ", 'process_in.name' = " << s_in.attached_process.name << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->dataptr == nullptr)
	{
		std::stringstream message;
		message << "'dataptr' can't be NULL ('dataptr' = " << dataptr << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	s_in.dataptr = this->dataptr;

	if (s_in.attached_process.is_autostart() && s_in.attached_process.last_input_socket(s_in))
		s_in.attached_process.exec();
}
