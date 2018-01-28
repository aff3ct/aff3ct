#ifndef QUANTIZER_STANDARD_HPP_
#define QUANTIZER_STANDARD_HPP_

#include "../Quantizer.hpp"

namespace aff3ct
{
namespace module
{
template <typename R = float, typename Q = float>
class Quantizer_standard : public Quantizer<R,Q>
{
private:
	const int   val_max;
	const int   val_min;
	const short fixed_point_pos; // 0 = no decimal part
	const int   factor;

public:
	Quantizer_standard(const int N, const short& fixed_point_pos, const int n_frames = 1);
	Quantizer_standard(const int N, const short& fixed_point_pos, const short& saturation_pos, const int n_frames = 1);
	virtual ~Quantizer_standard();

protected:
	void _process(const R *Y_N1, Q *Y_N2, const int frame_id);
};
}
}

#endif /* QUANTIZER_STANDARD_HPP_ */

