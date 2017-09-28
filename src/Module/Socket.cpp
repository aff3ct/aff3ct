#include <string>
#include <sstream>

#include "Module.hpp"
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

std::unordered_map<std::type_index,uint8_t> type_to_size = {{typeid(int8_t ), 1},
                                                            {typeid(int16_t), 2},
                                                            {typeid(int32_t), 4},
                                                            {typeid(int64_t), 8},
                                                            {typeid(float  ), 4},
                                                            {typeid(double ), 8}};

Socket::Socket(Process &process, const std::string name, const std::type_index datatype, const size_t databytes,
               void *dataptr)
: process(process), name(name), datatype(datatype), databytes(databytes), dataptr(dataptr)
{
}

std::string Socket::get_name() const
{
	return this->name;
}

std::type_index Socket::get_datatype() const
{
	return this->datatype;
}

std::string Socket::get_datatype_string() const
{
	return type_to_string[this->datatype];
}

uint8_t Socket::get_datatype_size() const
{
	return type_to_size[this->datatype];
}

size_t Socket::get_databytes() const
{
	return this->databytes;
}

size_t Socket::get_n_elmts() const
{
	return this->get_databytes() / (size_t)this->get_datatype_size();
}

void* Socket::get_dataptr() const
{
	return this->dataptr;
}

int Socket::bind(Socket &s)
{
	if (s.datatype != this->datatype)
	{
		std::stringstream message;
		message << "'s.datatype' has to be equal to 'datatype' ('s.datatype' = " << type_to_string[s.datatype]
		        << ", 'datatype' = " << type_to_string[this->datatype]
		        << ", 'name' = " << get_name()
		        << ", 's.name' = " << s.get_name()
		        << ", 'process.name' = " << process.get_name()
		        << ", 's.process.name' = " << s.process.get_name()
		        << ", 'process.module.name' = " << process.module.get_name()
		        << ", 's.process.module.name' = " << s.process.module.get_name() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (s.databytes != this->databytes)
	{
		std::stringstream message;
		message << "'s.databytes' has to be equal to 'databytes' ('s.databytes' = " << s.databytes
		        << ", 'databytes' = " << this->databytes
		        << ", 'name' = " << get_name()
		        << ", 's.name' = " << s.get_name()
		        << ", 'process.name' = " << process.get_name()
		        << ", 's.process.name' = " << s.process.get_name()
		        << ", 'process.module.name' = " << process.module.get_name()
		        << ", 's.process.module.name' = " << s.process.module.get_name() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (s.dataptr == nullptr)
	{
		std::stringstream message;
		message << "'s.dataptr' can't be NULL.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->dataptr = s.dataptr;

	if (this->process.is_autoexec() && this->process.last_input_socket(*this))
		return this->process.exec();
	else
		return 0;
}

template <typename T, class A>
int Socket::bind(std::vector<T,A> &vector)
{
	if (vector.size() != this->get_n_elmts())
	{
		std::stringstream message;
		message << "'vector.size()' has to be equal to 'get_n_elmts()' ('vector.size()' = " << vector.size()
		        << ", 'get_n_elmts()' = " << get_n_elmts()
		        << ", 'name' = " << get_name()
		        << ", 'process.name' = " << process.get_name()
		        << ", 'process.module.name' = " << process.module.get_name() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return bind(vector.data());
}

template <typename T>
int Socket::bind(T *array)
{
	if (type_to_string[typeid(T)] != type_to_string[this->datatype])
	{
		std::stringstream message;
		message << "'T' has to be equal to 'datatype' ('T' = " << type_to_string[typeid(T)]
		        << ", 'datatype' = " << type_to_string[this->datatype]
		        << ", 'socket.name' = " << get_name()
		        << ", 'process.name' = " << process.get_name()
		        << ", 'module.name' = " << process.module.get_name() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return bind(static_cast<void*>(array));
}

int Socket::bind(void* dataptr)
{
	if (dataptr == nullptr)
	{
		std::stringstream message;
		message << "'s.dataptr' can't be NULL.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->dataptr = dataptr;

	return 0;
}

// ==================================================================================== explicit template instantiation
template int aff3ct::module::Socket::bind<int8_t >(int8_t *);
template int aff3ct::module::Socket::bind<int16_t>(int16_t*);
template int aff3ct::module::Socket::bind<int32_t>(int32_t*);
template int aff3ct::module::Socket::bind<int64_t>(int64_t*);
template int aff3ct::module::Socket::bind<float  >(float  *);
template int aff3ct::module::Socket::bind<double >(double *);

template int aff3ct::module::Socket::bind<int8_t >(std::vector<int8_t >&);
template int aff3ct::module::Socket::bind<int16_t>(std::vector<int16_t>&);
template int aff3ct::module::Socket::bind<int32_t>(std::vector<int32_t>&);
template int aff3ct::module::Socket::bind<int64_t>(std::vector<int64_t>&);
template int aff3ct::module::Socket::bind<float  >(std::vector<float  >&);
template int aff3ct::module::Socket::bind<double >(std::vector<double >&);

#include <mipp.h>
template int aff3ct::module::Socket::bind<int8_t ,mipp::allocator<int8_t >>(std::vector<int8_t ,mipp::allocator<int8_t >>&);
template int aff3ct::module::Socket::bind<int16_t,mipp::allocator<int16_t>>(std::vector<int16_t,mipp::allocator<int16_t>>&);
template int aff3ct::module::Socket::bind<int32_t,mipp::allocator<int32_t>>(std::vector<int32_t,mipp::allocator<int32_t>>&);
template int aff3ct::module::Socket::bind<int64_t,mipp::allocator<int64_t>>(std::vector<int64_t,mipp::allocator<int64_t>>&);
template int aff3ct::module::Socket::bind<float  ,mipp::allocator<float  >>(std::vector<float  ,mipp::allocator<float  >>&);
template int aff3ct::module::Socket::bind<double ,mipp::allocator<double >>(std::vector<double ,mipp::allocator<double >>&);
// ==================================================================================== explicit template instantiation
