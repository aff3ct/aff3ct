#ifndef SCALING_FACTOR_HPP
#define SCALING_FACTOR_HPP

#include "Tools/MIPP/mipp.h"

template <typename R>
class Scaling_factor
{
protected:
	const int K;
	const int n_ite;

public:
	Scaling_factor(const int K, const int n_ite) : K(K), n_ite(n_ite) {}
	virtual ~Scaling_factor() {}
	virtual void operator()(mipp::vector<R> &ext, const int ite) = 0;
};

#endif /* SCALING_FACTOR_HPP */
