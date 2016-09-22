#ifndef FACTORY_CRC_HPP
#define FACTORY_CRC_HPP

#include "Module/CRC/CRC.hpp"
#include "Tools/params.h"

#include "Factory.hpp"

template <typename B>
struct Factory_CRC : public Factory
{
	static CRC<B>* build(const parameters &params);
};

#endif /* FACTORY_CRC_HPP */
