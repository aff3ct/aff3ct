#ifndef QUANTIZER_FAST_HPP_
#define QUANTIZER_FAST_HPP_

#include <vector>
#include "../Tools/MIPP/mipp.h"

#include "Quantizer.hpp"

template <typename R, typename Q>
class Quantizer_fast : public Quantizer<R,Q>
{
private:
	const int   val_max;
	const int   val_min;
	const short fixed_point_pos; // 0 = no decimal part
	const int   factor;

public:
	Quantizer_fast(const short& fixed_point_pos);
	Quantizer_fast(const short& fixed_point_pos, const short& saturation_pos);
	virtual ~Quantizer_fast();

	void process(mipp::vector<R>& Y_N1, mipp::vector<Q>& Y_N2);

private:
	inline R saturate(R val) const;
};

#endif /* QUANTIZER_FAST_HPP_ */

