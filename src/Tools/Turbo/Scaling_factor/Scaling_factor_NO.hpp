#ifndef SCALING_FACTOR_NO_HPP
#define SCALING_FACTOR_NO_HPP

#include "Scaling_factor.hpp"

template <typename R>
class Scaling_factor_NO : public Scaling_factor<R>
{
public:
	Scaling_factor_NO(const int K, const int n_ite);
	void operator()(mipp::vector<R> &ext, const int ite);
};

#endif