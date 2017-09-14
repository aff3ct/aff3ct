#ifndef LAUNCHER_REPETITION_HPP_
#define LAUNCHER_REPETITION_HPP_

#include "Factory/Module/Codec/Repetition/Codec_repetition.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class Repetition : public L
{
protected:
	factory::Codec_repetition::parameters *params_cdc;

public:
	Repetition(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Repetition();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();
	virtual void print_header();
};
}
}

#endif /* LAUNCHER_REPETITION_HPP_ */
