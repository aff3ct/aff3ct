#ifndef LAUNCHER_BCH_HPP_
#define LAUNCHER_BCH_HPP_

#include <iostream>

#include "Factory/Tools/Codec/BCH/Codec_BCH.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class BCH : public L
{
protected:
	factory::Codec_BCH *params_cdc;

public:
	BCH(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~BCH() = default;

protected:
	virtual void get_description_args();
	virtual void store_args();
};
}
}

#endif /* LAUNCHER_BCH_HPP_ */
