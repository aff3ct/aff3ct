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
		enum class tsk : size_t { commute, SIZE };
	}

class Switcher : public Module
{
public:
	inline Task& operator[](const swi::tsk t);

protected:
	const size_t n_data_sockets;
	const size_t n_elmts;
	const size_t n_bytes;
	const std::type_index datatype;

public:
	inline Switcher(const size_t n_data_sockets,
	                const size_t n_elmts,
	                const std::type_index datatype);
	virtual ~Switcher() = default;

	inline size_t          get_n_data_sockets() const;
	inline size_t          get_n_elmts       () const;
	inline size_t          get_n_bytes       () const;
	inline std::type_index get_datatype      () const;
	inline size_t          get_n_calls       () const;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Switcher/Switcher.hxx"
#endif

#endif /* SWITCHER_HPP_ */
