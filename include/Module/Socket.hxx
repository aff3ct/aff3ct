#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Socket.hpp"

namespace aff3ct
{
namespace module
{
static std::unordered_map<std::type_index,std::string> type_to_string = {{typeid(int8_t ), "int8"   },
                                                                         {typeid(int16_t), "int16"  },
                                                                         {typeid(int32_t), "int32"  },
                                                                         {typeid(int64_t), "int64"  },
                                                                         {typeid(float  ), "float32"},
                                                                         {typeid(double ), "float64"}};

static std::unordered_map<std::type_index,uint8_t> type_to_size = {{typeid(int8_t ), 1},
                                                                   {typeid(int16_t), 2},
                                                                   {typeid(int32_t), 4},
                                                                   {typeid(int64_t), 8},
                                                                   {typeid(float  ), 4},
                                                                   {typeid(double ), 8}};

Socket
::Socket(Task &task, const std::string &name, const std::type_index datatype, const size_t databytes,
         const bool fast, void *dataptr)
: task(task), name(name), datatype(datatype), databytes(databytes), fast(fast), dataptr(dataptr)
{
}

std::string Socket
::get_name() const
{
	return name;
}

std::type_index Socket
::get_datatype() const
{
	return datatype;
}

std::string Socket
::get_datatype_string() const
{
	return type_to_string[datatype];
}

uint8_t Socket
::get_datatype_size() const
{
	return type_to_size[datatype];
}

size_t Socket
::get_databytes() const
{
	return databytes;
}

size_t Socket
::get_n_elmts() const
{
	return get_databytes() / (size_t)get_datatype_size();
}

void* Socket
::get_dataptr() const
{
	return dataptr;
}

bool Socket
::is_fast() const
{
	return fast;
}

void Socket
::set_fast(const bool fast)
{
	this->fast = fast;
}

int Socket
::bind(Socket &s)
{
	if (!is_fast())
	{
		if (s.datatype != this->datatype)
		{
			std::stringstream message;
			message << "'s.datatype' has to be equal to 'datatype' ('s.datatype' = " << type_to_string[s.datatype]
			        << ", 'datatype' = " << type_to_string[this->datatype]
			        << ", 'name' = " << get_name()
			        << ", 's.name' = " << s.get_name()
			        << ", 'task.name' = " << task.get_name()
			        << ", 's.task.name' = " << s.task.get_name()
//				        << ", 'task.module.name' = " << task.get_module_name()
//				        << ", 's.task.module.name' = " << s.task.get_module_name()
			        << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (s.databytes != this->databytes)
		{
			std::stringstream message;
			message << "'s.databytes' has to be equal to 'databytes' ('s.databytes' = " << s.databytes
			        << ", 'databytes' = " << this->databytes
			        << ", 'name' = " << get_name()
			        << ", 's.name' = " << s.get_name()
			        << ", 'task.name' = " << task.get_name()
			        << ", 's.task.name' = " << s.task.get_name()
//				        << ", 'task.module.name' = " << task.get_module_name()
//				        << ", 's.task.module.name' = " << s.task.get_module_name()
			        << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (s.dataptr == nullptr)
		{
			std::stringstream message;
			message << "'s.dataptr' can't be NULL.";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}
	}

	this->dataptr = s.dataptr;

	if (this->task.is_autoexec() && this->task.is_last_input_socket(*this))
		return this->task.exec();
	else
		return 0;
}

int Socket
::operator()(Socket &s)
{
	return bind(s);
}

template <typename T, class A>
int Socket
::bind(std::vector<T,A> &vector)
{
	if (is_fast())
	{
		this->dataptr = static_cast<void*>(vector.data());
		return 0;
	}

	if (vector.size() != this->get_n_elmts())
	{
		std::stringstream message;
		message << "'vector.size()' has to be equal to 'get_n_elmts()' ('vector.size()' = " << vector.size()
		        << ", 'get_n_elmts()' = " << get_n_elmts()
		        << ", 'name' = " << get_name()
		        << ", 'task.name' = " << task.get_name()
//			        << ", 'task.module.name' = " << task.get_module_name()
		        << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return bind(vector.data());
}

template <typename T, class A>
int Socket
::operator()(std::vector<T,A> &vector)
{
	return bind(vector);
}

template <typename T>
int Socket
::bind(T *array)
{
	if (is_fast())
	{
		this->dataptr = static_cast<void*>(array);
		return 0;
	}

	if (type_to_string[typeid(T)] != type_to_string[this->datatype])
	{
		std::stringstream message;
		message << "'T' has to be equal to 'datatype' ('T' = " << type_to_string[typeid(T)]
		        << ", 'datatype' = " << type_to_string[this->datatype]
		        << ", 'socket.name' = " << get_name()
		        << ", 'task.name' = " << task.get_name()
//			        << ", 'module.name' = " << task.get_module_name()
		        << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return bind(static_cast<void*>(array));
}

template <typename T>
int Socket
::operator()(T *array)
{
	return bind(array);
}

int Socket
::bind(void* dataptr)
{
	if (!is_fast())
	{
		if (dataptr == nullptr)
		{
			std::stringstream message;
			message << "'s.dataptr' can't be NULL.";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}
	}

	this->dataptr = dataptr;

	return 0;
}

int Socket
::operator()(void* dataptr)
{
	return bind(dataptr);
}
}
}
