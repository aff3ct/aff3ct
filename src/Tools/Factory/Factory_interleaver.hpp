#ifndef FACTORY_INTERLEAVER_HPP
#define FACTORY_INTERLEAVER_HPP

#include "../../Interleaver/Interleaver.hpp"

#include "../params.h"

template <typename T>
struct Factory_interleaver
{
	static Interleaver<T>* build(const t_code_param &code_params,
	                             const int          &size,
	                             const int           seed = -1);
};

#endif /* FACTORY_INTERLEAVER_HPP */