/*!
 * \file
 * \brief Class module::Incrementer.
 */
#ifndef INCREMENTER_HPP_
#define INCREMENTER_HPP_

#include <cstdint>
#include <vector>

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace inc
	{
		enum class tsk : size_t { increment, SIZE };

		namespace sck
		{
			enum class increment : size_t { in, out, status };
		}
	}

template <typename T = int>
class Incrementer : public Module
{
public:
	inline Task&   operator[](const inc::tsk            t);
	inline Socket& operator[](const inc::sck::increment s);

protected:
	const size_t n_elmts;
	size_t ns;

public:
	Incrementer(const size_t n_elmts, const size_t ns = 0);
	virtual ~Incrementer() = default;
	virtual Incrementer* clone() const;

	size_t get_ns() const;
	void set_ns(const size_t ns);
	size_t get_n_elmts() const;

	template <class A = std::allocator<T>>
	void increment(const std::vector<T,A>& in,
	                     std::vector<T,A>& out,
	               const int frame_id = -1,
	               const bool managed_memory = true);

	void increment(const T *in, T *out, const int frame_id = -1, const bool managed_memory = true);

protected:
	virtual void _increment(const T *in, T *out, const size_t frame_id);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Incrementer/Incrementer.hxx"
#endif

#endif /* INCREMENTER_HPP_ */
