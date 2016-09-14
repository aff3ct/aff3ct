#ifndef FACTORY_CRC_HPP
#define FACTORY_CRC_HPP

#include "Tools/params.h"

#include "Module/CRC/CRC.hpp"

template <typename B>
struct Factory_CRC
{
	static CRC<B>* build(const parameters &params);
};

#endif /* FACTORY_CRC_HPP */
