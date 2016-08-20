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
	Quantizer_fast(const int N, const short& fixed_point_pos, const int n_frames = 1, 
	               const std::string name = "Quantizer_fast");
	Quantizer_fast(const int N, const short& fixed_point_pos, const short& saturation_pos, const int n_frames = 1,
	               const std::string name = "Quantizer_fast");
	virtual ~Quantizer_fast();

	void process(const mipp::vector<R>& Y_N1, mipp::vector<Q>& Y_N2);
};

#endif /* QUANTIZER_FAST_HPP_ */

