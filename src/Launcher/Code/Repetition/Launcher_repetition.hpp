#ifndef LAUNCHER_REPETITION_HPP_
#define LAUNCHER_REPETITION_HPP_

#include "Tools/Factory/Code/Repetition/Factory_decoder_repetition.hpp"
#include "Tools/Factory/Code/Repetition/Factory_encoder_repetition.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B = int, typename R = float, typename Q = R>
class Launcher_repetition : public C
{
protected:
	tools::Factory_encoder_repetition::parameters *m_enc = nullptr;
	tools::Factory_decoder_repetition::parameters *m_dec = nullptr;

public:
	Launcher_repetition(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_repetition();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();
	virtual void print_header();

	virtual void build_codec();
};
}
}

#include "Launcher_repetition.hxx"

#endif /* LAUNCHER_REPETITION_HPP_ */
