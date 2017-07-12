#ifndef LAUNCHER_TURBO_HPP_
#define LAUNCHER_TURBO_HPP_

#include "Tools/Factory/Code/Turbo/Factory_decoder_turbo.hpp"
#include "Tools/Factory/Code/Turbo/Factory_encoder_turbo.hpp"
#include "Tools/Factory/Code/Turbo/Factory_puncturer_turbo.hpp"

namespace aff3ct
{
namespace launcher
{
template <class cLauncher, typename B = int, typename R = float, typename Q = R, typename QD = Q>
class Launcher_turbo : public cLauncher
{
protected:
	tools::Factory_encoder_turbo  ::parameters *m_enc = nullptr;
	tools::Factory_puncturer_turbo::parameters *m_pct = nullptr;
	tools::Factory_decoder_turbo  ::parameters *m_dec = nullptr;

public:
	Launcher_turbo(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_turbo();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();
	virtual void print_header();

	virtual void build_codec();
};
}
}

#include "Launcher_turbo.hxx"

#endif /* LAUNCHER_TURBO_HPP_ */
