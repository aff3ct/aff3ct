#ifndef QUANTIZER_FAST_HPP_
#define QUANTIZER_FAST_HPP_

#include "../Quantizer.hpp"

namespace aff3ct
{
namespace module
{
template <typename R = float, typename Q = short>
class Quantizer_fast : public Quantizer<R,Q>
{
private:
	const int   val_max;
	const int   val_min;
	const short fixed_point_pos; // 0 = no decimal part
	const int   factor;

public:
	Quantizer_fast(const int N, const short& fixed_point_pos, const int n_frames = 1);
	Quantizer_fast(const int N, const short& fixed_point_pos, const short& saturation_pos, const int n_frames = 1);
	virtual ~Quantizer_fast();

	void process(const R *Y_N1, Q *Y_N2); using Quantizer<R,Q>::process;
};
}
}

#endif /* QUANTIZER_FAST_HPP_ */

