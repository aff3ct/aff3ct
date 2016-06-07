#ifndef SCALING_FACTOR_ARRAY_HPP
#define SCALING_FACTOR_ARRAY_HPP

#include "Scaling_factor.hpp"

template <typename R>
class Scaling_factor_array : public Scaling_factor<R>
{
private:
	const  mipp::vector<float> alpha_array = {0.15, 0.15, 0.25, 0.25, 
	                                          0.30, 0.30, 0.40, 0.40, 
	                                          0.70, 0.70, 0.80, 0.80, 
	                                          0.90, 0.90, 0.95, 0.95}; 
public:
	Scaling_factor_array(const int K, const int n_ite);
	void operator()(mipp::vector<R> &ext, const int ite);
};

#endif