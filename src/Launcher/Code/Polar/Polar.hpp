#ifndef LAUNCHER_POLAR_HPP_
#define LAUNCHER_POLAR_HPP_

#include <iostream>

#include "Factory/Tools/Codec/Polar/Codec_polar.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class Polar : public L
{
protected:
	factory::Codec_polar *params_cdc;

public:
	Polar(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Polar() = default;

protected:
	virtual void get_description_args();
	virtual void store_args();
};
}
}

#endif /* LAUNCHER_POLAR_HPP_ */
