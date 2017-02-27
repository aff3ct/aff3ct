/*!
 * \file
 * \brief Performs the coset approach (flips the bits or the signs depending on the initial codeword).
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef COSET_HPP_
#define COSET_HPP_

#include <vector>
#include <string>

#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
/*!
 * \class Coset_i
 *
 * \brief Performs the coset approach (flips the bits or the signs depending on the initial codeword).
 *
 * \tparam B: type of the bits in the ref data.
 * \tparam D: type of the input/output data.
 *
 * Please use Coset for inheritance (instead of Coset_i).
 */
template <typename B, typename D>
class Coset_i : public Module
{
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
	Coset_i(const int size, const int n_frames = 1, const std::string name = "Coset_i")
	: Module(n_frames, name), size(size)
	{
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Coset_i()
	{
	}

	/*!
	 * \brief Applies the coset approach on data using the reference.
	 *
	 * Flips the bits or the signs depending on the reference (flips if bit ref = 1, do nothing else).
	 *
	 * \param ref:      the ref codeword.
	 * \param in_data:  the input data to apply the coset on.
	 * \param out_data: the output data after the coset application.
	 */
	virtual void apply(const mipp::vector<B>& ref, const mipp::vector<D> &in_data, mipp::vector<D> &out_data) = 0;
};
}
}

#include "SC_Coset.hpp"

#endif /* COSET_HPP_ */
