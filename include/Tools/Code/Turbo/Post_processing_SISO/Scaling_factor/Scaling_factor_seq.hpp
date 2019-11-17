/*!
 * \file
 * \brief Class tools::Scaling_factor_seq.
 */
#ifndef SCALING_FACTOR_SEQ_HPP
#define SCALING_FACTOR_SEQ_HPP

#include <mipp.h>

#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
class Scaling_factor_seq : public Scaling_factor<B,R>
{
public:
	explicit Scaling_factor_seq(const int n_ite);

	bool siso_n(const int ite,
	            const mipp::vector<R>& sys,
	                  mipp::vector<R>& ext,
	                  mipp::vector<B>& s);

	bool siso_i(const int ite,
	            const mipp::vector<R>& sys,
	                  mipp::vector<R>& ext);

	virtual Scaling_factor_seq<B,R>* clone() const;
};
}
}

#endif
