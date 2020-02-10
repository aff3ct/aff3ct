/*!
 * \file
 * \brief Class module::Router.
 */
#ifndef ROUTER_HPP_
#define ROUTER_HPP_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>
#include <typeindex>

#include "Tools/Interface/Interface_reset.hpp"
#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace rtr
	{
		enum class tsk : size_t { route, SIZE };

		namespace sck
		{
			enum class route : size_t { in, in_out1, in_out2, status };
		}
	}

class Router : public Module, public tools::Interface_reset
{
public:
	inline Task&   operator[](const rtr::tsk        t);
	inline Socket& operator[](const rtr::sck::route s);

protected:
	const size_t n_elmts_in;
	const size_t n_bytes_in;
	const std::type_index datatype_in;
	const size_t n_elmts_out;
	const size_t n_bytes_out;
	const std::type_index datatype_out;
	const size_t n_outputs;

public:
	inline Router(const size_t n_elmts_in,
	              const std::type_index datatype_in,
	              const size_t n_elmts_out,
	              const std::type_index datatype_out,
	              const size_t n_outputs,
	              const int n_frames = 1);
	virtual ~Router() = default;

	inline size_t get_n_elmts_in () const;
	inline size_t get_n_bytes_in() const;
	inline std::type_index get_datatype_in() const;
	inline size_t get_n_elmts_out() const;
	inline size_t get_n_bytes_out() const;
	inline std::type_index get_datatype_out() const;
	inline size_t get_n_outputs() const;

	template <class A = std::allocator<int8_t>>
	size_t route(const std::vector<int8_t,A>& in, const int frame_id = -1);

	virtual size_t route(const int8_t *in, const int frame_id = -1);

	virtual void reset();

protected:
	virtual size_t _route(const int8_t *in, const int frame_id);

	virtual size_t select_route_inter(const size_t a, const size_t b);

private:
	static size_t compute_bytes(const size_t n_elmts, const std::type_index type);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Router/Router.hxx"
#endif

#endif /* ROUTER_HPP_ */
