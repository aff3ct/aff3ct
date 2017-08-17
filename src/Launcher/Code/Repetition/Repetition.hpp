#ifndef LAUNCHER_REPETITION_HPP_
#define LAUNCHER_REPETITION_HPP_

#include "Factory/Module/Code/Repetition/Decoder_repetition.hpp"
#include "Factory/Module/Code/Repetition/Encoder_repetition.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B = int, typename R = float, typename Q = R>
class Repetition : public C
{
protected:
	factory::Encoder_repetition::parameters *params_enc = nullptr;
	factory::Decoder_repetition::parameters *params_dec = nullptr;

public:
	Repetition(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Repetition();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void print_header();

	virtual void build_codec();
};
}
}

#include "Repetition.hxx"

#endif /* LAUNCHER_REPETITION_HPP_ */
