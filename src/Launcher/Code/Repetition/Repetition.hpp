#ifndef LAUNCHER_REPETITION_HPP_
#define LAUNCHER_REPETITION_HPP_

#include <iostream>

#include "Factory/Tools/Codec/Repetition/Codec_repetition.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class Repetition : public L
{
protected:
	factory::Codec_repetition *params_cdc;

public:
	Repetition(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Repetition() = default;

protected:
	virtual void get_description_args();
	virtual void store_args();
};
}
}

#endif /* LAUNCHER_REPETITION_HPP_ */
