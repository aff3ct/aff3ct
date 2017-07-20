#ifndef LAUNCHER_POLAR_HPP_
#define LAUNCHER_POLAR_HPP_

#include "Factory/Module/Code/Polar/Puncturer_polar.hpp"
#include "Factory/Module/Code/Polar/Decoder_polar.hpp"
#include "Factory/Module/Code/Polar/Encoder_polar.hpp"
#include "Factory/Tools/Code/Polar/Frozenbits_generator.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B = int, typename R = float, typename Q = R>
class Launcher_polar : public C
{
protected:
	factory::Frozenbits_generator::parameters *params_fb  = nullptr;
	factory::Puncturer_polar     ::parameters *params_pct = nullptr;
	factory::Encoder_polar       ::parameters *params_enc = nullptr;
	factory::Decoder_polar       ::parameters *params_dec = nullptr;

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
