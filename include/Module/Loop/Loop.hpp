/*!
 * \file
 * \brief Class module::Loop.
 */
#ifndef LOOP_HPP_
#define LOOP_HPP_

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
	namespace lop
	{
		enum class tsk : size_t { stop, SIZE };

		namespace sck
		{
			enum class stop : size_t { in1, in2, in_out1, in_out2, SIZE };
		}
	}

class Loop : public Module, public tools::Interface_reset
{
public:
	inline Task&   operator[](const lop::tsk       t);
	inline Socket& operator[](const lop::sck::stop s);

protected:
	const size_t n_elmts_in;
	const size_t n_bytes_in;
	const std::type_index datatype_in;
	const size_t n_elmts_out;
	const size_t n_bytes_out;
	const std::type_index datatype_out;
	size_t n_calls;

public:
	inline Loop(const size_t n_elmts_in,
	            const std::type_index datatype_in,
	            const size_t n_elmts_out,
	            const std::type_index datatype_out,
	            const int n_frames = 1);
	virtual ~Loop() = default;

	inline size_t get_n_elmts_in () const;
	inline size_t get_n_bytes_in () const;
	inline std::type_index get_datatype_in() const;
	inline size_t get_n_elmts_out() const;
	inline size_t get_n_bytes_out() const;
	inline std::type_index get_datatype_out() const;
	inline size_t get_n_calls    () const;

	template <class A = std::allocator<int8_t>>
	bool stop(const std::vector<int8_t,A>& in, const int frame_id = -1);

	virtual bool stop(const int8_t *in, const int frame_id = -1);

	virtual void reset();

protected:
	virtual bool _stop(const int8_t *in, const int frame_id);

private:
	static size_t compute_bytes(const size_t n_elmts, const std::type_index type);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Loop/Loop.hxx"
#endif

#endif /* LOOP_HPP_ */
