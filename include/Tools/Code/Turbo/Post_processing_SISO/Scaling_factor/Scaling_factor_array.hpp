/*!
 * \file
 * \brief Class tools::Scaling_factor_array.
 */
#ifndef SCALING_FACTOR_ARRAY_HPP
#define SCALING_FACTOR_ARRAY_HPP

#include <mipp.h>

#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
class Scaling_factor_array : public Scaling_factor<B,R>
{
private:
	const mipp::vector<float> alpha_array;

public:
	Scaling_factor_array(const int n_ite,                      // siso_n siso_i
	                     const mipp::vector<float> alpha_array = {0.15f, 0.15f,   // ite 1
	                                                              0.25f, 0.25f,   // ite 2
	                                                              0.30f, 0.30f,   // ite 3
	                                                              0.40f, 0.40f,   // ite 4
	                                                              0.70f, 0.70f,   // ite 5
	                                                              0.80f, 0.80f,   // ite 6
	                                                              0.90f, 0.90f,   // ite 7
	                                                              0.95f, 0.95f}); // ite 8

	bool siso_n(const int ite,
	            const mipp::vector<R>& sys,
	                  mipp::vector<R>& ext,
	                  mipp::vector<B>& s);

	bool siso_i(const int ite,
	            const mipp::vector<R>& sys,
	                  mipp::vector<R>& ext);

	virtual Scaling_factor_array<B,R>* clone() const;
};
}
}

#endif
