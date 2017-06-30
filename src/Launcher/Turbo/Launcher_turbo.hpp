#ifndef LAUNCHER_TURBO_HPP_
#define LAUNCHER_TURBO_HPP_

#include "Tools/Factory/Turbo/Factory_decoder_turbo.hpp"
#include "Tools/Factory/Turbo/Factory_encoder_turbo.hpp"
#include "Tools/Factory/Turbo/Factory_puncturer_turbo.hpp"

namespace aff3ct
{
namespace launcher
{
template <class cLauncher, typename B = int, typename R = float, typename Q = R, typename QD = Q>
class Launcher_turbo : public cLauncher
{
public:
	Launcher_turbo(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_turbo();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();
	virtual void print_header();

	virtual void build_codec();

	typename tools::Factory_puncturer_turbo<B,Q>::puncturer_parameters_turbo *m_pct = nullptr;
	typename tools::Factory_encoder_turbo  <B  >::encoder_parameters_turbo   *m_enc = nullptr;
	typename tools::Factory_decoder_turbo  <B,Q>::decoder_parameters_turbo   *m_dec = nullptr;
};
}
}

#include "Launcher_turbo.hxx"

#endif /* LAUNCHER_TURBO_HPP_ */
