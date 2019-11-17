/*!
 * \file
 * \brief Class tools::Scaling_factor_constant.
 */
#ifndef SCALING_FACTOR_CONSTANT_HPP
#define SCALING_FACTOR_CONSTANT_HPP

#include <mipp.h>

#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
class Scaling_factor_constant : public Scaling_factor<B,R>
{
private:
	R alpha;

public:
	Scaling_factor_constant(const int n_ite, const float alpha = 0.75f);

	bool siso_n(const int ite,
	            const mipp::vector<R>& sys,
	                  mipp::vector<R>& ext,
	                  mipp::vector<B>& s);

	bool siso_i(const int ite,
	            const mipp::vector<R>& sys,
	                  mipp::vector<R>& ext);

	virtual Scaling_factor_constant<B,R>* clone() const;
};
}
}

#endif
