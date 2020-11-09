/*!
 * \file
 * \brief Class module::Quantizer_NO.
 */
#ifndef QUANTIZER_NO_HPP_
#define QUANTIZER_NO_HPP_

#include "Module/Quantizer/Quantizer.hpp"

namespace aff3ct
{
namespace module
{
template <typename R = float, typename Q = int>
class Quantizer_NO : public Quantizer<R,Q>
{
public:
	Quantizer_NO(const int N);
	virtual ~Quantizer_NO() = default;

	virtual Quantizer_NO<R,Q>* clone() const;

protected:
	void _process(const R *Y_N1, Q *Y_N2, const size_t frame_id);
};
}
}

#endif /* QUANTIZER_NO_HPP_ */

