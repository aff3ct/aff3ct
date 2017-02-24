#ifndef SCALING_FACTOR_SEQ_HPP
#define SCALING_FACTOR_SEQ_HPP

#include "Scaling_factor.hpp"

namespace aff3ct
{
template <typename R>
class Scaling_factor_seq : public Scaling_factor<R>
{
public:
	Scaling_factor_seq(const int K, const int n_ite);
	void operator()(mipp::vector<R> &ext, const int ite);
};
}

#endif
