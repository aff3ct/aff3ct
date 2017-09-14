#ifndef LAUNCHER_RA_HPP_
#define LAUNCHER_RA_HPP_

#include "Factory/Module/Codec/RA/Codec_RA.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class RA : public L
{
protected:
	factory::Codec_RA::parameters *params_cdc;

public:
	RA(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~RA();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();
	virtual void print_header();
};
}
}

#endif /* LAUNCHER_RA_HPP_ */
