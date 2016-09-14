#ifndef FACTORY_INTERLEAVER_HPP
#define FACTORY_INTERLEAVER_HPP

#include "Tools/params.h"

#include "Module/Interleaver/Interleaver.hpp"

template <typename T>
struct Factory_interleaver
{
	static Interleaver<T>* build(const parameters &params, const int &size, const int seed = -1);
};

#endif /* FACTORY_INTERLEAVER_HPP */
