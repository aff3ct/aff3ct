#ifndef QUANTIZER_HPP_
#define QUANTIZER_HPP_

#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename R, typename Q>
class Quantizer
{
public:
	Quantizer(const std::string name = "Quantizer") {};
	virtual ~Quantizer() {};

	virtual void process(const mipp::vector<R>& Y_N1, mipp::vector<Q>& Y_N2) = 0;
};

#endif /* QUANTIZER_HPP_ */
