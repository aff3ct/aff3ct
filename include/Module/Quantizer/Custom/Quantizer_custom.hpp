/*!
 * \file
 * \brief Class module::Quantizer_custom.
 */
#ifndef QUANTIZER_CUSTOM_HPP_
#define QUANTIZER_CUSTOM_HPP_

#include "Module/Quantizer/Quantizer.hpp"

namespace aff3ct
{
namespace module
{
template <typename R = float, typename Q = float>
class Quantizer_custom : public Quantizer<R,Q>
{
private:
	const int val_max;
	const int val_min;
	R delta_inv;

public:
	Quantizer_custom(const int N);
	Quantizer_custom(const int N, const short& saturation_pos);
	Quantizer_custom(const int N, const float min_max);
	Quantizer_custom(const int N, const float min_max, const short& saturation_pos);
	virtual ~Quantizer_custom() = default;

	virtual Quantizer_custom<R,Q>* clone() const;

protected:
	void _process(const R *Y_N1, Q *Y_N2, const size_t frame_id);
};
}
}

#endif /* QUANTIZER_CUSTOM_HPP_ */

