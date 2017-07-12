#ifndef LAUNCHER_POLAR_HPP_
#define LAUNCHER_POLAR_HPP_

#include "Tools/Factory/Code/Polar/Factory_decoder_polar.hpp"
#include "Tools/Factory/Code/Polar/Factory_puncturer_polar.hpp"
#include "Tools/Factory/Code/Polar/Factory_encoder_polar.hpp"

namespace aff3ct
{
namespace launcher
{
template <class cLauncher, typename B = int, typename R = float, typename Q = R>
class Launcher_polar : public cLauncher
{
protected:
	tools::Factory_encoder_polar  ::parameters *m_enc = nullptr;
	tools::Factory_puncturer_polar::parameters *m_pct = nullptr;
	tools::Factory_decoder_polar  ::parameters *m_dec = nullptr;

public:
	Launcher_polar(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_polar();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();
	virtual void print_header();

	virtual void build_codec();
};
}
}

#include "Launcher_polar.hxx"

#endif /* LAUNCHER_POLAR_HPP_ */
