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
: task(task), name(name), datatype(datatype), databytes(databytes), fast(fast), dataptr(dataptr), bound_socket(nullptr)
{
}

const std::string& Socket
::get_name() const
{
	return name;
}

const std::type_index& Socket
::get_datatype() const
{
	return datatype;
}

const std::string& Socket
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

Task& Socket
::get_task() const
{
	return this->task;
}

const std::vector<Socket*> Socket
::get_bound_sockets() const
{
	return this->bound_sockets;
}

const Socket& Socket
::get_bound_socket() const
{
	if (this->bound_socket == nullptr)
	{
		std::stringstream message;
		message << "bound_socket can't be nullptr.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
	return *this->bound_socket;
}

void Socket
::set_fast(const bool fast)
{
	this->fast = fast;
}

void Socket
::bind(Socket &s_out, const bool copy_dataptr)
{
	if (!is_fast())
	{
		if (s_out.datatype != this->datatype)
		{
			std::stringstream message;
			message << "'s_out.datatype' has to be equal to 'datatype' ("
			        << "'s_out.datatype'"         << " = " << type_to_string[s_out.datatype] << ", "
			        << "'s_out.name'"             << " = " << s_out.get_name()               << ", "
			        << "'s_out.task.name'"        << " = " << s_out.task.get_name()          << ", "
//			        << "'s_out.task.module.name'" << " = " << s_out.task.get_module_name()   << ", "
			        << "'datatype'"               << " = " << type_to_string[this->datatype] << ", "
			        << "'name'"                   << " = " << get_name()                     << ", "
			        << "'task.name'"              << " = " << task.get_name()                << ", "
//			        << "'task.module.name'"       << " = " << task.get_module_name()
			        << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (s_out.databytes != this->databytes)
		{
			std::stringstream message;
			message << "'s_out.databytes' has to be equal to 'databytes' ("
			        << "'s_out.databytes'"        << " = " << s_out.databytes              << ", "
			        << "'s_out.name'"             << " = " << s_out.get_name()             << ", "
			        << "'s_out.task.name'"        << " = " << s_out.task.get_name()        << ", "
//			        << "'s_out.task.module.name'" << " = " << s_out.task.get_module_name() << ", "
			        << "'databytes'"              << " = " << this->databytes              << ", "
			        << "'name'"                   << " = " << get_name()                   << ", "
			        << "'task.name'"              << " = " << task.get_name()              << ", "
//			        << "'task.module.name'"       << " = " << task.get_module_name()
			        << ").";

			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (copy_dataptr && s_out.dataptr == nullptr)
		{
			std::stringstream message;
			message << "'s_out.dataptr' can't be NULL.";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}
	}

	this->bound_socket = &s_out;
	s_out.bound_sockets.push_back(this);
	if (copy_dataptr)
		this->dataptr = s_out.dataptr;
}

void Socket
::operator()(Socket &s_out, const bool copy_dataptr)
{
	bind(s_out, copy_dataptr);
}

template <typename T, class A>
void Socket
::bind(std::vector<T,A> &vector)
{
	if (is_fast())
		this->dataptr = static_cast<void*>(vector.data());

	if (vector.size() != this->get_n_elmts())
	{
		std::stringstream message;
		message << "'vector.size()' has to be equal to 'get_n_elmts()' ('vector.size()' = " << vector.size()
		        << ", 'get_n_elmts()' = " << get_n_elmts()
		        << ", 'name' = " << get_name()
		        << ", 'task.name' = " << task.get_name()
//		        << ", 'task.module.name' = " << task.get_module_name()
		        << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	bind(vector.data());
}

template <typename T, class A>
void Socket
::operator()(std::vector<T,A> &vector)
{
	bind(vector);
}

template <typename T>
void Socket
::bind(T *array)
{
	if (is_fast())
		this->dataptr = static_cast<void*>(array);

	if (type_to_string[typeid(T)] != type_to_string[this->datatype])
	{
		std::stringstream message;
		message << "'T' has to be equal to 'datatype' ('T' = " << type_to_string[typeid(T)]
		        << ", 'datatype' = " << type_to_string[this->datatype]
		        << ", 'socket.name' = " << get_name()
		        << ", 'task.name' = " << task.get_name()
//		        << ", 'module.name' = " << task.get_module_name()
		        << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	bind(static_cast<void*>(array));
}

template <typename T>
void Socket
::operator()(T *array)
{
	bind(array);
}

void Socket
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
}

void Socket
::operator()(void* dataptr)
{
	bind(dataptr);
}

void Socket
::reset()
{
	this->dataptr = nullptr;
	this->bound_sockets.clear();
}

}
}
