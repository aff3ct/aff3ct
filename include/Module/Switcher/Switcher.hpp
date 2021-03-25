/*!
 * \file
 * \brief Class module::Switcher.
 */
#ifndef SWITCHER_HPP_
#define SWITCHER_HPP_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>
#include <typeindex>

#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace swi
	{
		enum class tsk : size_t { commute, select, SIZE };
	}

class Switcher : public Module
{
public:
	inline       Task& operator[](const swi::tsk t);
	inline const Task& operator[](const swi::tsk t) const;

protected:
	const size_t n_data_sockets;
	const size_t n_elmts;
	const size_t n_bytes;
	const std::type_index datatype;
	      size_t path;

public:
	inline Switcher(const size_t n_data_sockets,
	                const size_t n_elmts,
	                const std::type_index datatype);
	virtual ~Switcher() = default;
	virtual Switcher* clone() const;


	inline size_t          get_n_data_sockets() const;
	inline size_t          get_n_elmts       () const;
	inline size_t          get_n_bytes       () const;
	inline std::type_index get_datatype      () const;
	inline size_t          get_path          () const;

protected:
	inline void set_path(const size_t path);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Switcher/Switcher.hxx"
#endif

#endif /* SWITCHER_HPP_ */
