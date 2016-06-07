#ifndef FACTORY_INTERLEAVER_HPP
#define FACTORY_INTERLEAVER_HPP

#include "../../Interleaver/Interleaver.hpp"
#include "../../Interleaver/Interleaver_LTE.hpp"
#include "../../Interleaver/Interleaver_NO.hpp"
#include "../../Interleaver/Interleaver_columns.hpp"
#include "../../Interleaver/Interleaver_golden.hpp"
#include "../../Interleaver/Interleaver_random.hpp"

#include "../params.h"

template <typename T>
struct Factory_interleaver
{
	static Interleaver<T>* build(const t_code_param &code_params,
	                             const int          &size);
};

#endif /* FACTORY_INTERLEAVER_HPP */