#ifndef FACTORY_CRC_HPP
#define FACTORY_CRC_HPP

#include "Module/CRC/CRC.hpp"
#include "Tools/params.h"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_CRC : public Factory
{
	static module::CRC<B>* build(const parameters &params);
};
}
}

#endif /* FACTORY_CRC_HPP */
