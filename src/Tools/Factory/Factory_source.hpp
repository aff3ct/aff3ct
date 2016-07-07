#ifndef FACTORY_SOURCE_HPP
#define FACTORY_SOURCE_HPP

#include "../../Source/Source.hpp"

#include "../params.h"

template <typename B>
struct Factory_source
{
	static Source<B>* build(const t_code_param &code_params, const int seed = 0);
};

#endif /* FACTORY_SOURCE_HPP */