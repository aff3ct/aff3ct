#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include <string>
#include <memory>
#include <vector>
#include <cstddef>
#include <typeindex>

#include "Module/Task.hpp"

namespace aff3ct
{
namespace module
{
class Socket
{
	friend Task;
protected:
	Task &task;

	const std::string     name;
	const std::type_index datatype;
	const size_t          databytes;
	      bool            fast;
	      void*           dataptr;

public:
	inline Socket(Task &task, const std::string &name, const std::type_index datatype, const size_t databytes,
	              const bool fast = false, void *dataptr = nullptr);

	inline std::string     get_name           () const;
	inline std::type_index get_datatype       () const;
	inline std::string     get_datatype_string() const;
	inline uint8_t         get_datatype_size  () const;
	inline size_t          get_databytes      () const;
	inline size_t          get_n_elmts        () const;
	inline void*           get_dataptr        () const;
	inline bool            is_fast            () const;

	inline void set_fast(const bool fast);

	inline int bind(Socket &s);

	inline int operator()(Socket &s);

	template <typename T, class A = std::allocator<T>>
	inline int bind(std::vector<T,A> &vector);

	template <typename T, class A = std::allocator<T>>
	inline int operator()(std::vector<T,A> &vector);

	template <typename T>
	inline int bind(T *array);

	template <typename T>
	inline int operator()(T *array);

	inline int bind(void* dataptr);

	inline int operator()(void* dataptr);
};
}
}

#include "Module/Socket.hxx"

#endif /* SOCKET_HPP_ */
