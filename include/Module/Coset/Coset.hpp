/*!
 * \file
 * \brief Class module::Coset.
 */
#ifndef COSET_HPP_
#define COSET_HPP_

#include <cstdint>
#include <cstddef>
#include <vector>
#include <memory>

#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace cst
	{
		enum class tsk : size_t { apply, SIZE };

		namespace sck
		{
			enum class apply : size_t { ref, in, out, status };
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
	inline Task&   operator[](const cst::tsk        t);
	inline Socket& operator[](const cst::sck::apply s);

protected:
	const int size; /*!< Size of the input data */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param size: size of the input data.
	 */
	Coset(const int size);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Coset() = default;

	virtual Coset<B,D>* clone() const;

	int get_size() const;

	/*!
	 * \brief Applies the coset approach on data using the reference.
	 *
	 * Flips the bits or the signs depending on the reference (flips if bit ref = 1, do nothing else).
	 *
	 * \param ref: the ref codeword.
	 * \param in:  the input data to apply the coset on.
	 * \param out: the output data after the coset application.
	 */
	template <class AB = std::allocator<B>, class AD = std::allocator<D>>
	void apply(const std::vector<B,AB>& ref, const std::vector<D,AD> &in, std::vector<D,AD> &out,
	           const int frame_id = -1, const bool managed_memory = true);

	void apply(const B *ref, const D *in, D *out, const int frame_id = -1, const bool managed_memory = true);

protected:
	virtual void _apply(const B *ref, const D *in, D *out, const size_t frame_id);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Coset/Coset.hxx"
#endif

#endif /* COSET_HPP_ */
