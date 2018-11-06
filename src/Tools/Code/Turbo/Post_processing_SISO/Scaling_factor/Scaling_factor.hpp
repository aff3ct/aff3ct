/*!
 * \file
 * \brief Re-scales the extrinsic information (used in the turbo decoding process).
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef SCALING_FACTOR_HPP
#define SCALING_FACTOR_HPP

#include "../Post_processing_SISO.hpp"

namespace aff3ct
{
namespace tools
{
/*!
 * \class Scaling_factor
 * \brief Re-scales the extrinsic information (used in the turbo decoding process).
 */
template <typename B = int, typename R = float>
class Scaling_factor : public Post_processing_SISO<B,R>
{
protected:
	const int n_ite; /*!< Number of iterations in the turbo decoding process */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K:     number of information bits in the frame.
	 * \param n_ite: number of iterations in the turbo decoding process.
	 */
	explicit Scaling_factor(const int n_ite)
	: n_ite(n_ite)
	{
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Scaling_factor() = default;
};
}
}

#endif /* SCALING_FACTOR_HPP */
