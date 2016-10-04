#ifndef SCALING_FACTOR_ARRAY_HPP
#define SCALING_FACTOR_ARRAY_HPP

#include "Scaling_factor.hpp"

template <typename R>
class Scaling_factor_array : public Scaling_factor<R>
{
private:
	const  mipp::vector<float> alpha_array = {0.15f, 0.15f, 0.25f, 0.25f, 
	                                          0.30f, 0.30f, 0.40f, 0.40f, 
	                                          0.70f, 0.70f, 0.80f, 0.80f, 
	                                          0.90f, 0.90f, 0.95f, 0.95f}; 
public:
	Scaling_factor_array(const int K, const int n_ite);
	void operator()(mipp::vector<R> &ext, const int ite);
};

#endif