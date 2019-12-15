#ifndef LAUNCHER_RS_HPP_
#define LAUNCHER_RS_HPP_

#include <iostream>

#include "Factory/Tools/Codec/RS/Codec_RS.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class RS : public L
{
protected:
	factory::Codec_RS *params_cdc;

public:
	RS(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~RS() = default;

protected:
	virtual void get_description_args();
	virtual void store_args();
};
}
}

#endif /* LAUNCHER_RS_HPP_ */
