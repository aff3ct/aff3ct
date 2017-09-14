#ifndef LAUNCHER_POLAR_HPP_
#define LAUNCHER_POLAR_HPP_

#include "Factory/Module/Codec/Polar/Codec_polar.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class Polar : public L
{
protected:
	factory::Codec_polar::parameters *params_cdc;

public:
	Polar(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Polar();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();
	virtual void print_header();
};
}
}

#endif /* LAUNCHER_POLAR_HPP_ */
