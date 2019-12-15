/*!
 * \file
 * \brief Class tools::Scaling_factor_vec.
 */
#ifndef SCALING_FACTOR_VEC_HPP
#define SCALING_FACTOR_VEC_HPP

#include <mipp.h>

#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor.hpp"

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

	virtual Scaling_factor_vec<B,R>* clone() const;
};
}
}

#endif
