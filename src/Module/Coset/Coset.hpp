/*!
 * \file
 * \brief Performs the coset approach (flips the bits or the signs depending on the initial codeword).
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef COSET_HPP_
#define COSET_HPP_

#include <cstdint>
#include <vector>
#include <memory>

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace cst
	{
		enum class tsk : uint8_t { apply, SIZE };

		namespace sck
		{
			enum class apply : uint8_t { ref, in, out, SIZE };
		}
	}

/*!
 * \class Coset
 *
 * \brief Performs the coset approach (flips the bits or the signs depending on the initial codeword).
 *
 * \tparam B: type of the bits in the ref data.
 * \tparam D: type of the input/output data.
 *
 * Please use Coset for inheritance (instead of Coset).
 */
template <typename B, typename D>
class Coset : public Module
{
public:
	inline Task&   operator[](const cst::tsk        t) { return Module::operator[]((int)t);                       }
	inline Socket& operator[](const cst::sck::apply s) { return Module::operator[]((int)cst::tsk::apply)[(int)s]; }

protected:
	const int size; /*!< Size of the input data */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param size:     size of the input data.
	 * \param n_frames: number of frames to process in the Source.
	 * \param name:     Source's name.
	 */
	Coset(const int size, const int n_frames = 1);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Coset() = default;

	int get_size() const;

	/*!
	 * \brief Applies the coset approach on data using the reference.
	 *
	 * Flips the bits or the signs depending on the reference (flips if bit ref = 1, do nothing else).
	 *
	 * \param ref:      the ref codeword.
	 * \param in_data:  the input data to apply the coset on.
	 * \param out_data: the output data after the coset application.
	 */
	template <class AB = std::allocator<B>, class AD = std::allocator<D>>
	void apply(const std::vector<B,AB>& ref, const std::vector<D,AD> &in, std::vector<D,AD> &out,
	           const int frame_id = -1);

	virtual void apply(const B *ref, const D *in, D *out, const int frame_id = -1);

protected:
	virtual void _apply(const B *ref, const D *in, D *out, const int frame_id);
};
}
}
#include "Module/Coset/Coset.hxx"

#endif /* COSET_HPP_ */
