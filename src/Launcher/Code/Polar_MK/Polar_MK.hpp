#ifndef LAUNCHER_POLAR_MK_HPP_
#define LAUNCHER_POLAR_MK_HPP_

#include <iostream>

#include "Factory/Tools/Codec/Polar_MK/Codec_polar_MK.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class Polar_MK : public L
{
protected:
	factory::Codec_polar_MK *params_cdc;

public:
	Polar_MK(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Polar_MK() = default;

protected:
	virtual void get_description_args();
	virtual void store_args();
};
}
}

#endif /* LAUNCHER_POLAR_MK_HPP_ */
