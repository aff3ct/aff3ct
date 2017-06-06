#ifndef FACTORY_ENCODER_RSC_HPP
#define FACTORY_ENCODER_RSC_HPP

#include <string>
#include <vector>
#include <iostream>

#include "Module/Encoder/RSC/Encoder_RSC_sys.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_encoder_RSC : public Factory
{
	static module::Encoder_RSC_sys<B>* build(const std::string       type,
	                                         const int               K,
	                                         const int               N,
	                                         const bool              buffered = true,
	                                         const std::vector<int>  poly     = {5,7},
	                                               std::ostream     &stream   = std::cout,
	                                         const int               n_frames = 1);
};
}
}

#endif /* FACTORY_ENCODER_RSC_HPP */
