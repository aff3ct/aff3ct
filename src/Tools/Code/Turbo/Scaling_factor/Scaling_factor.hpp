/*!
 * \file
 * \brief Re-scales the extrinsic information (used in the turbo decoding process).
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef SCALING_FACTOR_HPP
#define SCALING_FACTOR_HPP

#include "Tools/Perf/MIPP/mipp.h"

namespace aff3ct
{
namespace tools
{
/*!
 * \class Scaling_factor
 * \brief Re-scales the extrinsic information (used in the turbo decoding process).
 */
template <typename R = float>
class Scaling_factor
{
protected:
	const int K;     /*!< Number of information bits in one frame */
	const int n_ite; /*!< Number of iterations in the turbo decoding process */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K:     number of information bits in the frame.
	 * \param n_ite: number of iterations in the turbo decoding process.
	 */
	Scaling_factor(const int K, const int n_ite)
	: K(K), n_ite(n_ite)
	{
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Scaling_factor()
	{
	}

	/*!
	 * \brief Performs the scaling factor.
	 *
	 * \param ext: a vector of extrinsics to re-scale.
	 * \param ite: the current iteration number.
	 */
	virtual void operator()(mipp::vector<R> &ext, const int ite) = 0;
};
}
}

#endif /* SCALING_FACTOR_HPP */
