/*!
 * \file
 * \brief Class tools::Scaling_factor.
 */
#ifndef SCALING_FACTOR_HPP
#define SCALING_FACTOR_HPP

#include "Tools/Code/Turbo/Post_processing_SISO/Post_processing_SISO.hpp"

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
	 * \param n_ite: number of iterations in the turbo decoding process.
	 */
	explicit Scaling_factor(const int n_ite);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Scaling_factor() = default;

	virtual Scaling_factor<B,R>* clone() const;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor.hxx"
#endif

#endif /* SCALING_FACTOR_HPP */
