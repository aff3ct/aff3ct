#ifndef LAUNCHER_UNCODED_HPP_
#define LAUNCHER_UNCODED_HPP_

#include <iostream>

#include "Factory/Tools/Codec/Uncoded/Codec_uncoded.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class Uncoded : public L
{
protected:
	factory::Codec_uncoded *params_cdc;

public:
	Uncoded(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Uncoded() = default;

protected:
	virtual void get_description_args();
	virtual void store_args();
};
}
}

#endif /* LAUNCHER_UNCODED_HPP_ */
