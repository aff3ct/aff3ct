#ifndef QUANTIZER_CUSTOM_HPP_
#define QUANTIZER_CUSTOM_HPP_

#include "../Quantizer.hpp"

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
	Quantizer_custom(const int N, const int n_frames = 1);
	Quantizer_custom(const int N, const short& saturation_pos, const int n_frames = 1);
	Quantizer_custom(const int N, const float min_max, const int n_frames = 1);
	Quantizer_custom(const int N, const float min_max, const short& saturation_pos,  const int n_frames = 1);
	virtual ~Quantizer_custom() = default;

protected:
	void _process(const R *Y_N1, Q *Y_N2, const int frame_id);
};
}
}

#endif /* QUANTIZER_CUSTOM_HPP_ */

