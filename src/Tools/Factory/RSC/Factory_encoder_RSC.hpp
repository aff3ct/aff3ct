#ifndef FACTORY_ENCODER_RSC_HPP
#define FACTORY_ENCODER_RSC_HPP

#include <iostream>

#include "Module/Encoder/RSC/Encoder_RSC_sys.hpp"
#include "Tools/params.h"

#include "../Factory.hpp"

template <typename B>
struct Factory_encoder_RSC : public Factory
{
	static Encoder_RSC_sys<B>* build(const parameters &params, const int n_frames = 1,
	                                 std::ostream &stream = std::cout);
};

#endif /* FACTORY_ENCODER_RSC_HPP */
