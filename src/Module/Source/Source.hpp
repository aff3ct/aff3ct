/*!
 * \file
 * \brief Generates a message.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef SOURCE_HPP_
#define SOURCE_HPP_

#include <cstdint>
#include <memory>
#include <vector>

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace src
	{
		enum class tsk : uint8_t { generate, SIZE };

		namespace sck
		{
			enum class generate : uint8_t { U_K, SIZE };
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
class Source : public Module
{
public:
	inline Task&   operator[](const src::tsk           t) { return Module::operator[]((int)t);                          }
	inline Socket& operator[](const src::sck::generate s) { return Module::operator[]((int)src::tsk::generate)[(int)s]; }

protected:
	const int K; /*!< Number of information bits in one frame */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K:        number of information bits in the frame.
	 * \param n_frames: number of frames to process in the Source.
	 * \param name:     Source's name.
	 */
	Source(const int K, const int n_frames = 1);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Source() = default;

	virtual int get_K() const;

	/*!
	 * \brief Fulfills a vector with bits.
	 *
	 * \param U_K: a vector of bits to fill.
	 */
	template <class A = std::allocator<B>>
	void generate(std::vector<B,A>& U_K, const int frame_id = -1);

	virtual void generate(B *U_K, const int frame_id = -1);

protected:
	virtual void _generate(B *U_K, const int frame_id);
};
}
}
#include "Module/Source/Source.hxx"

#endif /* SOURCE_HPP_ */
