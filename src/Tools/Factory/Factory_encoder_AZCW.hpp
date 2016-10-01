#ifndef FACTORY_ENCODER_AZCW_HPP
#define FACTORY_ENCODER_AZCW_HPP

#include "Module/Encoder/Encoder_sys.hpp"
#include "Tools/params.h"

#include "Factory.hpp"

template <typename B>
struct Factory_encoder_AZCW : public Factory
{
	static Encoder_sys<B>* build(const parameters &params);
};

#endif /* FACTORY_ENCODER_AZCW_HPP */
