#ifndef SCALING_FACTOR_VEC_HPP
#define SCALING_FACTOR_VEC_HPP

#include "Scaling_factor.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
class Scaling_factor_vec : public Scaling_factor<B,R>
{
public:
	explicit Scaling_factor_vec(const int n_ite);

	bool siso_n(const int ite,
	            const mipp::vector<R>& sys,
	                  mipp::vector<R>& ext,
	                  mipp::vector<B>& s);

	bool siso_i(const int ite,
	            const mipp::vector<R>& sys,
	                  mipp::vector<R>& ext);
};
}
}

#endif
