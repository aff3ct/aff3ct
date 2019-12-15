#ifndef LAUNCHER_RSC_HPP_
#define LAUNCHER_RSC_HPP_

#include <iostream>

#include "Factory/Tools/Codec/RSC/Codec_RSC.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class RSC : public L
{
protected:
	factory::Codec_RSC *params_cdc;

public:
	RSC(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~RSC() = default;

protected:
	virtual void get_description_args();
	virtual void store_args();
};
}
}

#endif /* LAUNCHER_RSC_HPP_ */
