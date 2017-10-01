#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include <string>
#include <sstream>
#include <typeindex>

#include "Tools/Exception/exception.hpp"

namespace aff3ct
{
namespace module
{
class Task;

enum Socket_type
{
	IN     = 0,
	IN_OUT = 1,
	OUT    = 2
};

class Socket
{
	friend Task;

protected:
	Task &task;

	const std::string     name;
	const std::type_index datatype;
	const size_t          databytes;
	      void*           dataptr;

	Socket(Task &task, const std::string name, const std::type_index datatype, const size_t databytes,
	       void *dataptr = nullptr);

public:
	std::string     get_name           () const;
	std::type_index get_datatype       () const;
	std::string     get_datatype_string() const;
	uint8_t         get_datatype_size  () const;
	size_t          get_databytes      () const;
	size_t          get_n_elmts        () const;
	void*           get_dataptr        () const;

	int bind(Socket &s);

	template <typename T, class A = std::allocator<T>>
	int bind(std::vector<T,A> &vector);

	template <typename T>
	inline int bind(T *array);

	int bind(void* dataptr);
};
}
}

#endif /* SOCKET_HPP_ */
