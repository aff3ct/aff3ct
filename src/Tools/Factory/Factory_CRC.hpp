#ifndef FACTORY_CRC_HPP
#define FACTORY_CRC_HPP

#include <string>

#include "Module/CRC/CRC.hpp"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_CRC : public Factory
{
	static module::CRC<B>* build(const std::string type,
	                             const int         K,
	                             const int         size,
	                             const std::string poly,
	                             const int         n_frames = 1);
};
}
}

#endif /* FACTORY_CRC_HPP */
