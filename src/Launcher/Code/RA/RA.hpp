#ifndef LAUNCHER_RA_HPP_
#define LAUNCHER_RA_HPP_

#include <iostream>

#include "Factory/Tools/Codec/RA/Codec_RA.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class RA : public L
{
protected:
	factory::Codec_RA *params_cdc;

public:
	RA(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~RA() = default;

protected:
	virtual void get_description_args();
	virtual void store_args();
};
}
}

#endif /* LAUNCHER_RA_HPP_ */
