#ifndef SCALING_FACTOR_ARRAY_FAST_HPP
#define SCALING_FACTOR_ARRAY_FAST_HPP

#include "Scaling_factor.hpp"

template <typename R>
class Scaling_factor_array_fast : public Scaling_factor<R>
{
private:
	const  mipp::vector<float> alpha_array = {0.75, 0.15, 0.75, 0.25,
	                                          0.75, 0.30, 0.75, 0.40,
		                                      0.75, 0.70, 0.75, 0.80,
		                                      0.75, 0.90, 0.75, 0.95,
		                                      0.75, 1.00, 0.75, 1.00,
		                                      0.75, 1.00, 0.75, 1.00};
public:
	Scaling_factor_array_fast(const int K, const int n_ite);
	void operator()(mipp::vector<R> &ext, const int ite);
};

#endif