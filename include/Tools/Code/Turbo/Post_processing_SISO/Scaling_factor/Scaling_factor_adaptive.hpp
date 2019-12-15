/*!
 * \file
 * \brief Class tools::Scaling_factor_adaptive.
 */
#ifndef SCALING_FACTOR_ADAPTIVE_HPP
#define SCALING_FACTOR_ADAPTIVE_HPP

#include <mipp.h>

#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
class Scaling_factor_adaptive : public Scaling_factor<B,R>
{
public:
	explicit Scaling_factor_adaptive(const int n_ite);

	bool siso_n(const int ite,
	            const mipp::vector<R>& sys,
	                  mipp::vector<R>& ext,
	                  mipp::vector<B>& s);

	bool siso_i(const int ite,
	            const mipp::vector<R>& sys,
	                  mipp::vector<R>& ext);

	virtual Scaling_factor_adaptive<B,R>* clone() const;
};
}
}

#endif
