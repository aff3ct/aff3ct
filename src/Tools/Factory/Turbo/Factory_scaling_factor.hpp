#ifndef FACTORY_SCALING_FACTOR_HPP
#define FACTORY_SCALING_FACTOR_HPP

#include <string>

#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
struct Factory_scaling_factor : public Factory
{
	static Scaling_factor<B,R>* build(const std::string         type,
	                                  const int                 n_ite,
	                                  const float               cst = 0.75f,// siso_n siso_i
	                                  const mipp::vector<float> alpha_array = {0.15f, 0.15f,   // ite 1
	                                                                           0.25f, 0.25f,   // ite 2
	                                                                           0.30f, 0.30f,   // ite 3
	                                                                           0.40f, 0.40f,   // ite 4
	                                                                           0.70f, 0.70f,   // ite 5
	                                                                           0.80f, 0.80f,   // ite 6
	                                                                           0.90f, 0.90f,   // ite 7
	                                                                           0.95f, 0.95f}); // ite 8
};
}
}

#endif /* FACTORY_SCALING_FACTOR_HPP */
