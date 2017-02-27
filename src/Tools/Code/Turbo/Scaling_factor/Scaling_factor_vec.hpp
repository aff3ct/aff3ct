#ifndef SCALING_FACTOR_VEC_HPP
#define SCALING_FACTOR_VEC_HPP

#include "Scaling_factor.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float>
class Scaling_factor_vec : public Scaling_factor<R>
{
public:
	Scaling_factor_vec(const int K, const int n_ite);
	void operator() (mipp::vector<R> &ext, const int ite);
};
}
}

#endif
