#ifndef FACTORY_INTERLEAVER_HPP
#define FACTORY_INTERLEAVER_HPP

#include "../../Interleaver/Interleaver.hpp"

#include "../params.h"

template <typename T>
struct Factory_interleaver
{
	static Interleaver<T>* build(const t_code_param &code_params,
	                             const int          &size);
};

#endif /* FACTORY_INTERLEAVER_HPP */