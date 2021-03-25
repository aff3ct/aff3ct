/*!
 * \file
 * \brief Class module::Iterator.
 */
#ifndef ITERATOR_HPP_
#define ITERATOR_HPP_

#include <cstdint>

#include "Tools/Interface/Interface_reset.hpp"
#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace ite
	{
		enum class tsk : size_t { iterate, reset, SIZE };

		namespace sck
		{
			enum class iterate : size_t { out, status };
		}
	}

class Iterator : public Module, public tools::Interface_reset
{
public:
	inline Task&   operator[](const ite::tsk          t);
	inline Socket& operator[](const ite::sck::iterate s);

protected:
	size_t limit;
	size_t counter;

public:
	Iterator(const size_t limit);
	virtual ~Iterator() = default;
	virtual Iterator* clone() const;

	size_t get_limit() const;
	void set_limit(const size_t limit);

	void reset();

	template <class A = std::allocator<int8_t>>
	void iterate(std::vector<int8_t,A>& out, const int frame_id = -1, const bool managed_memory = true);

	void iterate(int8_t *out, const int frame_id = -1, const bool managed_memory = true);

protected:
	virtual void _iterate(int8_t *out, const size_t frame_id);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Iterator/Iterator.hxx"
#endif

#endif /* ITERATOR_HPP_ */
