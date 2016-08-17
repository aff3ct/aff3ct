#ifndef QUANTIZER_STANDARD_HPP_
#define QUANTIZER_STANDARD_HPP_

#include <vector>
#include "../Tools/MIPP/mipp.h"

#include "Quantizer.hpp"

template <typename R, typename Q>
class Quantizer_standard : public Quantizer<R,Q>
{
private:
	const int   val_max;
	const int   val_min;
	const short fixed_point_pos; // 0 = no decimal part
	const int   factor;

public:
	Quantizer_standard(const short& fixed_point_pos, 
	                   const std::string name = "Quantizer_standard");
	Quantizer_standard(const short& fixed_point_pos, 
	                   const short& saturation_pos, 
	                   const std::string name = "Quantizer_standard");
	virtual ~Quantizer_standard();

	void process(const mipp::vector<R>& Y_N1, mipp::vector<Q>& Y_N2);
};

#endif /* QUANTIZER_STANDARD_HPP_ */

