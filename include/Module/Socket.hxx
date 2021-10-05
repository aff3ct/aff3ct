#include <algorithm>
#include <iterator>
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
         const socket_t type, const bool fast, void *dataptr)
: task(task), name(name), datatype(datatype), databytes(databytes), fast(fast), dataptr(dataptr), bound_socket(nullptr),
  type(type)
{
	if (databytes % type_to_size[datatype] != 0)
	{
		std::stringstream message;
		message << "'databytes % type_to_size[datatype]' has to be equal to 0 ("
		        << "'databytes'"              << " = " << databytes                << ", "
		        << "'type_to_size[datatype]'" << " = " << type_to_size[datatype]   << ", "
		        << "'datatype'"               << " = " << type_to_string[datatype] << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
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

const std::vector<Socket*>& Socket
::get_bound_sockets() const
{
	return this->bound_sockets;
}

Socket& Socket
::get_bound_socket()
{
	if (this->bound_socket == nullptr)
	{
		std::stringstream message;
		message << "bound_socket can't be nullptr.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
	return *this->bound_socket;
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

socket_t Socket
::get_type() const
{
	return this->type;
}

void Socket
::set_fast(const bool fast)
{
	this->fast = fast;
}

void Socket
::bind(Socket &s_out, const int priority)
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
			        << "'task.name'"              << " = " << task.get_name()              /*<< ", "*/
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
			        << "'task.name'"              << " = " << task.get_name()            /*<< ", "*/
//			        << "'task.module.name'"       << " = " << task.get_module_name()
			        << ").";

			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (s_out.dataptr == nullptr)
		{
			std::stringstream message;
			message << "'s_out.dataptr' can't be NULL.";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}
	}

	if (this->bound_socket == &s_out)
		this->unbind(s_out);

	if (this->bound_socket != nullptr)
	{
		std::stringstream message;
		message << "This socket is already connected ("
		        << "'bound_socket->databytes'"        << " = " << this->bound_socket->databytes              << ", "
		        << "'bound_socket->name'"             << " = " << this->bound_socket->get_name()             << ", "
		        << "'bound_socket->task.name'"        << " = " << this->bound_socket->task.get_name()        << ", "
//		        << "'bound_socket->task.module.name'" << " = " << this->bound_socket->task.get_module_name() << ", "
		        << "'s_out.databytes'"                << " = " << s_out.databytes                            << ", "
		        << "'s_out.name'"                     << " = " << s_out.get_name()                           << ", "
		        << "'s_out.task.name'"                << " = " << s_out.task.get_name()                      << ", "
//		        << "'s_out.task.module.name'"         << " = " << s_out.task.get_module_name()               << ", "
		        << "'databytes'"                      << " = " << this->databytes                            << ", "
		        << "'name'"                           << " = " << get_name()                                 << ", "
		        << "'task.name'"                      << " = " << task.get_name()                          /*<< ", "*/
//		        << "'task.module.name'"               << " = " << task.get_module_name()
		        << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->bound_socket = &s_out;

	if (std::find(s_out.bound_sockets.begin(), s_out.bound_sockets.end(), this) != s_out.bound_sockets.end())
	{
		std::stringstream message;
		message << "It is not possible to bind the same socket twice ("
		        << "'s_out.databytes'"        << " = " << s_out.databytes              << ", "
		        << "'s_out.name'"             << " = " << s_out.get_name()             << ", "
		        << "'s_out.task.name'"        << " = " << s_out.task.get_name()        << ", "
//		        << "'s_out.task.module.name'" << " = " << s_out.task.get_module_name() << ", "
		        << "'databytes'"              << " = " << this->databytes              << ", "
		        << "'name'"                   << " = " << get_name()                   << ", "
		        << "'task.name'"              << " = " << task.get_name()            /*<< ", "*/
//		        << "'task.module.name'"       << " = " << task.get_module_name()
		        << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((size_t)priority > s_out.bound_sockets.size() || priority == -1)
		s_out.bound_sockets.push_back(this);
	else
		s_out.bound_sockets.insert(s_out.bound_sockets.begin() + priority, this);
	this->dataptr = s_out.dataptr;
}

void Socket
::operator()(Socket &s_out, const int priority)
{
	bind(s_out, priority);
}

template <typename T>
void Socket
::operator=(const void *array)
{
	if (this->get_type() == socket_t::SIN)
		this->bind(array);
	else
	{
		std::stringstream message;
		message << "Current socket have to be an input socket ("
		        << "'datatype'"           << " = " << type_to_string[this->datatype] << ", "
		        << "'name'"               << " = " << get_name()                     << ", "
		        << "'task.name'"          << " = " << task.get_name()                << ", "
		        << "'type'"               << " = " << (get_type() == socket_t::SIN ? "SIN" : "SOUT") /*<< ", "*/
//		        << "'task.module.name'"   << " = " << task.get_module_name()
		        << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename T>
void Socket
::operator=(void *array)
{
	if (this->get_type() == socket_t::SIN)
		this->bind(array);
	else
	{
		std::stringstream message;
		message << "Current socket have to be an input socket ("
		        << "'datatype'"           << " = " << type_to_string[this->datatype] << ", "
		        << "'name'"               << " = " << get_name()                     << ", "
		        << "'task.name'"          << " = " << task.get_name()                << ", "
		        << "'type'"               << " = " << (get_type() == socket_t::SIN ? "SIN" : "SOUT") /*<< ", "*/
//		        << "'task.module.name'"   << " = " << task.get_module_name()
		        << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename T>
void Socket
::operator=(const T *array)
{
	if (this->get_type() == socket_t::SIN)
		this->bind(array);
	else
	{
		std::stringstream message;
		message << "Current socket have to be an input socket ("
		        << "'datatype'"           << " = " << type_to_string[this->datatype] << ", "
		        << "'name'"               << " = " << get_name()                     << ", "
		        << "'task.name'"          << " = " << task.get_name()                << ", "
		        << "'type'"               << " = " << (get_type() == socket_t::SIN ? "SIN" : "SOUT") /*<< ", "*/
//		        << "'task.module.name'"   << " = " << task.get_module_name()
		        << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename T>
void Socket
::operator=(T *array)
{
	if (this->get_type() == socket_t::SIN)
		this->bind(array);
	else
	{
		std::stringstream message;
		message << "Current socket have to be an input socket ("
		        << "'datatype'"           << " = " << type_to_string[this->datatype] << ", "
		        << "'name'"               << " = " << get_name()                     << ", "
		        << "'task.name'"          << " = " << task.get_name()                << ", "
		        << "'type'"               << " = " << (get_type() == socket_t::SIN ? "SIN" : "SOUT") /*<< ", "*/
//		        << "'task.module.name'"   << " = " << task.get_module_name()
		        << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename T, class A>
void Socket
::operator=(const std::vector<T,A> &vector)
{
	if (this->get_type() == socket_t::SIN)
		this->bind(vector);
	else
	{
		std::stringstream message;
		message << "Current socket have to be an input socket ("
		        << "'datatype'"           << " = " << type_to_string[this->datatype] << ", "
		        << "'name'"               << " = " << get_name()                     << ", "
		        << "'task.name'"          << " = " << task.get_name()                << ", "
		        << "'type'"               << " = " << (get_type() == socket_t::SIN ? "SIN" : "SOUT") /*<< ", "*/
//		        << "'task.module.name'"   << " = " << task.get_module_name()
		        << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename T, class A>
void Socket
::operator=(std::vector<T,A> &vector)
{
	if (this->get_type() == socket_t::SIN)
		this->bind(vector);
	else
	{
		std::stringstream message;
		message << "Current socket have to be an input socket ("
		        << "'datatype'"           << " = " << type_to_string[this->datatype] << ", "
		        << "'name'"               << " = " << get_name()                     << ", "
		        << "'task.name'"          << " = " << task.get_name()                << ", "
		        << "'type'"               << " = " << (get_type() == socket_t::SIN ? "SIN" : "SOUT") /*<< ", "*/
//		        << "'task.module.name'"   << " = " << task.get_module_name()
		        << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

void Socket
::operator=(Socket &s)
{
	if (s.get_type() == socket_t::SOUT && this->get_type() == socket_t::SIN)
		this->bind(s);
	else if (s.get_type() == socket_t::SIN && this->get_type() == socket_t::SOUT)
		s.bind(*this);
	else
	{
		std::stringstream message;
		message << "Binding of output and input socket is required ("
		        << "'s.datatype'"         << " = " << type_to_string[s.datatype] << ", "
		        << "'s.name'"             << " = " << s.get_name()               << ", "
		        << "'s.task.name'"        << " = " << s.task.get_name()          << ", "
		        << "'s.type'"             << " = " << (s.get_type() == socket_t::SIN ? "SIN" : "SOUT") << ", "
//		        << "'s.task.module.name'" << " = " << s.task.get_module_name()   << ", "
		        << "'datatype'"           << " = " << type_to_string[this->datatype] << ", "
		        << "'name'"               << " = " << get_name()                     << ", "
		        << "'task.name'"          << " = " << task.get_name()                << ", "
		        << "'type'"               << " = " << (get_type() == socket_t::SIN ? "SIN" : "SOUT") /*<< ", "*/
//		        << "'task.module.name'"   << " = " << task.get_module_name()
		        << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

void Socket
::operator=(Task &t)
{
	if (this->get_type() == socket_t::SOUT)
		t.bind(*this);
	else
	{
		std::stringstream message;
		message << "The current socket should be and output socket ("
		        << "'datatype'"         << " = " << type_to_string[this->datatype] << ", "
		        << "'name'"             << " = " << get_name()                     << ", "
		        << "'task.name'"        << " = " << task.get_name()                << ", "
		        << "'type'"             << " = " << (get_type() == socket_t::SIN ? "SIN" : "SOUT") /*<< ", "*/
//		        << "'task.module.name'" << " = " << task.get_module_name()
		        << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename T, class A>
void Socket
::bind(const std::vector<T,A> &vector)
{
	this->bind(const_cast<std::vector<T,A>&>(vector));
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
::bind(const T *array)
{
	this->bind(const_cast<T*>(array));
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
		this->check_bound_socket();
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
	if (this->bound_socket != nullptr)
		this->unbind(*this->bound_socket);
	for (auto socket : this->bound_sockets)
		socket->unbind(*this);
}

size_t Socket
::unbind(Socket &s_out)
{
	if (this->bound_socket == nullptr)
	{
		std::stringstream message;
		message << "The current input socket can't be unbound because it is not bound to any output socket ("
		        << "'s_out.databytes'"                << " = " << s_out.databytes              << ", "
		        << "'s_out.name'"                     << " = " << s_out.get_name()             << ", "
		        << "'s_out.task.name'"                << " = " << s_out.task.get_name()        << ", "
//		        << "'s_out.task.module.name'"         << " = " << s_out.task.get_module_name() << ", "
		        << "'databytes'"                      << " = " << this->databytes              << ", "
		        << "'name'"                           << " = " << get_name()                   << ", "
		        << "'task.name'"                      << " = " << task.get_name()            /*<< ", "*/
//		        << "'task.module.name'"               << " = " << task.get_module_name()
		        << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->bound_socket != &s_out)
	{
		std::stringstream message;
		message << "This socket is connected to a different socket than 's_out' ("
		        << "'bound_socket->databytes'"        << " = " << this->bound_socket->databytes              << ", "
		        << "'bound_socket->name'"             << " = " << this->bound_socket->get_name()             << ", "
		        << "'bound_socket->task.name'"        << " = " << this->bound_socket->task.get_name()        << ", "
//		        << "'bound_socket->task.module.name'" << " = " << this->bound_socket->task.get_module_name() << ", "
		        << "'s_out.databytes'"                << " = " << s_out.databytes                            << ", "
		        << "'s_out.name'"                     << " = " << s_out.get_name()                           << ", "
		        << "'s_out.task.name'"                << " = " << s_out.task.get_name()                      << ", "
//		        << "'s_out.task.module.name'"         << " = " << s_out.task.get_module_name()               << ", "
		        << "'databytes'"                      << " = " << this->databytes                            << ", "
		        << "'name'"                           << " = " << get_name()                                 << ", "
		        << "'task.name'"                      << " = " << task.get_name()                          /*<< ", "*/
//		        << "'task.module.name'"               << " = " << task.get_module_name()
		        << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->bound_socket = nullptr;

	size_t unbind_pos = 0;
	auto it = std::find(s_out.bound_sockets.begin(), s_out.bound_sockets.end(), this);
	if (it != s_out.bound_sockets.end())
	{
		unbind_pos = (size_t)std::distance(s_out.bound_sockets.begin(), it);
		s_out.bound_sockets.erase(it);
	}
	else
	{
		std::stringstream message;
		message << "The 's_out' output socket is not bound to the current input socket ("
		        << "'s_out.databytes'"                << " = " << s_out.databytes              << ", "
		        << "'s_out.name'"                     << " = " << s_out.get_name()             << ", "
		        << "'s_out.task.name'"                << " = " << s_out.task.get_name()        << ", "
//		        << "'s_out.task.module.name'"         << " = " << s_out.task.get_module_name() << ", "
		        << "'databytes'"                      << " = " << this->databytes              << ", "
		        << "'name'"                           << " = " << get_name()                   << ", "
		        << "'task.name'"                      << " = " << task.get_name()            /*<< ", "*/
//		        << "'task.module.name'"               << " = " << task.get_module_name()
		        << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return unbind_pos;
}

void Socket
::set_name(const std::string &name)
{
	if (name != this->get_name())
	{
		this->check_bound_socket();
		this->name = name;
	}
}

void Socket
::set_datatype(const std::type_index datatype)
{
	if (datatype != this->get_datatype())
	{
		this->check_bound_socket();
		this->datatype = datatype;
	}
}

void Socket
::set_databytes(const size_t databytes)
{
	if (databytes != this->get_databytes())
	{
		this->check_bound_socket();
		this->databytes = databytes;
	}
}

void Socket
::set_dataptr(void* dataptr)
{
	if (dataptr != this->get_dataptr())
	{
		this->check_bound_socket();
		this->dataptr = dataptr;
	}
}

void Socket
::check_bound_socket()
{
	if (bound_sockets.size() != 0 || bound_socket != nullptr)
	{
		std::stringstream bound_sockets_str;
		if (bound_sockets.size() != 0)
		{
			bound_sockets_str << ", 'bound_sockets' = [";

			for (size_t bs = 0; bs < bound_sockets.size(); bs++)
			{
				bound_sockets_str << "{" << "'name'"      << " = " << bound_sockets[bs]->get_name()      << ", "
				                         << "'databytes'" << " = " << bound_sockets[bs]->get_databytes() << ", "
				                         << "'task.name'" << " = " << bound_sockets[bs]->get_task().get_name()
				                  << "}";
				if (bs < bound_sockets.size() -1)
					bound_sockets_str << ", ";
			}

			bound_sockets_str << "]";
		}
		else if (bound_socket != nullptr)
		{
			bound_sockets_str << ", 'bound_socket' = ";
			bound_sockets_str << "{" << "'name'"      << " = " << bound_socket->get_name()      << ", "
			                         << "'databytes'" << " = " << bound_socket->get_databytes() << ", "
			                         << "'task.name'" << " = " << bound_socket->get_task().get_name()
			                  << "}";
		}

		std::stringstream message;
		message << "The current socket is already bound ("
		        << "'dataptr'"   << " = " << get_dataptr()         << ", "
		        << "'databytes'" << " = " << get_databytes()       << ", "
		        << "'datatype'"  << " = " << get_datatype_string() << ", "
		        << "'name'"      << " = " << get_name()            << ", "
		        << "'task.name'" << " = " << task.get_name()
		        << bound_sockets_str.str()
		        << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

}
}
