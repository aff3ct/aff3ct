#ifndef LAUNCHER_TURBO_HPP_
#define LAUNCHER_TURBO_HPP_

#include <iostream>

#include "Factory/Tools/Codec/Turbo/Codec_turbo.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class Turbo : public L
{
protected:
	factory::Codec_turbo *params_cdc;

public:
	Turbo(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Turbo() = default;

protected:
	virtual void get_description_args();
	virtual void store_args();
};
}
}

#endif /* LAUNCHER_TURBO_HPP_ */
