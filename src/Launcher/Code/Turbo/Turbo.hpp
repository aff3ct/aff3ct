#ifndef LAUNCHER_TURBO_HPP_
#define LAUNCHER_TURBO_HPP_

#include "Factory/Module/Codec/Turbo/Codec_turbo.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class Turbo : public L
{
protected:
	factory::Codec_turbo::parameters *params_cdc;

public:
	Turbo(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Turbo();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();
	virtual void print_header();
};
}
}

#endif /* LAUNCHER_TURBO_HPP_ */
