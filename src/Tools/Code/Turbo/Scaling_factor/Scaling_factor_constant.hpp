#ifndef SCALING_FACTOR_CONSTANT_HPP
#define SCALING_FACTOR_CONSTANT_HPP

#include "Scaling_factor.hpp"

template <typename R>
class Scaling_factor_constant : public Scaling_factor<R>
{
private:
	R alpha;
	
public:
	Scaling_factor_constant(const int K, const int n_ite, const float alpha);
	void operator()(mipp::vector<R> &ext, const int ite);
};

#endif