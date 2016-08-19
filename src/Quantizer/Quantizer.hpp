#ifndef QUANTIZER_HPP_
#define QUANTIZER_HPP_

#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename R, typename Q>
class Quantizer_interface // please use Quantizer<R,Q> for inheritance (instead of Quantizer_interface<R,Q>)
{
public:
	Quantizer_interface(const std::string name = "Quantizer_interface") {};
	virtual ~Quantizer_interface() {};

	virtual void process(const mipp::vector<R>& Y_N1, mipp::vector<Q>& Y_N2) = 0;
};

#include "SC_Quantizer.hpp"

#endif /* QUANTIZER_HPP_ */
