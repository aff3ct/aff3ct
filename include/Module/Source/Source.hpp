/*!
 * \file
 * \brief Class module::Source.
 */
#ifndef SOURCE_HPP_
#define SOURCE_HPP_

#include <cstdint>
#include <memory>
#include <vector>

#include "Tools/Interface/Interface_set_seed.hpp"
#include "Tools/Interface/Interface_is_done.hpp"
#include "Tools/Interface/Interface_reset.hpp"
#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace src
	{
		enum class tsk : size_t { generate, SIZE };

		namespace sck
		{
			enum class generate : size_t { U_K, status };
		}
	}

/*!
 * \class Source
 *
 * \brief Generates a message.
 *
 * \tparam B: type of the bits in the Source.
 *
 * Please use Source for inheritance (instead of Source).
 */
template <typename B = int>
class Source : public Module, public tools::Interface_set_seed,
                              public tools::Interface_is_done,
                              public tools::Interface_reset
{
public:
	inline Task&   operator[](const src::tsk           t);
	inline Socket& operator[](const src::sck::generate s);

protected:
	const int K; /*!< Number of information bits in one frame */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K: number of information bits in the frame.
	 */
	Source(const int K);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Source() = default;

	virtual Source<B>* clone() const;

	virtual int get_K() const;

	/*!
	 * \brief Task method method fulfills a vector with bits.
	 *
	 * \param U_K: a vector of bits to fill.
	 */
	template <class A = std::allocator<B>>
	void generate(std::vector<B,A>& U_K, const int frame_id = -1, const bool managed_memory = true);

	void generate(B *U_K, const int frame_id = -1, const bool managed_memory = true);

	virtual void set_seed(const int seed);

	virtual bool is_done() const;

	virtual void reset();

protected:
	virtual void _generate(B *U_K, const size_t frame_id);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Source/Source.hxx"
#endif

#endif /* SOURCE_HPP_ */
