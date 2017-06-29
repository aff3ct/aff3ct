#ifndef LAUNCHER_REPETITION_HPP_
#define LAUNCHER_REPETITION_HPP_

#include "Tools/Factory/Repetition/Factory_decoder_repetition.hpp"
#include "Tools/Factory/Repetition/Factory_encoder_repetition.hpp"

namespace aff3ct
{
namespace launcher
{
template <class cLauncher, typename B = int, typename R = float, typename Q = R>
class Launcher_repetition : public cLauncher
{
public:
	Launcher_repetition(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_repetition();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();
	virtual void print_header();

	virtual void build_codec();

	typename tools::Factory_encoder_repetition<B  >::encoder_parameters_repetition *m_enc = nullptr;
	typename tools::Factory_decoder_repetition<B,Q>::decoder_parameters            *m_dec = nullptr;
};
}
}

#include "Launcher_repetition.hxx"

#endif /* LAUNCHER_REPETITION_HPP_ */
